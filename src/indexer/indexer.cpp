#include "indexer.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <queue>
#include <list>
#include <unordered_map>

#include "FeatAggregator.h"
#include "../html/html.h"
#include "../dto/DateStamp.h"
#include "../exception/file_access_failure.h"
#include "../exception/file_parsing_failure.h"
#include "../exception/failed_expectation.h"
#include "../cli/MsgInterface.h"

/**
 * Initialise the index
 * @param global_options Global blogator options
 * @return Initialised Index
 * @throws std::failed_expectation when the minimum requirements are not met (i.e.: missing files)
 */
std::shared_ptr<blogator::dto::Index> blogator::indexer::index( const std::shared_ptr<dto::Options> &global_options ) {
    auto index           = std::make_shared<dto::Index>();
    auto css_cache       = std::unordered_map<std::string, std::filesystem::path>(); //(K=filename, V=absolute path)
    auto feat_aggregator = indexer::FeatAggregator( global_options );
    auto &display        = cli::MsgInterface::getInstance();

    display.begin( "Indexing", 5, "CSS" );
    indexStylesheets( *global_options, *index, css_cache );
    display.progress( "Templates" );
    indexTemplates( *global_options, *index );
    display.progress( "Posts" );
    indexPosts( *global_options, *index, css_cache, feat_aggregator );
    display.progress( "sorting articles by date" );
    sortChronologically( index->_articles );
    display.progress( "generating targets" );
    generateDateIndexTargets( *index, *global_options );
    if( global_options->_index.index_by_tag ) {
        index->_indices.byTag.page_count += 1; //tag list page
        generateTagIndexTargets( *index, *global_options );
        generateTopTags( *index, *global_options );
    }
    if( global_options->_index.index_by_author ) {
        index->_indices.byAuthor.page_count += 1; //author list page
        generateAuthorIndexTargets( *index, *global_options );
    }
    display.progress( "DONE" );

    return std::move( index );
}

/**
 * Indexes the stylesheets (CSS) files
 * @param global_options Options DTO
 * @param index          Index DTO
 * @param css_cache      Container for caching the post-specific css files
 */
void blogator::indexer::indexStylesheets( const dto::Options &global_options,
                                          dto::Index &index,
                                          std::unordered_map<std::string, std::filesystem::path> &css_cache )
{
    static const std::regex blog_css_regex  = std::regex( "^.*(blog)\\.(css)$" );
    static const std::regex index_css_regex = std::regex( "^.*(index)\\.(css)$" );
    static const std::regex posts_css_regex = std::regex( "^.*\\.(css)$" );

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.css_dir ) ) {
        if( p.is_regular_file() ) {
            if( std::regex_match( p.path().string(), blog_css_regex ) ) {
                index._paths.css.blog = p.path();
            } else if( std::regex_match( p.path().string(), index_css_regex ) ) {
                index._paths.css.index = p.path();
            } else if( std::regex_match( p.path().string(), posts_css_regex ) ) {
                css_cache.emplace( std::make_pair( p.path().stem().string(), p.path() ) );
            }
        }
    }

    if( index._paths.css.blog.empty() ) {
        std::ofstream output( index._paths.css.blog = global_options._paths.css_dir / "blog.css" );
        std::cerr << "No master stylesheet was found for the articles. A blank one was created." << std::endl;
    }
    if( index._paths.css.index.empty() ) {
        std::ofstream output( index._paths.css.index = global_options._paths.css_dir / "index.css" );
        std::cerr << "No master stylesheet was found for the indices. A blank one was created." << std::endl;
    }
}

/**
 * Indexes the template files
 * @param global_options Options DTO
 * @param index          Index DTO
 * @throws exception::failed_expectation when detecting missing template file(s)
 */
