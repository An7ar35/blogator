#include "ConfigReader.h"
#include <fstream>
#include <iostream>

#include "../exception/file_access_failure.h"
#include "../exception/file_parsing_failure.h"
#include "../exception/failed_expectation.h"
#include "../cli/MsgInterface.h"

/**
 * Initialise configuration file reader/import
 * @param file_path Configuration file path
 * @return Option DTO with all the config file options loaded in
 * @throws exception::file_access_failure when config file could not be accessed
 */
std::shared_ptr<blogator::dto::Options> blogator::fs::ConfigReader::init( const std::filesystem::path &file_path ) const
{
    auto &display = cli::MsgInterface::getInstance();
    auto options  = std::make_shared<blogator::dto::Options>();
    auto map      = std::unordered_map<std::string, Value>();
    std::stringstream ss;

    try {
        loadConfigurationFile( file_path, map );
    } catch( exception::file_access_failure &e ) {
        display.error( "Configuration reader: " + std::string( e.what() ) );
        throw;
    }

    try {
        processTemplateOptions( map, *options );
    } catch( exception::file_parsing_failure &e ) {
        display.error( "Configuration reader: " + std::string( e.what() ) );
        throw;
    }

    try {
        processPostsOptions( map, *options );
    } catch( exception::file_parsing_failure &e ) {
        display.error( "Configuration reader: " + std::string( e.what() ) );
        throw;
    }

    try {
        processIndexOptions( map, *options );
        processLandingPageOptions( map, *options );
        processPageNavOptions( map, *options );
        processBreadcrumbOptions( map, *options );
    } catch( exception::file_parsing_failure &e ) {
        display.error( "Configuration reader: " + std::string( e.what() ) );
        throw;
    }

    try {
        processMonthsOptions( map, *options );
    } catch( exception::file_parsing_failure &e ) {
        display.error( "Configuration reader: " + std::string( e.what() ) );
        display.msg( "Using default month strings (eng)." );
    }

    try {
        processRssOptions( map, *options );
    } catch( exception::file_parsing_failure &e ) {
        options->_rss.generate = false;
        display.error( "Configuration reader: " + std::string( e.what() ) );
        display.msg( "RSS feed will *not* be generated." );
    } catch( exception::failed_expectation &e ) {
        options->_rss.generate = false;
        display.error( "Configuration reader: " + std::string( e.what() ) );
        display.msg( "RSS feed will *not* be generated." );
    }

    try {
        processJsonOptions( map, *options );
    } catch( exception::file_parsing_failure &e ) {
        display.error( "Configuration reader: " + std::string( e.what() ) );
        display.msg( "JSON Index will *not* be generated." );
    }

    return std::move( options );
}

/**
 * Generates a blank configuration template file
 * @param file_path Output path for the file
 */
