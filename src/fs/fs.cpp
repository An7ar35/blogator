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
 *
 * @param file_path
 * @return
 */
std::shared_ptr<blogator::dto::Options> blogator::fs::importOptions( const std::filesystem::path &file_path ) { //TODO error control?
    auto reader = ConfigReader();
    return reader.init( file_path );
}

/**
 * Sets-up the base directories for generating the posts/index
 * @param global_options Global blogator options
 * @throws std::runtime_error when a filesystem call has thrown an exception
 */
void blogator::fs::setupEnvironment( const std::shared_ptr<dto::Options> &global_options ) {
    try {
        std::uintmax_t purge_count = 0;
        purge_count += std::filesystem::remove_all( global_options->_paths.posts_dir );
        purge_count += std::filesystem::remove_all( global_options->_paths.index_dir );
        purge_count += std::filesystem::remove( global_options->_paths.root_dir / global_options->_rss.file_name ) ? 1 : 0;

        std::cout << " (" << purge_count << " file(s)/folder(s) purged)" << std::endl;

        if( !std::filesystem::exists( global_options->_paths.template_dir ) )
            std::filesystem::create_directories( global_options->_paths.template_dir ); //TODO generate missing templates if none are found

        if( !std::filesystem::create_directories( global_options->_paths.posts_dir ) )
            std::cerr << "Could not create directories: " << global_options->_paths.posts_dir.string() << std::endl;
        if( !std::filesystem::create_directories( global_options->_paths.index_dir ) )
            std::cerr << "Could not create directories: " << global_options->_paths.index_dir.string() << std::endl;

        if( !std::filesystem::create_directories( global_options->_paths.index_date_dir ) )
            std::cerr << "Could not create index sub-directory: " << global_options->_paths.index_date_dir.string() << std::endl;
        if( global_options->_index.index_by_year )
            if( !std::filesystem::create_directories( global_options->_paths.index_year_dir ) )
                std::cerr << "Could not create index sub-directory: " << global_options->_paths.index_year_dir.string() << std::endl;
        if( global_options->_index.index_by_tag )
            if( !std::filesystem::create_directories( global_options->_paths.index_tag_dir ) )
                std::cerr << "Could not create index sub-directory: " << global_options->_paths.index_tag_dir.string() << std::endl;
        if( global_options->_index.index_by_author )
            if( !std::filesystem::create_directories( global_options->_paths.index_author_dir ) )
                std::cerr << "Could not create index sub-directory: " << global_options->_paths.index_tag_dir.string() << std::endl;

    } catch( std::exception &e ) {
        std::stringstream ss;
        ss << "Environment setup failed: " << e.what();
        throw std::runtime_error( ss.str() );
    }
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

    display.begin( "Loading templates", 9, "landing page" );
    templates._landing->src        = master_index._paths.templates.landing;
    templates._landing->html       = fs::importHTML( templates._landing->src );
    display.progress( "landing page entry" );
    templates._landing_entry->src  = master_index._paths.templates.landing_entry;
    templates._landing_entry->html = fs::importHTML( templates._landing_entry->src );
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
        display.log( "Custom index year list template was found" );
    } else {
        templates._year_list = templates._index_list;
    }

    display.progress( "tag list page" );
    if( !master_index._paths.templates.tag_list.empty() ) {
        templates._tag_list       = std::make_shared<dto::Template>( dto::Template::Type::INDEX_LIST );
        templates._tag_list->src  = master_index._paths.templates.tag_list;
        templates._tag_list->html = fs::importHTML( templates._tag_list->src );
        display.log( "Custom index tag list template was found" );
    } else {
        templates._tag_list = templates._index_list;
    }

    display.progress( "author list page" );
    if( !master_index._paths.templates.author_list.empty() ) {
        templates._author_list       = std::make_shared<dto::Template>( dto::Template::Type::INDEX_LIST );
        templates._author_list->src  = master_index._paths.templates.author_list;
        templates._author_list->html = fs::importHTML( templates._author_list->src );
        display.log( "Custom index author list template was found" );
    } else {
        templates._author_list = templates._index_list;
    }

    display.progress( "index page entry" );
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

    display.begin( "Extracting template rel. paths", 9, "landing page" );
    templates._landing->path_write_pos = dto::Templates::extractRelativePaths( *templates._landing->html );
    total += templates._landing->path_write_pos.size();

    display.progress( "landing page entry" );
    templates._landing_entry->path_write_pos = dto::Templates::extractRelativePaths( *templates._landing_entry->html );
    total += templates._landing_entry->path_write_pos.size();

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

    display.progress( "tag list page" );
    if( !master_index._paths.templates.tag_list.empty() ) {
        templates._tag_list->path_write_pos = dto::Templates::extractRelativePaths( *templates._tag_list->html );
        total += templates._tag_list->path_write_pos.size();
    }

    display.progress( "author list page" );
    if( !master_index._paths.templates.author_list.empty() ) {
        templates._author_list->path_write_pos = dto::Templates::extractRelativePaths( *templates._author_list->html );
        total += templates._author_list->path_write_pos.size();
    }

    display.progress( "index page entry" );
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

    display.begin( "Locating insertion points", 9, "landing page" );
    templates._landing->block_write_pos = getConsecutiveWritePositions( *templates._landing->html, templates._landing->block_classes );

    display.progress( "landing page entry" );
    templates._landing_entry->block_write_pos = getConsecutiveWritePositions( *templates._landing_entry->html, templates._landing_entry->block_classes );

    display.progress( "post page" );
    templates._post->block_write_pos = getConsecutiveWritePositions( *templates._post->html, templates._post->block_classes );

    display.progress( "index page" );
    templates._index->block_write_pos = getConsecutiveWritePositions( *templates._index->html, templates._index->block_classes );

    display.progress( "index list page" );
    templates._index_list->block_write_pos = getConsecutiveWritePositions( *templates._index_list->html, templates._index_list->block_classes );

    display.progress( "date list page" );
    if( !master_index._paths.templates.year_list.empty() )
        templates._year_list->block_write_pos = getConsecutiveWritePositions( *templates._year_list->html, templates._year_list->block_classes );

    display.progress( "tag list page" );
    if( !master_index._paths.templates.tag_list.empty() )
        templates._tag_list->block_write_pos = getConsecutiveWritePositions( *templates._tag_list->html, templates._tag_list->block_classes );

    display.progress( "author list page" );
    if( !master_index._paths.templates.author_list.empty() )
        templates._author_list->block_write_pos = getConsecutiveWritePositions( *templates._author_list->html, templates._author_list->block_classes );

    display.progress( "index page entry" );
    templates._index_entry->block_write_pos = getConsecutiveWritePositions( *templates._index_entry->html, templates._index_entry->block_classes );

    display.progress( "DONE" );
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
        std::cerr << "[fs::adaptRelPath(..)] " << e.what() << std::endl; //TODO?
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

        display.begin( "Validating template rel. paths", 9, "landing page" );
        valid_count += checkTemplateRelPaths( *templates._landing );
        total_count += templates._landing->path_write_pos.size();

        display.progress( "landing page entry" );
        valid_count += checkTemplateRelPaths( *templates._landing_entry );
        total_count += templates._landing_entry->path_write_pos.size();

        display.progress( "post page" );
        valid_count += checkTemplateRelPaths( *templates._post );
        total_count += templates._post->path_write_pos.size();

        display.progress( "index page" );
        valid_count += checkTemplateRelPaths( *templates._index );
        total_count += templates._index->path_write_pos.size();

        display.progress( "index list page" );
        valid_count += checkTemplateRelPaths( *templates._index_list );
        total_count += templates._index_list->path_write_pos.size();

        display.progress( "date list page" );
        if( !master_index._paths.templates.year_list.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._year_list );
            total_count += templates._year_list->path_write_pos.size();
        }

        display.progress( "tag list page" );
        if( !master_index._paths.templates.tag_list.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._tag_list );
            total_count += templates._tag_list->path_write_pos.size();
        }

        display.progress( "author list page" );
        if( !master_index._paths.templates.author_list.empty() ) {
            valid_count += checkTemplateRelPaths( *templates._author_list );
            total_count += templates._author_list->path_write_pos.size();
        }

        display.progress( "index page entry" );
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