void blogator::indexer::indexTemplates( const dto::Options &global_options, dto::Index &index ) {
    static const std::regex landing_rx       = std::regex( "^.*(template_start)\\.(?:html|htm)$" );
    static const std::regex landing_entry_rx = std::regex( "^.*(template_start_entry)\\.(?:html|htm)$" );
    static const std::regex post_rx          = std::regex( "^.*(template_post)\\.(?:html|htm)$" );
    static const std::regex index_rx         = std::regex( "^.*(template_index)\\.(?:html|htm)$" );
    static const std::regex tag_list_rx      = std::regex( "^.*(template_tag_list)\\.(?:html|htm)$" );
    static const std::regex author_list_rx   = std::regex( "^.*(template_author_list)\\.(?:html|htm)$" );
    static const std::regex index_entry_rx   = std::regex( "^.*(template_index_entry)\\.(?:html|htm)$" );

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.template_dir ) ) {
        if( p.is_regular_file() ) {
            auto path = p.path().string();
            if( std::regex_match( path, post_rx ) ) {
                index._paths.templates.post = p.path();
            } else if( std::regex_match( path, index_rx ) ) {
                index._paths.templates.index = p.path();
            } else if( std::regex_match( path, landing_rx ) ) {
                index._paths.templates.start = p.path();
            } else if( std::regex_match( path, landing_entry_rx ) ) {
                index._paths.templates.start_entry = p.path();
            } else if( std::regex_match( path, tag_list_rx ) ) {
                index._paths.templates.tag_list = p.path();
            } else if( std::regex_match( path, author_list_rx ) ) {
                index._paths.templates.author_list = p.path();
            } else if( std::regex_match( path, index_entry_rx ) ) {
                index._paths.templates.index_entry = p.path();
            }
        }
    }

    size_t errors { 0 }; //TODO pass msgs to MsgInterface instead of cerr? or redirect cerr to it;

    if( index._paths.templates.start.empty() ) {
        std::cerr << "Template missing: landing page" << std::endl;
        ++errors;
    }
    if( index._paths.templates.start_entry.empty() ) {
        std::cerr << "Template missing: landing entry" << std::endl;
        ++errors;
    }
    if( index._paths.templates.post.empty() ) {
        std::cerr << "Template missing: post page" << std::endl;
        ++errors;
    }
    if( index._paths.templates.index.empty() ) {
        std::cerr << "Template missing: index page" << std::endl;
        ++errors;
    }
    if( global_options._index.index_by_tag && index._paths.templates.tag_list.empty() ) {
        std::cerr << "Template missing: tag list page" << std::endl;
        ++errors;
    }
    if( global_options._index.index_by_author && index._paths.templates.author_list.empty() ) {
        std::cerr << "Template missing: author list page" << std::endl;
        ++errors;
    }
    if( index._paths.templates.index_entry.empty() ) {
        std::cerr << "Template missing: index entry" << std::endl;
        ++errors;
    }

    if( errors > 0 )
        throw exception::failed_expectation( "Missing template(s)." );
}

/**
 * Indexes the posts (articles)
 * @param global_options Options DTO
 * @param index          Master index
 * @param css_cache      Cached post-specific css files
 * @throws exception::failed_expectation when no source post files were found to index
 */
void blogator::indexer::indexPosts( const dto::Options &global_options,
                                    dto::Index &index,
                                    std::unordered_map<std::string, std::filesystem::path> &css_cache,
                                    FeatAggregator &feat_aggregator )
{
    static const std::regex index_entry_html_rx = std::regex( "^(?:.*\\/)(.+)(?:_index)\\.(?:htm|html)$" );
    static const std::regex html_rx             = std::regex( "^.+\\.(?:htm|html)$" );

    auto index_entry_files = std::unordered_map<std::string, std::filesystem::path>();
    auto index_entry_match = std::smatch();

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.source_dir ) ) {
        if( p.is_regular_file() ) {
            auto path = p.path().string();
            if( std::regex_match( path, index_entry_match, index_entry_html_rx ) ) {
                if( index_entry_match.size() > 1 )
                    index_entry_files.emplace(
                        std::make_pair( index_entry_match[1].str(), p.path() )
                    );
            } else if( std::regex_match( path, html_rx ) ) {
                index._articles.emplace_back( readFileProperties( p.path() ) );

                if( !global_options._posts.build_future &&
                    index._articles.back()._datestamp > global_options.getRuntimeDateStamp() )
                {
                    index._articles.erase( std::prev( index._articles.end() ) );
                } else {
                    addTags( global_options, index._articles.back(), index );
                    addAuthors( global_options, index._articles.back(), index );
                    addCSS( css_cache, index._articles.back() );
                    addOutputPath( global_options._paths, index._articles.back() );
                    feat_aggregator.addArticleIfFeatured( index._articles.back() );
                }
            }
        }
    }

    if( index._articles.empty() )
        throw exception::failed_expectation( "No articles (posts) to index found." );

    //Add any *_index html files to their respective articles index entry path if found
    if( !index_entry_files.empty() )
        for( auto &article : index._articles ) {
            auto entry_it = index_entry_files.find( article._paths.src_html.filename().stem().string() );
            if( entry_it != index_entry_files.end() )
                article._paths.entry_html = entry_it->second;
        }

    //Get the list of ordered Articles featured on the landing page
    index._featured = feat_aggregator.getFeaturedArticles();
}