void blogator::fs::ConfigReader::generateBlankConfigFile( const std::filesystem::path & file_path ) {
    std::stringstream ss;
    ss << "// -------------------------------\n"
       << "// | Blogator configuration file |\n"
       << "// -------------------------------\n"
       << "site-url = \"www.website-domain.com\";\n"
       << "\n"
       << "//Month strings\n"
       << "months = [\"January\", \"February\", \"March\", \"April\", \"May\", \"June\", \"July\", \"August\", \"September\", \"October\", \"November\", \"December\"];\n"
       << "\n"
       << "//Templates\n"
       << "template-change-paths = false;\n"
       << "\n"
       << "//Posts settings\n"
       << "build-future       = false;\n"
       << "safe-purge         = true;\n"
       << "posts-change-paths = false;\n"
       << "toc-auto-generate  = 0;\n"
       << "toc-level-offset   = 1;\n"
       << "toc-auto-numerate  = true;\n"
       << "toc-heading        = \"<h2>Table of Contents</h2>\";\n"
       << "\n"
       << "//Index settings\n"
       << "show-post-numbers  = true;\n"
       << "show-post-summary  = false;\n"
       << "post-summary-pads  = [\"<p>\", \"</p>\"];\n"
       << "featured-css-class = \"featured\";\n"
       << "items-per-page     = 10;\n"
       << "index-by-year      = false;\n"
       << "index-by-tag       = true;\n"
       << "index-by-author    = false;\n"
       << "json-index         = false;\n"
       << "json-index-append  = [];\n"
       << "\n"
       << "//Per-Page navigation DIV contents\n"
       << "page-nav-separator = \" / \";\n"
       << "page-nav-forward   = \">>\";\n"
       << "page-nav-backwards = \"<<\";\n"
       << "page-nav-first     = \"first\";\n"
       << "page-nav-last      = \"last\";\n"
       << "\n"
       << "//breadcrumb navigation DIV contents\n"
       << "breadcrumb-landing-page   = \"Home\";\n"
       << "breadcrumb-by-date-page   = \"Posts\";\n"
       << "breadcrumb-by-year-page   = \"Year\";\n"
       << "breadcrumb-by-tag-page    = \"Categories\";\n"
       << "breadcrumb-by-author-page = \"Authors\";\n"
       << "breadcrumb-index-page     = \"Page #\";\n"
       << "\n"
       << "//Landing page (i.e.: start page)\n"
       << "landing-most-recent = 5;\n"
       << "landing-top-tags    = 3;\n"
       << "landing-top_authors = 3;\n"
       << "landing-featured    = [\"0.html\", \"1.html\"];\n"
       << "landing-duplicates  = true;\n"
       << "\n"
       << "//RSS header settings\n"
       << "rss             = true;\n"
       << "rss-title       = \"website title\";\n"
       << "rss-description = \"site description\";\n"
       << "rss-copyright   = \"site copyright\";\n"
       << "rss-img-url     = \"logo.png\";\n"
       << "rss-img-link    = \"www.website-domain.com\";\n"
       << "rss-img-width   = \"50 px\";\n"
       << "rss-img-height  = \"50 px\";\n"
       << "rss-img-alt     = \"site logo\";";

    if( std::filesystem::exists( file_path ) )
        throw exception::file_access_failure(
            "File '" + file_path.string() + "' already exists. Delete first before generating a configuration file template."
        );

    auto config_out = std::ofstream( file_path );

    if( !config_out.is_open() )
        throw exception::file_access_failure(
            "File '" + file_path.string() + "' could not be opened for writing."
        );

    config_out << ss.str();
    config_out.close();
}

/**
 * Loads the configuration file into a map of raw {K,V}
 * @param path Configuration file path
 * @param map  Raw map container
 * @throws exception::file_access_failure when file cannot be opened for reading
 */
void blogator::fs::ConfigReader::loadConfigurationFile( const std::filesystem::path &path,
                                                        std::unordered_map<std::string, ConfigReader::Value> &map ) const
{
    auto &display = cli::MsgInterface::getInstance();

    std::string   line;
    std::ifstream config_file( path );
    size_t        line_number = 0;

    if( !config_file.is_open() )
        throw exception::file_access_failure( "Could not open: " + path.string() );

    while( getline( config_file, line ) ) {
        ++line_number;
        std::smatch matches;
        if( !std::regex_search( line, COMMENT_RX ) ) {
            if( std::regex_search( line, matches, KV_BOOL_VAL_RX) && matches.length() >= 3 ) {
                map.emplace( std::make_pair( matches[1], Value( line_number, Type::BOOLEAN, matches[2] ) ) );
            } else if( std::regex_search( line, matches, KV_STR_VAL_RX ) && matches.length() >= 3 ) {
                map.emplace( std::make_pair( matches[1], Value( line_number, Type::STRING, matches[2] ) ) );
            } else if( std::regex_search( line, matches, KV_INT_VAL_RX ) && matches.length() >= 3 ) {
                map.emplace( std::make_pair( matches[1], Value( line_number, Type::INTEGER, matches[2] ) ) );
            } else if( std::regex_search( line, matches, KV_STR_ARR_RX ) && matches.length() >= 3 ) {
                map.emplace( std::make_pair( matches[1], Value( line_number, Type::STRING_ARRAY, matches[2] ) ) );
            } else {
                display.error( "Configuration line #" + std::to_string( line_number ) + " could not be parsed." );
            }
        }
    }

    config_file.close();
}

/**
 * Process all Template specific options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 * @throws exception::file_parsing_failure when parsing failed
 */
