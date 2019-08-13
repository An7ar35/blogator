#include "fs.h"
#include <iostream>
#include <fstream>
#include <regex>

#include <eadlib/cli/colour.h>

#include "ConfigReader.h"
#include "../cli/MsgInterface.h"
#include "../html/reader/reader.h"
#include "../exception/file_access_failure.h"

/**
 * Imports configuration options from config file to dto::Options object
 * @param file_path Configuration file path
 * @param info      Software information
 * @return Options DTO
 */
std::shared_ptr<blogator::dto::Options> blogator::fs::importOptions( const std::filesystem::path &file_path,
                                                                     const dto::BlogatorInfo &info ) {
    auto reader = ConfigReader();
    return reader.init( file_path, info );
}

/**
 * Sets-up the base directories for generating the posts/index
 * @param global_options Global blogator options
 * @throws std::runtime_error when a filesystem call has thrown an exception
 */
void blogator::fs::setupEnvironment( const std::shared_ptr<dto::Options> &global_options ) {
    auto &display = cli::MsgInterface::getInstance();

    try {
        auto purged_count = purge( *global_options );
        std::cout << " (" << purged_count << " file(s)/folder(s) purged)" << std::endl;

        if( !std::filesystem::exists( global_options->_paths.template_dir ) ) {
            std::filesystem::create_directories( global_options->_paths.template_dir );
        }
        if( !std::filesystem::exists( global_options->_paths.posts_dir ) ) {
            if( !std::filesystem::create_directories( global_options->_paths.posts_dir ) ) {
                display.error( "Could not create directories: " + global_options->_paths.posts_dir.string() );
            }
        }
        if( !std::filesystem::create_directories( global_options->_paths.index_dir ) ) {
            display.error( "Could not create directories: " + global_options->_paths.index_dir.string() );
        }
        if( !std::filesystem::create_directories( global_options->_paths.index_date_dir ) ) {
            display.error( "Could not create index sub-directory: " + global_options->_paths.index_date_dir.string() );
        }
        if( global_options->_index.index_by_year ) {
            if( !std::filesystem::create_directories( global_options->_paths.index_year_dir ) )
                display.error( "Could not create index sub-directory: " + global_options->_paths.index_year_dir.string() );
        }
        if( global_options->_index.index_by_tag ) {
            if( !std::filesystem::create_directories( global_options->_paths.index_tag_dir ) )
                display.error( "Could not create index sub-directory: " + global_options->_paths.index_tag_dir.string() );
        }
        if( global_options->_index.index_by_author ) {
            if( !std::filesystem::create_directories( global_options->_paths.index_author_dir ) )
                display.error( "Could not create index sub-directory: " + global_options->_paths.index_tag_dir.string() );
        }
    } catch( std::exception &e ) {
        std::stringstream ss;
        ss << "Environment setup failed: " << e.what();
        throw std::runtime_error( ss.str() );
    }
}

/**
 * Purges all generated folders
 * @param options Global Options DTO
 * @return Number of file/folders purged
 */
uintmax_t blogator::fs::purge( const blogator::dto::Options &options ) {
    auto &display = cli::MsgInterface::getInstance();
    std::uintmax_t purge_count = 0;

    purge_count += std::filesystem::remove_all( options._paths.index_dir );
    purge_count += std::filesystem::remove( options._paths.root_dir / options._rss.file_name ) ? 1 : 0;

    if( options._posts.safe_purge && std::filesystem::exists( options._paths.posts_dir ) ) {
        auto file_list = std::list<std::filesystem::path>();
        for( auto &p: std::filesystem::recursive_directory_iterator( options._paths.posts_dir ) ) {
            if( p.is_regular_file() ) {
                if( p.path().extension().string() == ".html" ||
                    p.path().extension().string() == ".htm"  ||
                    p.path().extension().string() == ".css" )
                {
                    file_list.emplace_back( p.path() );
                }
            }
        }

        for( const auto &p : file_list )
            purge_count += std::filesystem::remove( p ) ? 1 : 0;

    } else {
        purge_count += std::filesystem::remove_all( options._paths.posts_dir );
    }

    return purge_count;
}

/**
 * Imports the HTML templates to use
 * @param master_index Master Index DTO (with the paths to the html template files)
 * @param options      Option DTO
 * @return Template DTO loaded and analysed for insertion points
 */
