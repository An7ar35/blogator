#include "indexer.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>

#include "../html/html.h"
#include "../dto/DateStamp.h"

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
    static const std::regex header_regx = std::regex( "^.*(header)\\.(?:html|htm)$" );
    static const std::regex footer_regx = std::regex( "^.*(footer)\\.(?:html|htm)$" );
    static const std::regex html_regx   = std::regex( "^.*(?:html|htm)$" );

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.source_dir ) ) {
        if( p.is_regular_file() ) {
            if( std::regex_match( p.path().string(), header_regx ) ) {
                index->_paths.header = p.path();
            } else if( std::regex_match( p.path().string(), footer_regx ) ) {
                index->_paths.footer = p.path();
            } else if( std::regex_match( p.path().string(), html_regx ) ) {
                index->_articles.emplace_back( readFileProperties( p.path() ) );
                addTags( index->_articles.back(),*index );
                addCSS( css_cache, index->_articles.back() );
            }
        }
    }

    if( index->_paths.header.empty() )
        throw std::runtime_error( "No header file found." );
    if( index->_paths.footer.empty() )
        throw  std::runtime_error( "No footer file found." );
    if( index->_articles.empty() )
        throw  std::runtime_error( "No files to index found." );

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
    auto it = found_stylesheets.find( article._html_filepath.stem().string() );

    if( it != found_stylesheets.end() ) {
        article._css_filepath = it->second;
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
 */
blogator::dto::Article blogator::indexer::readFileProperties( const std::filesystem::path &path ) {
    auto article       = blogator::dto::Article();
    bool heading_found = false;
    bool date_found    = false;

    article._html_filepath = path;

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
            std::cout << "could not open: " << path.string() << std::endl;
        }

    } catch( std::invalid_argument &e ) {
        std::cerr << e.what() << std::endl; //TODO error control: better flaging of error for controller
    }

    return article;
}