void blogator::fs::ConfigReader::processTemplateOptions( std::unordered_map<std::string, ConfigReader::Value> &map,
                                                         dto::Options &options ) const
{
    static const std::string adapt_rel_paths = "template-change-paths";

    auto adapt_rel_paths_it = map.find( adapt_rel_paths );

    if( adapt_rel_paths_it != map.end() ) {
        if( adapt_rel_paths_it->second.type == Type::BOOLEAN ) {
            options._templates.adapt_rel_paths = ( adapt_rel_paths_it->second.value == "true" );
            adapt_rel_paths_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + adapt_rel_paths + "' value to boolean "
                "(line #" + std::to_string( adapt_rel_paths_it->second.line ) + "): " + adapt_rel_paths_it->second.value
            );
        }
    }

    _display.msg( "Relative paths adapt (templates) : ", options._templates.adapt_rel_paths, "TRUE", "FALSE" );
}

/**
 * Process all Post pages specific options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 * @throws exception::file_parsing_failure when parsing failed
 */
void blogator::fs::ConfigReader::processPostsOptions( std::unordered_map<std::string, ConfigReader::Value> &map,
                                                      dto::Options &options ) const
{
    static const std::string build_future    = "build-future";
    static const std::string safe_purge      = "safe-purge";
    static const std::string adapt_rel_paths = "posts-change-paths";
    static const std::string generate_toc    = "toc-auto-generate";
    static const std::string offset_toc      = "toc-level-offset";
    static const std::string numerate_toc    = "toc-auto-numerate";
    static const std::string toc_heading     = "toc-heading";

    auto build_future_it    = map.find( build_future );
    auto safe_purge_it      = map.find( safe_purge );
    auto adapt_rel_paths_it = map.find( adapt_rel_paths );
    auto generate_toc_it    = map.find( generate_toc );
    auto offset_toc_it      = map.find( offset_toc );
    auto numerate_toc_it    = map.find( numerate_toc );
    auto toc_heading_it     = map.find( toc_heading );

    if( build_future_it != map.end() ) {
        if( build_future_it->second.type == Type::BOOLEAN ) {
            options._posts.build_future = ( build_future_it->second.value == "true" );
            build_future_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + build_future + "' value to boolean "
                "(line #" + std::to_string( build_future_it->second.line ) + "): " + build_future_it->second.value
            );
        }
    }

    if( safe_purge_it != map.end() ) {
        if( safe_purge_it->second.type == Type::BOOLEAN ) {
            options._posts.safe_purge = ( safe_purge_it->second.value == "true" );
            safe_purge_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + safe_purge + "' value to boolean "
                "(line #" + std::to_string( safe_purge_it->second.line ) + "): " + safe_purge_it->second.value
            );
        }
    }

    if( adapt_rel_paths_it != map.end() ) {
        if( adapt_rel_paths_it->second.type == Type::BOOLEAN ) {
            options._posts.adapt_rel_paths = ( adapt_rel_paths_it->second.value == "true" );
            adapt_rel_paths_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + adapt_rel_paths + "' value to boolean "
                "(line #" + std::to_string( adapt_rel_paths_it->second.line ) + "): " + adapt_rel_paths_it->second.value
            );
        }
    }

    if( generate_toc_it != map.end() ) {
        try {
            auto levels = std::stoi( generate_toc_it->second.value );
            options._posts.toc.generate_toc = ( levels > 6 ? 6 : levels );
            generate_toc_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + generate_toc + "' value to integer "
                "(line #" + std::to_string( generate_toc_it->second.line ) + "): " + generate_toc_it->second.value
            );
        }
    }

    if( offset_toc_it != map.end() ) {
        try {
            auto offset = std::stoi( offset_toc_it->second.value );
            options._posts.toc.level_offset = ( offset > 5 ? 5 : offset );
            offset_toc_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + offset_toc + "' value to integer "
                "(line #" + std::to_string( offset_toc_it->second.line ) + "): " + offset_toc_it->second.value
            );
        }
    }

    if( adapt_rel_paths_it != map.end() ) {
        if( numerate_toc_it->second.type == Type::BOOLEAN ) {
            options._posts.toc.numbering = ( numerate_toc_it->second.value == "true" );
            numerate_toc_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + numerate_toc + "' value to boolean "
                "(line #" + std::to_string( numerate_toc_it->second.line ) + "): " + numerate_toc_it->second.value
            );
        }
    }

    if( toc_heading_it != map.end() ) {
        if( toc_heading_it->second.type == Type::STRING ) {
            options._posts.toc.heading = toc_heading_it->second.value;
            toc_heading_it->second.validated = true;
            _display.debug( "ToC heading .....................: \"" + toc_heading_it->second.value + "\"" );
        }
    }

    _display.msg( "Build future-dated posts ........: ", options._posts.build_future, "TRUE", "FALSE" );
    _display.msg( "Safe purge post output dir.......: ", options._posts.safe_purge, "TRUE", "FALSE" );
    _display.msg( "Relative paths adapt (posts) ....: ", options._posts.adapt_rel_paths, "TRUE", "FALSE" );
    _display.msg( "Auto-generate tables of contents : ", ( options._posts.toc.generate_toc > 0 ), "TRUE (depth: " + std::to_string( options._posts.toc.generate_toc ) + ", offset: " + std::to_string( options._posts.toc.level_offset ) + ")", "FALSE" );
}