std::shared_ptr<blogator::dto::Templates> blogator::fs::importTemplates( const dto::Index &master_index,
                                                                         const dto::Options &options )
{
    auto &display  = cli::MsgInterface::getInstance();
    auto templates = std::make_shared<dto::Templates>();

    importTemplateHTML( master_index, *templates );

    if( options._templates.adapt_rel_paths )
        extractTemplateRelPaths( master_index, *templates );

    getConsecutiveWritePositions( master_index, *templates );

    return std::move( templates );
}

/**
 * Import HTML for all the templates
 * @param master_index Master Index DTO
 * @param templates    Templates DTO
 */
void blogator::fs::importTemplateHTML( const dto::Index &master_index, dto::Templates &templates ) {
    auto &display  = cli::MsgInterface::getInstance();

    display.begin( "Loading templates", 11, "landing page" );
    templates._landing->src        = master_index._paths.templates.landing;
    templates._landing->html       = fs::importHTML( templates._landing->src );
    display.progress( "post page" );
    templates._post->src           = master_index._paths.templates.post;
    templates._post->html          = fs::importHTML( templates._post->src );
    display.progress( "index page" );
    templates._index->src          = master_index._paths.templates.index;
    templates._index->html         = fs::importHTML( templates._index->src );
    display.progress( "index list page" );
    templates._index_list->src     = master_index._paths.templates.index_list;
    templates._index_list->html    = fs::importHTML( templates._index_list->src );

    display.progress( "year list page" );
    if( !master_index._paths.templates.year_list.empty() ) {
        templates._year_list       = std::make_shared<dto::Template>( dto::Template::Type::INDEX_LIST );
        templates._year_list->src  = master_index._paths.templates.year_list;
        templates._year_list->html = fs::importHTML( templates._year_list->src );
        display.debug( "Custom index year-list template was found" );
    } else {
        templates._year_list = templates._index_list;
    }

    display.progress( "year index page" );
    if( !master_index._paths.templates.year_index.empty() ) {
        templates._year_index       = std::make_shared<dto::Template>( dto::Template::Type::INDEX );
        templates._year_index->src  = master_index._paths.templates.year_index;
        templates._year_index->html = fs::importHTML( templates._year_index->src );
        display.debug( "Custom index year-index template was found" );
    } else {
        templates._year_index = templates._index;
    }

    display.progress( "tag list page" );
    if( !master_index._paths.templates.tag_list.empty() ) {
        templates._tag_list       = std::make_shared<dto::Template>( dto::Template::Type::INDEX_LIST );
        templates._tag_list->src  = master_index._paths.templates.tag_list;
        templates._tag_list->html = fs::importHTML( templates._tag_list->src );
        display.debug( "Custom index tag-list template was found" );
    } else {
        templates._tag_list = templates._index_list;
    }

    display.progress( "tag index page" );
    if( !master_index._paths.templates.tag_index.empty() ) {
        templates._tag_index       = std::make_shared<dto::Template>( dto::Template::Type::INDEX );
        templates._tag_index->src  = master_index._paths.templates.tag_index;
        templates._tag_index->html = fs::importHTML( templates._tag_index->src );
        display.debug( "Custom index tag-index template was found" );
    } else {
        templates._tag_index = templates._index;
    }

    display.progress( "author list page" );
    if( !master_index._paths.templates.author_list.empty() ) {
        templates._author_list       = std::make_shared<dto::Template>( dto::Template::Type::INDEX_LIST );
        templates._author_list->src  = master_index._paths.templates.author_list;
        templates._author_list->html = fs::importHTML( templates._author_list->src );
        display.debug( "Custom index author-list template was found" );
    } else {
        templates._author_list = templates._index_list;
    }

    display.progress( "author index page" );
    if( !master_index._paths.templates.author_index.empty() ) {
        templates._author_index       = std::make_shared<dto::Template>( dto::Template::Type::INDEX );
        templates._author_index->src  = master_index._paths.templates.author_index;
        templates._author_index->html = fs::importHTML( templates._author_index->src );
        display.debug( "Custom index author-index template was found" );
    } else {
        templates._author_index = templates._index;
    }

    display.progress( "index entry" );
    templates._index_entry->src    = master_index._paths.templates.index_entry;
    templates._index_entry->html   = fs::importHTML( templates._index_entry->src );
    display.progress( "DONE" );
}

/**
 * Extracts any relative paths in the Templates' HTML
 * @param master_index Master Index DTO
 * @param templates    Templates DTO
 */
