#include <utility>

#include "indexer.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <queue>
#include <list>
#include <unordered_map>
#include <numeric>

#include "FeatAggregator.h"
#include "../html/html.h"
#include "../dto/DateStamp.h"
#include "../dto/Line.h"
#include "../cli/MsgInterface.h"
#include "../exception/file_access_failure.h"
#include "../exception/file_parsing_failure.h"
#include "../exception/failed_expectation.h"
#include "../fs/fs.h"
#include "../html/editor/editor.h"

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

    bool   doFeatured = !global_options->_landing_page.featured.empty();
    bool   doByYear   = global_options->_index.index_by_year;
    bool   doByTag    = global_options->_index.index_by_tag;
    bool   doByAuthor = global_options->_index.index_by_author;

    size_t optional = ( doFeatured ? 2 : 0 )
                    + ( doByYear ? 1 : 0 )
                    + ( doByTag ? 1 : 0 )
                    + ( doByAuthor ? 1 : 0 );

    display.begin( "Indexing", ( 5 + optional ), "CSS" );
    indexStylesheets( *global_options, *index );

    display.progress( "Templates" );
    indexTemplates( *global_options, *index );

    display.progress( "Posts" );
    indexPosts( *global_options, *index );

    display.progress( "sorting articles by date" );
    sortChronologically( index->_articles, *global_options );

    if( doFeatured ) {
        display.progress( "checking featured sources" );
        checkFeaturedExist( *global_options );
        display.progress( "generating featured targets" );
        indexFeatured( feat_aggregator, *index );
    }

    display.progress( "generating chrono index targets" );
    generateChronologicalIndexTargets( *index, *global_options );

    if( global_options->_index.index_by_year ) {
        display.progress( "generating 'by year' targets" );
        index->_indices.byYear.page_count += 1; //year list page
        generateYearIndexTargets( *index, *global_options );
    }

    if( global_options->_index.index_by_tag ) {
        display.progress( "generating 'by tag' targets" );
        index->_indices.byTag.page_count += 1; //tag list page
        generateTagIndexTargets( *index, *global_options );
        generateTopTags( *index, *global_options );
    }

    if( global_options->_index.index_by_author ) {
        display.progress( "generating 'by author' targets" );
        index->_indices.byAuthor.page_count += 1; //author list page
        generateAuthorIndexTargets( *index, *global_options );
        generateTopAuthors( *index, *global_options );
    }

    display.progress( "DONE" );
    return std::move( index );
}

/**
 * Indexes the stylesheets (CSS) files
 * @param global_options Options DTO
 * @param index          Index DTO
 */
void blogator::indexer::indexStylesheets( const dto::Options &global_options, dto::Index &index ) {
    auto &display = cli::MsgInterface::getInstance();

    static const std::regex blogator_css_rx  = std::regex( R"(^.*blogator\.css$)" );

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.css_dir ) ) {
        if( p.is_regular_file() ) {
            if( std::regex_match( p.path().string(), blogator_css_rx ) ) {
                index._paths.css.blog  = p.path();
            }
        }
    }

    if( index._paths.css.blog.empty() ) {
        std::ofstream output( index._paths.css.blog = global_options._paths.css_dir / "blogator.css" );
        display.msg( "No master stylesheet was found for the articles. A blank one was created." );
    }
}

/**
 * Indexes the template files
 * @param global_options Options DTO
 * @param index          Index DTO
 * @throws exception::failed_expectation when detecting missing template file(s)
 */
