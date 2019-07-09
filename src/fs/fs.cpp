#include "fs.h"
#include <iostream>
#include <fstream>
#include <regex>

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
 * @param options Option DTO (with the paths to the html template files)
 * @return Template DTO loaded and analysed for insertion points
 */
std::shared_ptr<blogator::dto::Template> blogator::fs::importTemplates( const dto::Index &master_index ) {
    auto &display  = cli::MsgInterface::getInstance();
    auto templates = std::make_shared<dto::Template>();

    display.begin( "Loading templates", 7, "landing page" );
    templates->_start.html       = fs::importHTML( master_index._paths.templates.landing );
    display.progress( "landing page entry" );
    templates->_start_entry.html = fs::importHTML( master_index._paths.templates.landing_entry );
    display.progress( "post page" );
    templates->_post.html        = fs::importHTML( master_index._paths.templates.post );
    display.progress( "index page" );
    templates->_index.html       = fs::importHTML( master_index._paths.templates.index );
    display.progress( "tag list page" );
    templates->_tag_list.html    = fs::importHTML( master_index._paths.templates.tag_list );
    display.progress( "author list page" );
    templates->_author_list.html = fs::importHTML( master_index._paths.templates.author_list );
    display.progress( "index page entry" );
    templates->_index_entry.html = fs::importHTML( master_index._paths.templates.index_entry );
    display.progress( "DONE" );

    display.begin( "Locating insertion points", 7, "landing page" );
    html::reader::findInsertPositions( *templates->_start.html, templates->_start.div_write_pos );
    display.progress( "landing page entry" );
    html::reader::findInsertPositions( *templates->_start_entry.html, templates->_start_entry.div_write_pos );
    display.progress( "post page" );
    html::reader::findInsertPositions( *templates->_post.html, templates->_post.div_write_pos );
    display.progress( "index page" );
    html::reader::findInsertPositions( *templates->_index.html, templates->_index.div_write_pos );
    display.progress( "tag list page" );
    html::reader::findInsertPositions( *templates->_tag_list.html, templates->_tag_list.div_write_pos );
    display.progress( "author list page" );
    html::reader::findInsertPositions( *templates->_author_list.html, templates->_author_list.div_write_pos );
    display.progress( "index page entry" );
    html::reader::findInsertPositions( *templates->_index_entry.html, templates->_index_entry.div_write_pos );
    display.progress( "DONE" );

    return std::move( templates );
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