void blogator::fs::extractTemplateRelPaths( const dto::Index &master_index, dto::Templates &templates ) {
    auto &display = cli::MsgInterface::getInstance();
    size_t total  = 0;

    display.begin( "Extracting template rel. paths", 11, "landing page" );
    templates._landing->path_write_pos = dto::Templates::extractRelativePaths( *templates._landing->html );
    total += templates._landing->path_write_pos.size();

    display.progress( "post page" );
    templates._post->path_write_pos = dto::Templates::extractRelativePaths( *templates._post->html );
    total += templates._post->path_write_pos.size();

    display.progress( "index page" );
    templates._index->path_write_pos = dto::Templates::extractRelativePaths( *templates._index->html );
    total += templates._index->path_write_pos.size();

    display.progress( "index list page" );
    templates._index_list->path_write_pos = dto::Templates::extractRelativePaths( *templates._index_list->html );
    total += templates._index_list->path_write_pos.size();

    display.progress( "year list page" );
    if( !master_index._paths.templates.year_list.empty() ) {
        templates._year_list->path_write_pos = dto::Templates::extractRelativePaths( *templates._year_list->html );
        total += templates._year_list->path_write_pos.size();
    }

    display.progress( "year index page" );
    if( !master_index._paths.templates.year_index.empty() ) {
        templates._year_index->path_write_pos = dto::Templates::extractRelativePaths( *templates._year_index->html );
        total += templates._year_index->path_write_pos.size();
    }

    display.progress( "tag list page" );
    if( !master_index._paths.templates.tag_list.empty() ) {
        templates._tag_list->path_write_pos = dto::Templates::extractRelativePaths( *templates._tag_list->html );
        total += templates._tag_list->path_write_pos.size();
    }

    display.progress( "tag index page" );
    if( !master_index._paths.templates.tag_index.empty() ) {
        templates._tag_index->path_write_pos = dto::Templates::extractRelativePaths( *templates._tag_index->html );
        total += templates._tag_index->path_write_pos.size();
    }

    display.progress( "author list page" );
    if( !master_index._paths.templates.author_list.empty() ) {
        templates._author_list->path_write_pos = dto::Templates::extractRelativePaths( *templates._author_list->html );
        total += templates._author_list->path_write_pos.size();
    }

    display.progress( "author index page" );
    if( !master_index._paths.templates.author_index.empty() ) {
        templates._author_index->path_write_pos = dto::Templates::extractRelativePaths( *templates._author_index->html );
        total += templates._author_index->path_write_pos.size();
    }

    display.progress( "index entry" );
    templates._index_entry->path_write_pos   = dto::Templates::extractRelativePaths( *templates._index_entry->html );
    total += templates._index_entry->path_write_pos.size();

    display.progress( "DONE (" + std::to_string( total ) + " found)" );
}

/**
 * Gets all write positions for all the Templates
 * @param master_index Master Index DTO
 * @param templates    Templates DTO
 */
void blogator::fs::getConsecutiveWritePositions( const dto::Index &master_index, dto::Templates &templates ) {
    using html::reader::getConsecutiveWritePositions;
    auto &display = cli::MsgInterface::getInstance();

    display.begin( "Locating insertion points", 11, "landing page" );
    templates._landing->block_write_pos = getConsecutiveWritePositions( *templates._landing->html, templates._landing->block_classes );

    display.progress( "post page" );
    templates._post->block_write_pos = getConsecutiveWritePositions( *templates._post->html, templates._post->block_classes );

    display.progress( "index page" );
    templates._index->block_write_pos = getConsecutiveWritePositions( *templates._index->html, templates._index->block_classes );

    display.progress( "index list page" );
    templates._index_list->block_write_pos = getConsecutiveWritePositions( *templates._index_list->html, templates._index_list->block_classes );

    display.progress( "year list page" );
    if( !master_index._paths.templates.year_list.empty() )
        templates._year_list->block_write_pos = getConsecutiveWritePositions( *templates._year_list->html, templates._year_list->block_classes );

    display.progress( "year index page" );
    if( !master_index._paths.templates.year_index.empty() )
        templates._year_index->block_write_pos = getConsecutiveWritePositions( *templates._year_index->html, templates._year_index->block_classes );

    display.progress( "tag list page" );
    if( !master_index._paths.templates.tag_list.empty() )
        templates._tag_list->block_write_pos = getConsecutiveWritePositions( *templates._tag_list->html, templates._tag_list->block_classes );

    display.progress( "tag index page" );
    if( !master_index._paths.templates.tag_index.empty() )
        templates._tag_index->block_write_pos = getConsecutiveWritePositions( *templates._tag_index->html, templates._tag_index->block_classes );

    display.progress( "author list page" );
    if( !master_index._paths.templates.author_list.empty() )
        templates._author_list->block_write_pos = getConsecutiveWritePositions( *templates._author_list->html, templates._author_list->block_classes );

    display.progress( "author index page" );
    if( !master_index._paths.templates.author_index.empty() )
        templates._author_index->block_write_pos = getConsecutiveWritePositions( *templates._author_index->html, templates._author_index->block_classes );

    display.progress( "index entry" );
    templates._index_entry->block_write_pos = getConsecutiveWritePositions( *templates._index_entry->html, templates._index_entry->block_classes );

    display.progress( "DONE" );
}