/**
 * Process Months string replacements
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 * @throws exception::file_parsing_failure when parsing failed
 */
void blogator::fs::ConfigReader::processMonthsOptions( std::unordered_map<std::string, ConfigReader::Value> &map,
                                                       dto::Options &options ) const
{
    std::unordered_map<unsigned, std::string> month_map;

    static const std::string months = "months";
    auto months_it = map.find( months );

    if( months_it != map.end() ) {
        auto it = std::sregex_iterator( months_it->second.value.begin(),
                                        months_it->second.value.end(),
                                        QUOTED_STR_RX );
        size_t mm = 1;
        while( it != std::sregex_iterator() ) {
            if( it->size() > 1 && !it->str( 1 ).empty() )
                month_map.emplace( std::make_pair( mm, it->str( 1 ) ) );
            else
                throw exception::file_parsing_failure(
                    "Malformed month string in '" + months + "' array "
                    "(line #" + std::to_string( months_it->second.line ) + "): " + it->str()
                );

            ++mm;
            ++it;
        }

        months_it->second.validated = true;
        options._months = month_map;
    }
}

/**
 * Process all Index page specific options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 * @throws exception::file_parsing_failure when parsing failed
 */
void blogator::fs::ConfigReader::processIndexOptions( std::unordered_map<std::string, ConfigReader::Value> &map,
                                                      dto::Options &options ) const
{
    static const std::string items_per_page     = "items-per-page";
    static const std::string show_post_summary  = "show-post-summary";
    static const std::string post_summary_pads  = "post-summary-pads";
    static const std::string featured_css_class = "featured-css-class";
    static const std::string index_by_year      = "index-by-year";
    static const std::string index_by_tag       = "index-by-tag";
    static const std::string index_by_author    = "index-by-author";

    auto items_per_page_it     = map.find( items_per_page );
    auto show_post_summary_it  = map.find( show_post_summary );
    auto post_summary_pads_it  = map.find( post_summary_pads );
    auto featured_css_class_it = map.find( featured_css_class );
    auto index_by_year_it      = map.find( index_by_year );
    auto index_by_tag_it       = map.find( index_by_tag );
    auto index_by_author_it    = map.find( index_by_author );

    if( items_per_page_it != map.end() ) {
        try {
            options._index.items_per_page = std::stoi( items_per_page_it->second.value );
            items_per_page_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + items_per_page + "' value to integer "
                "(line #" + std::to_string( items_per_page_it->second.line ) + "): " + items_per_page_it->second.value
            );
        }
    }

    if( show_post_summary_it != map.end() ) {
        if( show_post_summary_it->second.type == Type::BOOLEAN ) {
            options._index.show_summary = ( show_post_summary_it->second.value == "true" );
            show_post_summary_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + show_post_summary + "' value to boolean "
                "(line #" + std::to_string( show_post_summary_it->second.line ) + "): " + show_post_summary_it->second.value
            );
        }
    }
            /* ^^ look above ^^ */
    if( options._index.show_summary && post_summary_pads_it != map.end() ) {
        if( post_summary_pads_it->second.type == Type::STRING_ARRAY ) {
            auto pads = std::vector<std::string>();

            auto it = std::sregex_iterator( post_summary_pads_it->second.value.begin(),
                                            post_summary_pads_it->second.value.end(),
                                            QUOTED_STR_RX );
            while( it != std::sregex_iterator() ) {
                if( it->size() > 1 && !it->str( 1 ).empty() )
                    pads.emplace_back( it->str( 1 ) );
                else
                    throw exception::file_parsing_failure(
                        "Malformed padding string in '" + post_summary_pads + "' array "
                        "(line #" + std::to_string( post_summary_pads_it->second.line ) + "): " + it->str()
                    );

                ++it;
            }

            if( pads.empty() )
                _display.msg( "No index summary padding provided in config file. Default (none) will be used." );

            if( pads.size() == 1 ) {
                options._index.summary_pad_begin = pads.at( 0 );
                post_summary_pads_it->second.validated = true;
                _display.debug( "Index summary padding ...........: \"" + options._index.summary_pad_begin + "\"" );
            }
            if( pads.size() > 1 ) {
                options._index.summary_pad_begin = pads.at( 0 );
                options._index.summary_pad_end   = pads.at( 1 );
                post_summary_pads_it->second.validated = true;
                _display.debug( "Index summary paddings ..........: \"" +  options._index.summary_pad_begin + "\"/\"" + options._index.summary_pad_end + "\"" );
            }
        }

        if( post_summary_pads_it->second.type == Type::STRING ) {
            options._index.summary_pad_begin = post_summary_pads_it->second.value;
            options._index.summary_pad_end   = post_summary_pads_it->second.value;
            post_summary_pads_it->second.validated = true;
            _display.debug( "Index summary padding \"" + post_summary_pads_it->second.value + "\"" );
        }
    }

    if( featured_css_class_it != map.end() && !featured_css_class_it->second.value.empty() ) {
        options._index.featured_css_class = featured_css_class_it->second.value;
        featured_css_class_it->second.validated = true;
    }

    if( index_by_year_it != map.end() ) {
        if( index_by_year_it->second.type == Type::BOOLEAN ) {
            options._index.index_by_year = ( index_by_year_it->second.value == "true" );
            index_by_year_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + index_by_year + "' value to boolean "
                "(line #" + std::to_string( index_by_year_it->second.line ) + "): " + index_by_year_it->second.value
            );
        }
    }

    if( index_by_tag_it != map.end() ) {
        if( index_by_tag_it->second.type == Type::BOOLEAN ) {
            options._index.index_by_tag = ( index_by_tag_it->second.value == "true" );
            index_by_tag_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + index_by_tag + "' value to boolean "
                "(line #" + std::to_string( index_by_tag_it->second.line ) + "): " + index_by_tag_it->second.value
            );
        }
    }

    if( index_by_author_it != map.end() ) {
        if( index_by_author_it->second.type == Type::BOOLEAN ) {
            options._index.index_by_author = ( index_by_author_it->second.value == "true" );
            index_by_author_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + index_by_author + "' value to boolean "
                "(line #" + std::to_string( index_by_author_it->second.line ) + "): " + index_by_author_it->second.value
            );
        }
    }

    _display.msg( "Insert summary in index entries .: ", options._index.show_summary, "TRUE", "FALSE" );
    _display.msg( "Insert 'featured' CSS class .....: ", !options._index.featured_css_class.empty(), "\"" + options._index.featured_css_class + "\"", "N/A" );
    _display.msg( "Build index 'by-year' ...........: ", options._index.index_by_year, "TRUE", "FALSE" );
    _display.msg( "Build index 'by-tag' ............: ", options._index.index_by_tag, "TRUE", "FALSE" );
    _display.msg( "Build index 'by-author'..........: ", options._index.index_by_author, "TRUE", "FALSE" );
}