void blogator::indexer::indexTemplates( const dto::Options &global_options, dto::Index &index ) {
    static const std::regex landing_rx       = std::regex( R"(^.*[\/\\]landing\.(?:html|htm)$)" );
    static const std::regex post_rx          = std::regex( R"(^.*[\/\\]post\.(?:html|htm)$)" );
    static const std::regex index_rx         = std::regex( R"(^.*[\/\\]index\.(?:html|htm)$)" );
    static const std::regex index_list_rx    = std::regex( R"(^.*[\/\\]index_list\.(?:html|htm)$)" );
    static const std::regex year_list_rx     = std::regex( R"(^.*[\/\\]year_list\.(?:html|htm)$)" );
    static const std::regex year_index_rx    = std::regex( R"(^.*[\/\\]year_index\.(?:html|htm)$)" );
    static const std::regex tag_list_rx      = std::regex( R"(^.*[\/\\]tag_list\.(?:html|htm)$)" );
    static const std::regex tag_index_rx     = std::regex( R"(^.*[\/\\]tag_index\.(?:html|htm)$)" );
    static const std::regex author_list_rx   = std::regex( R"(^.*[\/\\]author_list\.(?:html|htm)$)" );
    static const std::regex author_index_rx  = std::regex( R"(^.*[\/\\]author_index\.(?:html|htm)$)" );
    static const std::regex index_entry_rx   = std::regex( R"(^.*[\/\\]index_entry\.(?:html|htm)$)" );

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.template_dir ) ) {
        if( p.is_regular_file() ) {
            auto path = p.path().string();
            if( std::regex_match( path, post_rx ) ) {
                index._paths.templates.post = p.path();
            } else if( std::regex_match( path, index_rx ) ) {
                index._paths.templates.index = p.path();
            } else if( std::regex_match( path, landing_rx ) ) {
                index._paths.templates.landing = p.path();
            } else if( std::regex_match( path, index_list_rx ) ) {
                index._paths.templates.index_list = p.path();
            } else if( std::regex_match( path, year_list_rx ) ) {
                index._paths.templates.year_list = p.path();
            } else if( std::regex_match( path, year_index_rx ) ) {
                index._paths.templates.year_index = p.path();
            } else if( std::regex_match( path, tag_list_rx ) ) {
                index._paths.templates.tag_list = p.path();
            } else if( std::regex_match( path, tag_index_rx ) ) {
                index._paths.templates.tag_index = p.path();
            } else if( std::regex_match( path, author_list_rx ) ) {
                index._paths.templates.author_list = p.path();
            } else if( std::regex_match( path, author_index_rx ) ) {
                index._paths.templates.author_index = p.path();
            } else if( std::regex_match( path, index_entry_rx ) ) {
                index._paths.templates.index_entry = p.path();
            }
        }
    }

    auto &display = cli::MsgInterface::getInstance();
    size_t errors { 0 };

    if( index._paths.templates.landing.empty() ) {
        display.error( "Template missing: landing page" );
        ++errors;
    }
    if( index._paths.templates.post.empty() ) {
        display.error( "Template missing: post page" );
        ++errors;
    }
    if( index._paths.templates.index.empty() ) {
        display.error( "Template missing: index page" );
        ++errors;
    }
    if( index._paths.templates.index_list.empty() ) {
        display.error( "Template missing: index list page" );
        ++errors;
    }
    if( index._paths.templates.index_entry.empty() ) {
        display.error( "Template missing: index entry" );
        ++errors;
    }

    if( errors > 0 )
        throw exception::failed_expectation( "Missing template(s)." );
}

/**
 * Indexes the posts (articles) along with any custom index entries and stylesheets
 * @param global_options Options DTO
 * @param index          Master index
 * @throws exception::failed_expectation when no source post files were found to index
 */