/**
 * Reads in the content of a file into a string
 * @param path Path of file to read
 * @return Content as a string
 * @throws exception::file_access_failure when reading failed
 */
std::string blogator::fs::readFileContent( const std::filesystem::path & path ) {
    try {
        auto file = std::ifstream( path );
        auto ss = std::stringstream();
        ss << file.rdbuf();
        return ss.str();

    } catch( std::exception & e ) {
        auto &display = cli::MsgInterface::getInstance();
        display.debug( "fs::readFileContent( " + path.string() + ")] " + e.what() + ": " + strerror( errno ) );
        throw exception::file_access_failure( "Could not fetch content of: " + path.string() );
    }
}

/**
 * Imports lines from an HTML file
 * @param file_path Path of file
 * @return HTML DTO object
 * @throws blogator::exception::file_access_failure when access to source file fails
 */
std::unique_ptr<blogator::dto::HTML> blogator::fs::importHTML( const std::filesystem::path &file_path ) {
    auto html = std::make_unique<blogator::dto::HTML>();

    std::string   line;
    std::ifstream file( file_path.string() );

    if( !file.is_open() )
        throw exception::file_access_failure( "Could not open: " + file_path.string() );

    while( getline( file, line ) ) {
        html->_lines.emplace_back( line );
    }

    file.close();

    return std::move( html );
}

/**
 * Imports lines from a HTML file
 * @param file_path Path of file
 * @param positions Range(s) of content to import
 * @return HTML DTO object
 * @throws blogator::exception::file_access_failure when access to source file fails
 */
blogator::dto::HTML blogator::fs::importHTML( const std::filesystem::path &file_path,
                                              const std::list<dto::SeekRange> &positions )
{
    auto   html          = dto::HTML();
    size_t curr_line     = 0;
    bool   stay_on_line  = false;
    auto   post          = std::ifstream( file_path );

    if( !post.is_open() )
        throw exception::file_access_failure( "Could not open: " + file_path.string() );

    std::string line;

    auto pos_it = positions.cbegin();
    while( pos_it != positions.cend() ) {
        std::stringstream ss;

        for( ; curr_line < pos_it->_from.line; ++curr_line ) {
            post.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        }

        auto tmp_curr_line = curr_line;
        while( tmp_curr_line <= pos_it->_to.line ) {
            if( !stay_on_line ) {
                getline( post, line );
            }

            if( pos_it->_from.line == pos_it->_to.line ) { //range on same line
                ss << line.substr( pos_it->_from.col, pos_it->_to.col - pos_it->_from.col );
                html._lines.emplace_back( ss.str() );
                stay_on_line = ( std::next( pos_it ) != positions.cend() && std::next( pos_it )->_from.line == tmp_curr_line );

            } else if( pos_it->_from.line == tmp_curr_line ) { //start line
                ss << line.substr( pos_it->_from.col );
                stay_on_line = false;

            } else if( pos_it->_to.line == tmp_curr_line ) { //end line
                ss << line.substr( 0, pos_it->_to.col );
                html._lines.emplace_back( ss.str() );
                stay_on_line = ( std::next( pos_it ) != positions.cend() && std::next( pos_it )->_from.line == tmp_curr_line );

            } else { //line between the range
                ss << line;
                stay_on_line = false;
            }

            ++tmp_curr_line;
        }

        if( !stay_on_line )
            curr_line = tmp_curr_line;

        ++pos_it;
    }

    return html;
}

/**
 * Adapts a relative path based on a new location
 * @param source_file Source file where the path was lifted from
 * @param target_file Target file where the path will be written to
 * @param rel_path    Relative path
 * @return Modified relative path or original if an error has occurred
 */
std::filesystem::path blogator::fs::adaptRelPath( const std::filesystem::path & source_file,
                                                  const std::filesystem::path & target_file,
                                                  const std::filesystem::path & rel_path )
{
    try {
        auto source_abs = ( source_file.parent_path() / rel_path ).lexically_normal();
        auto target_rel = source_abs.lexically_relative( target_file.parent_path() );
        return target_rel;

    } catch( std::exception &e ) {
        std::stringstream ss;
        ss << "[fs::adaptRelPath(..)] " << e.what();
        auto &display = cli::MsgInterface::getInstance();
        display.error( ss.str() );
        return rel_path;
    }
}