/**
 * Process all Landing Page specific options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 * @throws exception::file_parsing_failure when parsing failed
 */
void blogator::fs::ConfigReader::processLandingPageOptions( std::unordered_map<std::string, ConfigReader::Value> &map,
                                                            dto::Options &options ) const
{
    static const std::string most_recent = "landing-most-recent";
    static const std::string top_tags    = "landing-top-tags";
    static const std::string top_authors = "landing-top_authors";
    static const std::string featured    = "landing-featured";
    static const std::string duplicates  = "landing-duplicates";

    auto most_recent_it = map.find( most_recent );
    auto top_tags_it    = map.find( top_tags );
    auto top_authors_it = map.find( top_authors );
    auto featured_it    = map.find( featured );
    auto duplicates_it    = map.find( duplicates );

    if( most_recent_it != map.end() ) {
        try {
            options._landing_page.most_recent = std::stoi( most_recent_it->second.value );
            most_recent_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + most_recent + "' value to integer "
                "(line #" + std::to_string( most_recent_it->second.line ) + "): " + most_recent_it->second.value
            );
        }
    }

    if( top_authors_it != map.end() ) {
        try {
            options._landing_page.top_authors = std::stoi( top_authors_it->second.value );
            top_authors_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + top_authors + "' value to integer "
                "(line #" + std::to_string( top_authors_it->second.line ) + "): " + top_authors_it->second.value
            );
        }
    }

    if( top_tags_it != map.end() ) {
        try {
            options._landing_page.top_tags = std::stoi( top_tags_it->second.value );
            top_tags_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + top_tags + "' value to integer "
                "(line #" + std::to_string( top_tags_it->second.line ) + "): " + top_tags_it->second.value
            );
        }
    }

    if( featured_it != map.end() ) {
        auto it = std::sregex_iterator( featured_it->second.value.begin(),
                                        featured_it->second.value.end(),
                                        QUOTED_STR_RX );

        size_t i = 0;
        while( it != std::sregex_iterator() ) {
            if( it->size() > 1 && std::regex_search( it->str( 1 ), HTML_FILENAME_RX ) )
                options._landing_page.featured.emplace( std::make_pair( it->str( 1 ), i ) );
            else
                throw exception::file_parsing_failure(
                    "Malformed file name in '" + featured + "' array (line #" +
                    std::to_string( featured_it->second.line ) + "): " + it->str()
                );

            ++i;
            ++it;
        }

        featured_it->second.validated = true;
    }

    if( duplicates_it != map.end() ) {
        if( duplicates_it->second.type == Type::BOOLEAN ) {
            options._landing_page.duplicates = ( duplicates_it->second.value == "true" );
            duplicates_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + duplicates + "' value to boolean "
                "(line #" + std::to_string( duplicates_it->second.line ) + "): " + duplicates_it->second.value
            );
        }
    }

    _display.msg( "Landing page post duplicates ....: ", options._landing_page.duplicates, "ALLOW", "DISALLOW" );
}