void blogator::indexer::indexPosts( const dto::Options &global_options, dto::Index &index ) {
    static auto &display = cli::MsgInterface::getInstance();

    static const std::regex index_entry_html_rx = std::regex( R"(^(.*[\/\\].+)(?:_index)\.(?:htm|html)$)" );
    static const std::regex html_rx             = std::regex( "^.+\\.(?:htm|html)$" );
    static const std::regex css_rx              = std::regex( R"(^(.*[\/\\].+)\.(css)$)" );

    auto index_entry_files = std::unordered_map<std::string, std::filesystem::path>();
    auto custom_css_files  = std::unordered_map<std::string, std::filesystem::path>();
    auto removal_queue     = std::deque<std::filesystem::path>(); //removed article paths
    auto index_entry_match = std::smatch();
    auto custom_css_match  = std::smatch();

    for( auto &p: std::filesystem::recursive_directory_iterator( global_options._paths.source_dir ) ) {
        if( p.is_regular_file() ) {
            auto path = p.path().string();
            if( std::regex_match( path, index_entry_match, index_entry_html_rx ) ) {
                if( index_entry_match.size() > 1 )
                    index_entry_files.emplace(
                        std::make_pair( index_entry_match[ 1 ].str(), p.path() )
                    );
            } else if( std::regex_match( path, custom_css_match, css_rx ) ) {
                if( custom_css_match.size() > 1 )
                    custom_css_files.emplace(
                        std::make_pair( custom_css_match[ 1 ].str(), p.path() )
                    );
            } else if( std::regex_match( path, html_rx ) ) {
                try {
                    auto &article = index._articles.emplace_back( readFileProperties( global_options, p.path() ) );

                    if( !global_options._posts.build_future &&
                        article._datestamp > global_options.getRuntimeDateStamp() )
                    {
                        std::stringstream ss;
                        ss << "Skipping future post (" << article._datestamp << " > " << global_options.getRuntimeDateStamp() << "): "
                           << article._paths.src_html.lexically_relative( global_options._paths.root_dir );
                        display.debug( ss.str() );

                        removal_queue.emplace_back( article._paths.src_html );
                        index._articles.erase( std::prev( index._articles.end() ) );

                    } else {
                        addYear( global_options, article, index );
                        addTags( global_options, article, index );
                        addAuthors( global_options, article, index );

                        if( global_options._index.show_summary && article._summary.html._lines.empty() )
                            display.debug(
                                "No summary found in: " + article._paths.src_html.lexically_relative( global_options._paths.root_dir ).string()
                            );
                    }

                } catch( std::exception &e ) {
                    display.error( e.what() );
                }
            }
        }
    }

    while( !removal_queue.empty() ) { //Remove corresponding custom CSS (if any) of removed articles
        const auto &p        = removal_queue.front();
        const auto file_root = p.parent_path() / p.filename().stem();
        auto       css_it    = custom_css_files.find( file_root.string() );

        if( css_it != custom_css_files.end() )
            custom_css_files.erase( css_it );

        removal_queue.pop_front();
    }

    if( index._articles.empty() )
        throw exception::failed_expectation( "No articles (posts) to index found." );

    //Add any * css files to their respective articles css path if found and
    //add any *_index html files to their respective articles index entry path if found
    if( !index_entry_files.empty() || !custom_css_files.empty() ) {
        for( auto &article : index._articles ) {
            auto file_root = article._paths.src_html.parent_path() / article._paths.src_html.filename().stem();
            auto file_rel  = global_options._folders.posts.root / article._paths.src_html.lexically_relative( global_options._paths.source_dir );
            auto entry_it  = index_entry_files.find( file_root.string() );
            auto css_it    = custom_css_files.find( file_root.string() );

            if( entry_it != index_entry_files.end() ) {
                display.debug(
                    "Found custom index entry: " + file_rel.string() + " -> " +
                    ( global_options._folders.posts.root / entry_it->second.lexically_relative( global_options._paths.source_dir ) ).string()
                );
                article._paths.entry_html = entry_it->second;

                cacheCustomIndexEntry( global_options, article );

                index_entry_files.erase( entry_it );
            }

            if( css_it != custom_css_files.end() ) {
                display.debug(
                    "Found custom stylesheet entry: " + file_rel.string() + " -> " +
                    ( global_options._folders.posts.root / css_it->second.lexically_relative( global_options._paths.source_dir ) ).string()
                );
                article._paths.css = css_it->second;
                custom_css_files.erase( css_it );
            }
        }

        if( !custom_css_files.empty() ) {
            for( const auto &e : custom_css_files ) {
                display.warning(
                    "Found orphaned custom css entry: " +
                    ( global_options._folders.posts.root / e.second.lexically_relative( global_options._paths.source_dir ) ).string()
                );
            }
        }

        if( !index_entry_files.empty() ) {
            for( const auto &e : index_entry_files ) {
                display.warning(
                    "Found orphaned custom index entry: " +
                    ( global_options._folders.posts.root / e.second.lexically_relative( global_options._paths.source_dir ) ).string()
                );
            }
        }
    }
}

