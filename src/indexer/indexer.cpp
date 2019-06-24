#include "indexer.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>

#include "../html/html.h"
#include "../dto/DateStamp.h"
#include "../exception/file_access_failure.h"
#include "../exception/file_parsing_failure.h"

/**
 * Initialise the index
 * @param global_options Global blogator options
 * @return Initialised Index
 * @throws std::runtime_error when the minimum requirements are not met (i.e.: missing files)
 */
std::unique_ptr<blogator::dto::Index> blogator::indexer::index( const blogator::dto::Options &global_options ) {
    auto index     = std::make_unique<dto::Index>();
    auto css_cache = std::unordered_map<std::string, std::filesystem::path>(); //(K=filename, V=absolute path)

    //====CSS====
    static const std::regex posts_css_regex = std::regex( "^.*\\.(css)$" );
    static const std::regex blog_css_regex  = std::regex( "^.*(blog)\\.(css)$" );
    static const std::regex index_css_regex = std::regex( "^.*(index)\\.(css)$" );

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.css_dir ) ) {
        if( p.is_regular_file() ) {
            if( std::regex_match( p.path().string(), blog_css_regex ) ) {
                index->_paths.blog_css = p.path();
            } else if( std::regex_match( p.path().string(), index_css_regex ) ) {
                index->_paths.index_css = p.path();
            } else if( std::regex_match( p.path().string(), posts_css_regex ) ) {
                css_cache.emplace( std::make_pair( p.path().stem().string(), p.path() ) );
            }
        }
    }

    if( index->_paths.blog_css.empty() ) {
        std::ofstream output( index->_paths.blog_css = global_options._paths.css_dir / "blog.css" );
        std::cerr << "No master stylesheet was found for the articles. A blank one was created." << std::endl;
    }
    if( index->_paths.index_css.empty() ) {
        std::ofstream output( index->_paths.index_css = global_options._paths.css_dir / "index.css" );
        std::cerr << "No master stylesheet was found for the indices. A blank one was created." << std::endl;
    }

    //====HTML====
    static const std::regex template_post_rx    = std::regex( "^.*(template_post)\\.(?:html|htm)$" );
    static const std::regex template_index_rx   = std::regex( "^.*(template_index)\\.(?:html|htm)$" );
    static const std::regex template_start_rx   = std::regex( "^.*(template_start)\\.(?:html|htm)$" );
    static const std::regex index_entry_html_rx = std::regex( "^(?:.*\\/)(.+)(?:_index)\\.(?:htm|html)$" );
    static const std::regex html_rx             = std::regex( "^.+\\.(?:htm|html)$" );

    auto index_entry_files = std::unordered_map<std::string, std::filesystem::path>();
    auto index_entry_match = std::smatch();

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.source_dir ) ) {
        if( p.is_regular_file() ) {
            auto path = p.path().string();
            if( std::regex_match( path, template_post_rx ) ) {
                index->_paths.post_template = p.path();
            } else if( std::regex_match( path, template_index_rx ) ) {
                index->_paths.index_template = p.path();
            } else if( std::regex_match( path, template_start_rx ) ) {
                index->_paths.start_template = p.path();
            } else if( std::regex_match( path, index_entry_match, index_entry_html_rx ) ) {
                if( index_entry_match.size() > 1 )
                    index_entry_files.emplace(
                        std::make_pair( index_entry_match[1].str(), p.path() )
                    );
            } else if( std::regex_match( path, html_rx ) ) {
                index->_articles.emplace_back( readFileProperties( p.path() ) );
                addTags( index->_articles.back(),*index );
                addCSS( css_cache, index->_articles.back() );
                addOutputPath( global_options._paths, index->_articles.back() );
            }
        }
    }

    if( index->_paths.post_template.empty() )
        throw std::runtime_error( "No post page template file found." );
    if( index->_paths.index_template.empty() )
        throw std::runtime_error( "No index page template file found." );
    if( index->_paths.start_template.empty() )
        throw std::runtime_error( "No start page template file found." );
    if( index->_articles.empty() )
        throw  std::runtime_error( "No files to index found." );

    //Add any *_index html files to their respective articles index entry path if found
    if( !index_entry_files.empty() )
        for( auto &article : index->_articles ) {
            auto entry_it = index_entry_files.find( article._paths.src_html.filename().stem().string() );
            if( entry_it != index_entry_files.end() )
                article._paths.entry_html = entry_it->second;
        }

    std::sort( index->_articles.begin(),
               index->_articles.end(),
               []( const dto::Article &a, const dto::Article &b ) {
                    return dto::DateStamp::compare( a._datestamp, b._datestamp ) > 0;
               }
    );

    return std::move( index );
}