/**
 * Process all per-page navigation DIV options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 */
void blogator::fs::ConfigReader::processPageNavOptions( std::unordered_map<std::string, ConfigReader::Value> & map,
                                                        dto::Options & options ) const
{
    static const std::string separator = "page-nav-separator";
    static const std::string forward   = "page-nav-forward";
    static const std::string backwards = "page-nav-backwards";
    static const std::string first     = "page-nav-first";
    static const std::string last      = "page-nav-last";

    auto separator_it = map.find( separator );
    auto forward_it   = map.find( forward );
    auto backwards_it = map.find( backwards );
    auto first_it     = map.find( first );
    auto last_it      = map.find( last );

    if( separator_it != map.end() && !separator_it->second.value.empty() ) {
        options._page_nav.separator = separator_it->second.value;
        separator_it->second.validated = true;
    }

    if( forward_it != map.end() && !forward_it->second.value.empty() ) {
        options._page_nav.forward = forward_it->second.value;
        forward_it->second.validated = true;
    }

    if( backwards_it != map.end() && !backwards_it->second.value.empty() ) {
        options._page_nav.backward = backwards_it->second.value;
        backwards_it->second.validated = true;
    }

    if( first_it != map.end() && !first_it->second.value.empty() ) {
        options._page_nav.first = first_it->second.value;
        first_it->second.validated = true;
    }

    if( last_it != map.end() && !last_it->second.value.empty() ) {
        options._page_nav.last = last_it->second.value;
        last_it->second.validated = true;
    }
}

/**
 * Process all Breadcrumb DIV specific options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 */