/**
 * Index featured posts for the landing page
 * @param feat_aggregator Featured aggregator
 * @param index           Index DTO
 */
void blogator::indexer::indexFeatured( indexer::FeatAggregator &feat_aggregator,
                                       dto::Index &index )
{
    for( auto &article : index._articles ) {
        article._is_featured = feat_aggregator.addArticleIfFeatured( article );
    }

    index._featured = feat_aggregator.getFeaturedArticles();
}

/**
 * Adds year of the article if new to the index's global Year set along with
 * their corresponding year directory IDs
 * @param global_options Global blogator options
 * @param article        Article from which to extract tags to add to the global set
 * @param master_index   Master index file
 */
void blogator::indexer::addYear( const dto::Options &global_options,
                                 const dto::Article &article,
                                 dto::Index &master_index )
{
    auto year = std::to_string( article._datestamp._year );
    master_index._indices.byYear.cats.insert(
        std::make_pair( year, std::to_string( master_index._indices.byYear.cats.size() ) )
    );
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
        master_index._indices.byTag.cats.insert(
            std::make_pair( tag, std::to_string( master_index._indices.byTag.cats.size() ) )
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
        master_index._indices.byAuthor.cats.insert(
            std::make_pair( author, std::to_string( master_index._indices.byAuthor.cats.size() ) )
        );
    }
}

/**
 * Sort the articles in chronological order from newest to oldest (via the DateStamp) + creates the individual filenames
 * @param articles       Collection of articles
 * @param global_options Global blogator options
 */
void blogator::indexer::sortChronologically( blogator::dto::Index::Articles_t &articles, const dto::Options &global_options ) {

    std::sort( articles.begin(),
               articles.end(),
               []( const dto::Article &a, const dto::Article &b ) {
                   return dto::DateStamp::compare( a._datestamp, b._datestamp ) > 0;
               }
    );

    size_t i = 0;
    for( auto &article : articles ) {
        article._number = ( articles.size() - i );
        addOutputPath( global_options._paths, article );
        ++i;
    }
}