/**
 * Adds new tags found in article into the index's global Tag set along with
 * their corresponding tag directories IDs
 * @param global_options Global blogator options
 * @param article        Article from which to extract tags to add to the global set
 * @param master_index   Master index file
 */
void blogator::indexer::addTags( const dto::Options &global_options,
                                 const blogator::dto::Article &article,
                                 blogator::dto::Index &master_index )
{
    for( const auto &tag : article._tags ) {
        master_index._indices.byTag.tags.insert(
            std::make_pair( tag, std::to_string( master_index._indices.byTag.tags.size() ) )
        );
    }
}

/**
 * Adds new authors found in article into the index's global Author set along with
 * their corresponding author directories IDs
 * @param global_options Global blogator options
 * @param article        Article from which to extract authors to add to the global set
 * @param master_index   Master index file
 */
void blogator::indexer::addAuthors( const dto::Options &global_options,
                                    const dto::Article &article,
                                    dto::Index &master_index )
{
    for( const auto &author : article._authors ) {
        master_index._indices.byAuthor.authors.insert(
            std::make_pair( author, std::to_string( master_index._indices.byAuthor.authors.size() ) )
        );
    }
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
 * Sort the articles in chronological order from newest to oldest (via the DateStamp)
 * @param articles Collection of articles
 */
void blogator::indexer::sortChronologically( blogator::dto::Index::Articles_t &articles ) {

    std::sort( articles.begin(),
               articles.end(),
               []( const dto::Article &a, const dto::Article &b ) {
                   return dto::DateStamp::compare( a._datestamp, b._datestamp ) > 0;
               }
    );

    size_t i = 0;
    for( auto &article : articles ) {
        article._number = ( articles.size() - i );
        ++i;
    }
}

/**
 * Generates all tag index targets (output index file names and article indices for each)
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateTagIndexTargets( dto::Index &master_index,
                                                 const dto::Options &global_options )
{
    { //TAG ARTICLE INDEX MAPPING
        auto & map = master_index._indices.byTag.tags;
        size_t i   = 0;

        for( const auto & article : master_index._articles ) {

            for( const auto & tag : article._tags ) {
                auto tag_it = map.find( tag );

                if( tag_it != map.end() )
                    tag_it->second.article_indices.emplace_back( i );

            }

            ++i;
        }
    }

    { //TAG INDEX FILE NAMES
        for( auto &tag : master_index._indices.byTag.tags ) {
            if( !tag.second.article_indices.empty() ) {
                const size_t page_count = calcPageCount( global_options._index.items_per_page,
                                                         tag.second.article_indices.size() );

                for( size_t p = 0; p < page_count; ++p )
                    tag.second.file_names.emplace_back( makeFileName( tag.second.prefix_id, p ) );

                master_index._indices.byTag.page_count += page_count;
            }
        }
    }
}

/**
 * Generates all author index targets (output index file names and article indices for each)
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateAuthorIndexTargets( dto::Index& master_index,
                                                    const dto::Options &global_options )
{
    { //AUTHOR ARTICLE INDEX MAPPING
        auto & map = master_index._indices.byAuthor.authors;
        size_t i   = 0;

        for( const auto & article : master_index._articles ) {

            for( const auto & author : article._authors ) {
                auto author_it = map.find( author );

                if( author_it != map.end() )
                    author_it->second.article_indices.emplace_back( i );

            }

            ++i;
        }
    }

    { //AUTHOR INDEX FILE NAMES
        for( auto &author : master_index._indices.byAuthor.authors ) {
            if( !author.second.article_indices.empty() ) {
                const size_t page_count = calcPageCount( global_options._index.items_per_page,
                                                         author.second.article_indices.size() );

                for( size_t p = 0; p < page_count; ++p )
                    author.second.file_names.emplace_back( makeFileName( author.second.prefix_id, p ) );

                master_index._indices.byAuthor.page_count += page_count;
            }
        }
    }
}

/**
 * Generates an ordered (desc) top tags used by the articles in the index
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateTopTags( blogator::dto::Index &master_index,
                                         const blogator::dto::Options &global_options )
{
    auto compare  = []( auto &a, auto &b ) { return a.first < b.first; };
    auto max_heap = std::priority_queue<std::pair<size_t, std::string>,
                                        std::vector<std::pair<size_t, std::string>>,
                                        decltype( compare )
                                       >( compare );

    for( const auto &t : master_index._indices.byTag.tags )
        max_heap.push( std::make_pair( t.second.article_indices.size(), t.first ) );

    size_t i = 0;
    while( !max_heap.empty() && i < global_options._landing_page.top_tags ) {
        master_index._indices.byTag.top_tags.emplace_back( max_heap.top().second );
        max_heap.pop();
        ++i;
    }
}

/**
 * Generates date index filenames
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateDateIndexTargets( blogator::dto::Index &master_index,
                                                  const blogator::dto::Options &global_options )
{
    master_index._indices.byDate.page_count = calcPageCount( global_options._index.items_per_page,
                                                             master_index._articles.size() );

    for( size_t p = 0; p < master_index._indices.byDate.page_count; ++p )
        master_index._indices.byDate.page_file_names.emplace_back( makeFileName( p ) );
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
    const static auto heading_tag = std::make_pair<std::string, std::string>( "<h1>", "</h1>" );
    const static auto author_tag  = std::make_pair<std::string, std::string>( "<span class=\"author\">", "</span>" );
    const static auto date_tag    = std::make_pair<std::string, std::string>( "<time datetime=\"", "\"" );

    auto article       = blogator::dto::Article();
    bool heading_found = false;
    bool date_found    = false;

    article._paths.src_html = path;

    std::string   line;
    std::ifstream html_file( path.string() );

    try {
        using html::reader::getContentBetween;
        using html::reader::getContentsBetween;
        using html::reader::getTags;

        if( html_file.is_open() ) {
            while( getline( html_file, line ) ) {
                auto trim_begin = line.begin();
                auto trim_end = std::find_if( trim_begin, line.end(),
                                              ( []( int c ) { return !isspace( c ); } ) );

                line.erase( trim_begin, trim_end );

                if( !heading_found ) {
                    article._heading = getContentBetween( heading_tag.first, heading_tag.second, line );
                    heading_found = !article._heading.empty();
                }

                if( !date_found ) {
                    auto date = getContentBetween( date_tag.first, date_tag.second, line );
                    if( !date.empty() ) {
                        article._datestamp = convertDate( date );
                        date_found = true;
                    }
                }

                auto authors = getContentsBetween( "<span class=\"author\">", "</span>", line );
                auto tags    = getTags( line ); //TODO look into modifying this to getContentsBetween

                std::copy( authors.begin(), authors.end(),
                           std::inserter( article._authors, article._authors.begin() )
                );
                std::copy( tags.begin(), tags.end(),
                           std::inserter( article._tags, article._tags.begin() )
                );
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
void blogator::indexer::addOutputPath( const dto::Options::AbsPaths &global_paths,
                                       blogator::dto::Article &article )
{
    auto abs_path = global_paths.posts_dir / article._paths.src_html.filename();
    article._paths.out_html = abs_path.lexically_relative( global_paths.posts_dir );
}

/**
 * Creates a file name based on a page number 'n'
 * @param n Page number
 * @return File name formatted as '{n}.html'
 */
std::filesystem::path blogator::indexer::makeFileName( const size_t & n ) {
    std::stringstream ss;
    ss << n << ".html";
    return std::filesystem::path( ss.str() );
}

/**
 * Creates a file name based on a page number 'n'
 * @param prefix File name prefix
 * @param n      Page number
 * @return File name formatted as '{prefix}_{n}.html'
 */
std::filesystem::path blogator::indexer::makeFileName( const std::string &prefix, const size_t &n ) {
    std::stringstream ss;
    ss << prefix << "_" << n << ".html";
    return std::filesystem::path( ss.str() );
}

/**
 * Calculates a page count required for 'n' items
 * @param items_per_page Items per page
 * @param item_count     Total number of items to fit
 * @return Number of pages required
 */
size_t blogator::indexer::calcPageCount( const size_t &items_per_page, const size_t & item_count ) {
    return ( item_count / items_per_page ) + ( ( item_count % items_per_page ) > 0 );
}