void blogator::fs::ConfigReader::processBreadcrumbOptions( std::unordered_map<std::string, ConfigReader::Value> & map,
                                                           dto::Options & options ) const
{
    static const std::string landing_page   = "breadcrumb-landing-page";
    static const std::string by_date_page   = "breadcrumb-by-date-page";
    static const std::string by_year_page   = "breadcrumb-by-year-page";
    static const std::string by_tag_page    = "breadcrumb-by-tag-page";
    static const std::string by_author_page = "breadcrumb-by-author-page";
    static const std::string index_page     = "breadcrumb-index-page";

    auto landing_page_it   = map.find( landing_page );
    auto by_date_page_it   = map.find( by_date_page );
    auto by_year_page_it   = map.find( by_year_page );
    auto by_tag_page_it    = map.find( by_tag_page );
    auto by_author_page_it = map.find( by_author_page );
    auto index_page_it     = map.find( index_page );

    if( landing_page_it != map.end() && !landing_page_it->second.value.empty() ) {
        options._breadcrumb.start = landing_page_it->second.value;
        landing_page_it->second.validated = true;
    }

    if( by_date_page_it != map.end() && !by_date_page_it->second.value.empty() ) {
        options._breadcrumb.by_date = by_date_page_it->second.value;
        by_date_page_it->second.validated = true;
    }

    if( by_year_page_it != map.end() && !by_year_page_it->second.value.empty() ) {
        options._breadcrumb.by_year = by_year_page_it->second.value;
        by_year_page_it->second.validated = true;
    }

    if( by_tag_page_it != map.end() && !by_tag_page_it->second.value.empty() ) {
        options._breadcrumb.by_tag = by_tag_page_it->second.value;
        by_tag_page_it->second.validated = true;
    }

    if( by_author_page_it != map.end() && !by_author_page_it->second.value.empty() ) {
        options._breadcrumb.by_author = by_author_page_it->second.value;
        by_author_page_it->second.validated = true;
    }

    if( index_page_it != map.end() && !index_page_it->second.value.empty() ) {
        options._breadcrumb.page = index_page_it->second.value;
        index_page_it->second.validated = true;
    }
}

/**
 * Process all RSS specific options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 * @throws exception::file_parsing_failure when parsing failed
 * @throws exception::failed_expectation when a required option is not found
 */