/**
 * Generates all tag index targets (output index file names and article indices for each)
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateYearIndexTargets( dto::Index &master_index,
                                                  const dto::Options &global_options )
{
    { //YEARLY ARTICLE INDEX MAPPING
        auto & map = master_index._indices.byYear.cats;
        size_t i   = 0;

        for( const auto & article : master_index._articles ) {
            auto year = std::to_string( article._datestamp._year );
            auto year_it = map.find( year );

            if( year_it != map.end() )
                year_it->second.article_indices.emplace_back( i );

            ++i;
        }
    }

    { //YEARLY INDEX FILE NAMES
        for( auto &tag : master_index._indices.byYear.cats ) {
            if( !tag.second.article_indices.empty() ) {
                const size_t page_count = calcPageCount( global_options._index.items_per_page,
                                                         tag.second.article_indices.size() );

                for( size_t p = 0; p < page_count; ++p )
                    tag.second.file_names.emplace_back( makeFileName( tag.second.prefix_id, p ) );

                master_index._indices.byYear.page_count += page_count;
            }
        }
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
        auto & map = master_index._indices.byTag.cats;
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
        for( auto &tag : master_index._indices.byTag.cats ) {
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
        auto & map = master_index._indices.byAuthor.cats;
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
        for( auto &author : master_index._indices.byAuthor.cats ) {
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

    for( const auto &t : master_index._indices.byTag.cats )
        max_heap.push( std::make_pair( t.second.article_indices.size(), t.first ) );

    size_t i = 0;
    while( !max_heap.empty() && i < global_options._landing_page.top_tags ) {
        master_index._indices.byTag.top.emplace_back( max_heap.top().second );
        max_heap.pop();
        ++i;
    }
}

/**
 * Generates an ordered (desc) top authors mentioned in the author tags in the index articles
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateTopAuthors( dto::Index &master_index,
                                            const dto::Options &global_options )
{
    auto compare  = []( auto &a, auto &b ) { return a.first < b.first; };
    auto max_heap = std::priority_queue<std::pair<size_t, std::string>,
        std::vector<std::pair<size_t, std::string>>,
        decltype( compare )
    >( compare );

    for( const auto &t : master_index._indices.byAuthor.cats )
        max_heap.push( std::make_pair( t.second.article_indices.size(), t.first ) );

    size_t i = 0;
    while( !max_heap.empty() && i < global_options._landing_page.top_authors ) {
        master_index._indices.byAuthor.top.emplace_back( max_heap.top().second );
        max_heap.pop();
        ++i;
    }
}

/**
 * Generates chronological index filenames
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
void blogator::indexer::generateChronologicalIndexTargets( dto::Index & master_index,
                                                           const dto::Options & global_options )
{
    master_index._indices.chronological.page_count = calcPageCount( global_options._index.items_per_page,
                                                                    master_index._articles.size() );

    for( size_t p = 0; p < master_index._indices.chronological.page_count; ++p ) {
        master_index._indices.chronological.file_names.emplace_back( makeFileName( p ) );
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
 * @param global_options Global Options DTO
 * @return Article DTO
 * @throws blogator::exception::file_parsing_failure when issues are raised during content parsing
 */
blogator::dto::Article blogator::indexer::readFileProperties( const dto::Options &options, const std::filesystem::path &path ) {
    static auto &display = cli::MsgInterface::getInstance();

    const static auto heading_tag = std::make_pair<std::string, std::string>( "<span class=\"title\">", "</span>" );
    const static auto author_tag  = std::make_pair<std::string, std::string>( "<span class=\"author\">", "</span>" );
    const static auto date_tag    = std::make_pair<std::string, std::string>( "<time datetime=\"", "\"" );
    const static auto tag_tag     = std::make_pair<std::string, std::string>( "<span class=\"tag\">", "</span>" );
    const static auto code_tag    = std::make_pair<std::string, std::string>( "<code", "</code>" );

//    const static auto heading_rx = std::regex( "<h1>\\s*(.*)\\s*</h1>" );
//    const static auto date_rx    = std::regex( "<time datetime=\"(\\d{4})-(\\d{2})-(\\d{2})\">" );
//    const static auto tag_rx     = std::regex( R"(<span class="tag">\s*(.*)\s*</span>)" );
//    const static auto author_rx  = std::regex( R"(<span class="author">\s*(.*)\s*</span>)" );

    auto article           = blogator::dto::Article();
    bool heading_found     = false;
    bool date_found        = false;
    auto summary_positions = std::deque<dto::InsertPosition>();

    article._paths.src_html = path;

    std::string   line;
    size_t        line_count = 0;
    std::ifstream html_file( path.string() );

    try {
        using html::reader::getContentBetween;
        using html::reader::getContentsBetween;
        using html::reader::getSummaryPositions;
        using html::reader::getTags;

        if( html_file.is_open() ) {
            while( getline( html_file, line ) ) {

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

                if( options._index.show_summary )
                    getSummaryPositions( line_count, line, summary_positions );

                auto authors = getContentsBetween( author_tag.first, author_tag.second, line );
                auto tags    = getContentsBetween( tag_tag.first, tag_tag.second, line );

                std::copy( authors.begin(), authors.end(),
                           std::inserter( article._authors, article._authors.begin() )
                );
                std::copy( tags.begin(), tags.end(),
                           std::inserter( article._tags, article._tags.begin() )
                );

                ++line_count;
            }

            html_file.close();

            { //caching summary and extracting all relative paths found within
                auto summary_range_pos = std::list<dto::SeekRange>();

                while( summary_positions.size() >= 2 ) {
                    auto start = summary_positions.front();
                    summary_positions.pop_front();
                    auto finish = summary_positions.front();
                    summary_positions.pop_front();
                    summary_range_pos.emplace_back( dto::SeekRange( start, finish ) );
                }

                article._summary.html           = fs::importHTML( article._paths.src_html, summary_range_pos );

                auto hrefs_count = html::editor::removeHyperlinks( article._summary.html );

                if( hrefs_count > 0 )
                    display.debug(
                        "Removed " + std::to_string( hrefs_count ) + " hyperlinks in post's summary text: " +
                        path.lexically_relative( options._paths.root_dir ).string()
                    );

                article._summary.path_write_pos = dto::Templates::extractRelativePaths( article._summary.html );
            }

        } else {
            throw exception::file_access_failure( "Could not open: " + path.string() );
        }

    } catch( std::invalid_argument &e ) {
        throw exception::file_parsing_failure(
            "Failed parsing for post '" + path.lexically_relative( options._paths.root_dir ).string() + "': " + e.what()
        );
    }

    if( !heading_found )
        throw exception::file_parsing_failure(
            "No title (<span class=\"title\">..</span>) found in post: " + path.lexically_relative( options._paths.root_dir ).string()
        );
    if( !date_found )
        throw exception::file_parsing_failure(
            "No date (<time>) found in post: " + path.lexically_relative( options._paths.root_dir ).string()
        );
    if( summary_positions.size() % 2 > 0 )
        display.error(
            "Missing open/close 'summary' tag detected in: " + path.lexically_relative( options._paths.root_dir ).string()
        );

    if( article._tags.empty() )
        article._tags.emplace( "N/A" );

    return article;
}