/**
 * Adds new tags found in article into the index's global Tag set
 * @param article      Article from which to extract tags to add to the global set
 * @param master_index Master index file
 */
void blogator::indexer::addTags( const blogator::dto::Article &article,
                                 blogator::dto::Index &master_index )
{
    std::transform( article._tags.begin(),
                    article._tags.end(),
                    std::inserter( master_index._global_tags, master_index._global_tags.end() ),
                    []( const std::string &tag ) { return std::make_pair( tag, std::filesystem::path() ); }
    );
}

/**
 * Adds the path of any stylesheets matching the file name of the article source HTML
 * @param found_stylesheets Collection of stylesheets found in the Options' "css" folder
 * @param article Article to amend its stylesheet to if found
 */
void blogator::indexer::addCSS( std::unordered_map<std::string, std::filesystem::path> &found_stylesheets,
                                blogator::dto::Article &article )
{
    auto it = found_stylesheets.find( article._paths.src_html.stem().string() );

    if( it != found_stylesheets.end() ) {
        article._paths.css = it->second;
        found_stylesheets.erase( it );
    }
}

/**
 * Converts a 'timedate' into a DateStamp object
 * @param date string representation of the html <time timedate='..'>..</time> tag
 * @return DateStamp object
 * @throws std::invalid_argument when the given string cannot be converted into a DateStamp object
 */
blogator::dto::DateStamp blogator::indexer::convertDate( const std::string & date ) {
    auto date_rx = std::regex( R"(.*?(\d{4})-(\d{2})-(\d{2}))" );
    std::smatch matches;
    std::regex_search( date, matches, date_rx );

    if( matches.length() < 4 )
        throw std::invalid_argument( "Malformed date found in <time> tag: " + date );

    try {
        return blogator::dto::DateStamp(
            std::stoi( matches[ 1 ] ),
            std::stoi( matches[ 2 ] ),
            std::stoi( matches[ 3 ] )
        );
    } catch( std::invalid_argument &e ) {
        throw std::invalid_argument( "Date found in <time> tag could not be converted to DateStamp: " + date );
    }
}

/**
 * Gets all the properties (heading, tags, date) from an html post
 * @param path Path of the html file
 * @return Article DTO
 * @throws blogator::exception::file_parsing_failure when issues are raised during content parsing
 */
blogator::dto::Article blogator::indexer::readFileProperties( const std::filesystem::path &path ) {
    auto article       = blogator::dto::Article();
    bool heading_found = false;
    bool date_found    = false;

    article._paths.src_html = path;

    std::string   line;
    std::ifstream html_file( path.string() );

    try {
        if( html_file.is_open() ) {
            while( getline( html_file, line ) ) {
                auto trim_begin = line.begin();
                auto trim_end = std::find_if( trim_begin, line.end(),
                                              ( []( int c ) { return !isspace( c ); } ) );

                line.erase( trim_begin, trim_end );

                if( !heading_found ) {
                    article._heading = html::reader::getHeading( line );
                    heading_found = !article._heading.empty();
                }

                if( !date_found ) {
                    auto date = html::reader::getDate( line );
                    if( !date.empty() ) {
                        article._datestamp = convertDate( date );
                        date_found = true;
                    }
                }

                auto tags = html::reader::getTags( line );

                std::copy( tags.begin(), tags.end(),
                           std::inserter( article._tags, article._tags.begin() ) );
            }

            html_file.close();

        } else {
            throw exception::file_access_failure( "Could not open: " + path.string() );
        }

    } catch( std::invalid_argument &e ) {
        throw exception::file_parsing_failure( "Failed parsing for post '" + path.string() + "': " + e.what() );
    }

    if( !heading_found )
        throw exception::file_parsing_failure( "No heading (h1) found in post: " + path.string() );
    if( !date_found )
        throw exception::file_parsing_failure( "No date (<time>) found in post: " + path.string() );

    if( article._tags.empty() )
        article._tags.emplace( "no tag" );

    return article;
}

/**
 * Creates the absolute output path of the post
 * @param global_paths Global paths set in dto::Options
 * @param article      Article to create an output path for
 */
void blogator::indexer::addOutputPath( const dto::Options::Paths &global_paths,
                                       blogator::dto::Article &article )
{
    auto abs_path = global_paths.posts_dir / article._paths.src_html.filename();
    article._paths.out_html = abs_path.lexically_relative( global_paths.posts_dir );
}