void blogator::fs::ConfigReader::processRssOptions( std::unordered_map<std::string, ConfigReader::Value> & map,
                                                    dto::Options & options ) const
{
    static const std::string generate    = "rss";
    static const std::string item_count  = "rss-item-count";
    static const std::string site_url    = "site-url";
    static const std::string title       = "rss-title";
    static const std::string description = "rss-description";
    static const std::string copyright   = "rss-copyright";
    static const std::string img_url     = "rss-img-url";
    static const std::string img_link    = "rss-img-link";
    static const std::string img_width   = "rss-img-width";
    static const std::string img_height  = "rss-img-height";
    static const std::string img_alt     = "rss-img-alt";

    auto generate_it    = map.find( generate );
    auto item_count_it  = map.find( item_count );
    auto site_url_it    = map.find( site_url );
    auto title_it       = map.find( title );
    auto description_it = map.find( description );
    auto copyright_it   = map.find( copyright );
    auto img_url_it     = map.find( img_url );
    auto img_link_it    = map.find( img_link );
    auto img_width_it   = map.find( img_width );
    auto img_height_it  = map.find( img_height );
    auto img_alt_it     = map.find( img_alt );

    if( generate_it != map.end() ) {
        if( generate_it->second.type == Type::BOOLEAN ) {
            options._rss.generate = ( generate_it->second.value == "true" );
            generate_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + generate + "' value to boolean "
                "(line #" + std::to_string( generate_it->second.line ) + "): " + generate_it->second.value
            );
        }
    }

    if( item_count_it != map.end() ) {
        try {
            options._rss.item_count = std::stoi( item_count_it->second.value );
            item_count_it->second.validated = true;
        } catch( std::exception &e ) {
            throw exception::file_parsing_failure(
                "Error converting '" + item_count + "' value to integer "
                "(line #" + std::to_string( item_count_it->second.line ) + "): " + item_count_it->second.value
            );
        }
    }

    if( site_url_it != map.end() ) {
        if( options._rss.generate && site_url_it->second.value.empty() )
            throw exception::failed_expectation(
                "The target site's url '" + site_url + "' is required to create an RSS feed."
            );

        if( std::regex_search( site_url_it->second.value, WEB_URL_RX ) ) {
            options._rss.root_url = site_url_it->second.value;
            site_url_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Malformed URL in '" + site_url + "' value "
                "(line #" + std::to_string( site_url_it->second.line ) + "): " + site_url_it->second.value
            );
        }
    }

    if( title_it != map.end() && !title_it->second.value.empty() ) {
        options._rss.title = title_it->second.value;
        title_it->second.validated = true;
    }

    if( description_it != map.end() && !description_it->second.value.empty() ) {
        options._rss.title = title_it->second.value;
        title_it->second.validated = true;
    }

    if( copyright_it != map.end() && !copyright_it->second.value.empty() ) {
        options._rss.copyright = copyright_it->second.value;
        copyright_it->second.validated = true;
    }

    if( img_url_it != map.end() ) {
        if( std::regex_search( img_url_it->second.value, FILE_PATH ) ) {
            options._rss.img_url = img_url_it->second.value;
            site_url_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Expected relative file path to site root is malformed in '" + img_url + "' value "
                "(line #" + std::to_string( img_url_it->second.line ) + "): " + img_url_it->second.value
            );
        }
    }

    if( img_link_it != map.end() ) {
        if( std::regex_search( img_link_it->second.value, WEB_URL_RX ) ) {
            options._rss.img_link = img_link_it->second.value;
            site_url_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Malformed URL in '" + img_link + "' value "
                "(line #" + std::to_string( img_link_it->second.line ) + "): " + img_link_it->second.value
            );
        }
    }

    if( img_alt_it != map.end() ) {
        options._rss.img_alt = img_alt_it->second.value;
        img_alt_it->second.validated = true;
    }

    if( img_width_it != map.end() ) {
        if( std::regex_search( img_width_it->second.value, CSS_DIMENSION_RX ) ) {
            options._rss.img_width = img_width_it->second.value;
            img_width_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Malformed CSS dimension in '" + img_width + "' value "
                "(line #" + std::to_string( img_width_it->second.line ) + "): " + img_width_it->second.value
            );
        }
    }

    if( img_height_it != map.end() ) {
        if( std::regex_search( img_height_it->second.value, CSS_DIMENSION_RX ) ) {
            options._rss.img_height = img_height_it->second.value;
            img_height_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Malformed CSS dimension in '" + img_height + "' value "
                "(line #" + std::to_string( img_height_it->second.line ) + "): " + img_height_it->second.value
            );
        }
    }

    _display.msg( "Generate RSS feed ...............: ", options._rss.generate, "TRUE", "FALSE" );
}

/**
 * Process all JSON related options
 * @param map     Raw {K,V} map of options parsed from configuration file
 * @param options Options DTO
 */
void blogator::fs::ConfigReader::processJsonOptions( std::unordered_map<std::string, Value> &map,
                                                     dto::Options &options) const
{
    static const std::string generate_index = "json-index";
    static const std::string append_paths   = "json-index-append";

    auto generate_index_it = map.find( generate_index );
    auto index_append_it   = map.find( append_paths );

    if( generate_index_it != map.end() ) {
        if( generate_index_it->second.type == Type::BOOLEAN ) {
            options._json_index.generate = ( generate_index_it->second.value == "true" );
            generate_index_it->second.validated = true;
        } else {
            throw exception::file_parsing_failure(
                "Error converting '" + generate_index + "' value to boolean "
                "(line #" + std::to_string( generate_index_it->second.line ) + "): " + generate_index_it->second.value
            );
        }
    }

    if( index_append_it != map.end() ) {
        auto it = std::sregex_iterator( index_append_it->second.value.begin(),
                                        index_append_it->second.value.end(),
                                        QUOTED_STR_RX );

        size_t i = 0;
        while( it != std::sregex_iterator() ) {
            if( it->size() > 1 && std::regex_search( it->str( 1 ), JSON_FILENAME_RX ) )
                options._json_index.append_paths.emplace_back( it->str( 1 ) );
            else
                throw exception::file_parsing_failure(
                    "Malformed file name in '" + append_paths + "' array (line #" +
                    std::to_string( index_append_it->second.line ) + "): " + it->str()
                );

            ++i;
            ++it;
        }

        index_append_it->second.validated = true;
    }
}