/**
 * Caches a custom index entry for an Article
 * @param options Global Options DTO
 * @param article Article DTO
 */
void blogator::indexer::cacheCustomIndexEntry( const dto::Options & options, dto::Article & article ) {
    static auto &display   = cli::MsgInterface::getInstance();
    try {
        auto entry             = std::make_shared<dto::Template>( dto::Template::Type::INDEX_ENTRY );
        entry->src             = article._paths.entry_html;
        entry->html            = fs::importHTML( entry->src );

        auto hrefs_count = html::editor::removeHyperlinks( *entry->html );

        if( hrefs_count > 0 )
            display.debug(
                "Removed " + std::to_string( hrefs_count ) + " hyperlinks in custom index entry: " +
                article._paths.entry_html.lexically_relative( options._paths.root_dir ).string()
            );

        entry->block_write_pos = html::reader::getConsecutiveWritePositions( *entry->html, entry->block_classes );
        entry->path_write_pos  = dto::Templates::extractRelativePaths( *entry->html );

        fs::checkTemplateRelPaths( *entry );

        article._cust_index_entry = entry;

    } catch( exception::file_access_failure &e ) {
        display.error( e.what() );
    }
}

/**
 * Creates the absolute output path of the post
 * @param global_paths Global paths set in dto::Options
 * @param article      Article to create an output path for
 */
void blogator::indexer::addOutputPath( const dto::Options::AbsPaths &global_paths,
                                       blogator::dto::Article &article )
{
    auto file_name = std::to_string( article._number ) + article._paths.src_html.filename().extension().string();
    auto abs_path  = global_paths.posts_dir / file_name;
    article._paths.out_html = abs_path.lexically_relative( global_paths.posts_dir );
}

/**
 * Checks the Option's landing page featured article list has valid paths
 * @param options Global Options DTO
 */
void blogator::indexer::checkFeaturedExist( const dto::Options &options ) {
    static auto &display = cli::MsgInterface::getInstance();

    for( const auto &f : options._landing_page.featured ) {
        if( !std::filesystem::exists( options._paths.source_dir / f.first ) ) {
            display.error(
                "Featured article in configuration file does not exists (i:" + std::to_string( f.second ) + "): " + f.first
            );
        }
    }
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