/**
 * Checks all relative paths found in the template files
 * @param templates    Master templates DTO
 * @param options      Option DTO
 */
void blogator::fs::checkTemplateRelPaths( const dto::Index     &master_index,
                                          const dto::Templates &templates,
                                          const dto::Options   &options )
{
    if( options._templates.adapt_rel_paths ) {
        auto & display = cli::MsgInterface::getInstance();
        size_t total_count = 0;
        size_t valid_count = 0;

        display.begin( "Validating template rel. paths", 11, "landing page" );
        valid_count += checkTemplateRelPaths( *templates._landing );
        total_count += templates._landing->path_write_pos.size();

        display.progress( "post page" );
        valid_count += checkTemplateRelPaths( *templates._post );
        total_count += templates._post->path_write_pos.size();

        display.progress( "index page" );
        valid_count += checkTemplateRelPaths( *templates._index );
        total_count += templates._index->path_write_pos.size();

        display.progress( "index list page" );
        valid_count += checkTemplateRelPaths( *templates._index_list );
        total_count += templates._index_list->path_write_pos.size();

        display.progress( "year list page" );
        if( !master_index._paths.templates.year_list.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._year_list );
            total_count += templates._year_list->path_write_pos.size();
        }

        display.progress( "year index page" );
        if( !master_index._paths.templates.year_index.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._year_index );
            total_count += templates._year_index->path_write_pos.size();
        }

        display.progress( "tag list page" );
        if( !master_index._paths.templates.tag_list.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._tag_list );
            total_count += templates._tag_list->path_write_pos.size();
        }

        display.progress( "tag index page" );
        if( !master_index._paths.templates.tag_index.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._tag_index );
            total_count += templates._tag_index->path_write_pos.size();
        }

        display.progress( "author list page" );
        if( !master_index._paths.templates.author_list.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._author_list );
            total_count += templates._author_list->path_write_pos.size();
        }

        display.progress( "author index page" );
        if( !master_index._paths.templates.author_index.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._author_index );
            total_count += templates._author_index->path_write_pos.size();
        }

        display.progress( "index entry" );
        valid_count += checkTemplateRelPaths( *templates._index_entry );
        total_count += templates._index_entry->path_write_pos.size();

        using eadlib::cli::FGColour;
        using eadlib::cli::Decoration;
        if( valid_count < total_count )
            display.progress(
                eadlib::cli::format<FGColour::YELLOW, Decoration::BOLD>(
                    std::to_string( valid_count ) + '/' + std::to_string( total_count ) + " valid" )
            );
        else
            display.progress( "VALIDATED" );
    }
}

/**
 * Checks all relative paths found in a template
 * @param src_template Template to check
 * @return Valid count
 */
size_t blogator::fs::checkTemplateRelPaths( const blogator::dto::Template & src_template )
{
    static auto &display = cli::MsgInterface::getInstance();
    size_t validated     = 0;

    for( const auto &path : src_template.path_write_pos ) {
        auto abs_path = ( src_template.src.parent_path() / path.second ).lexically_normal();
        if( !std::filesystem::exists( abs_path ) ) {
            std::stringstream ss;
            ss << "Cannot resolve relative path in template '"
               << src_template.src.filename().string()
               << "' at " << path.first << ": " << path.second.string();
            display.warning( ss.str() );
        } else {
            ++validated;
        }
    }

    return validated;
}

/**
 * Checks all relative paths found in a collection of path positions
 * @param root     Root path to site
 * @param parent   Original parent path
 * @param path_pos ConsecutivePathPositions_t collection
 * @return Valid count
 */
size_t blogator::fs::checkRelPaths( const std::filesystem::path &root,
                                    const std::filesystem::path &parent,
                                    const dto::ConsecutivePathPositions_t &path_pos )
{
    static auto &display = cli::MsgInterface::getInstance();
    static const auto html_spaces_rx = std::regex("%20");
    size_t validated     = 0;

    for( const auto &path : path_pos ) {
        auto abs_path = ( parent.parent_path() / path.second ).lexically_normal();
        auto temp     = std::regex_replace( abs_path.string(), html_spaces_rx, " ");

        if( !std::filesystem::exists( temp ) ) {
            std::stringstream ss;
            ss << "Cannot resolve relative path in '" << parent.lexically_relative( root ) << "' at "
               << path.first << ": " << path.second.string();
            display.warning( ss.str() );
        } else {
            ++validated;
        }
    }

    return validated;
}
