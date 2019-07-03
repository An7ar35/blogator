#include "fs.h"
#include <iostream>
#include <fstream>
#include <regex>

#include "ConfigReader.h"
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
 * @return Success
 */
bool blogator::fs::setupEnvironment( const std::shared_ptr<dto::Options> &global_options ) {
    try {
        std::uintmax_t purge_count = 0;
        purge_count += std::filesystem::remove_all( global_options->_paths.posts_dir );
        purge_count += std::filesystem::remove_all( global_options->_paths.index_dir );
        purge_count += std::filesystem::remove( global_options->_paths.root_dir / global_options->_rss.file_name ) ? 1 : 0;

        std::cout << "Purged " << purge_count << " file(s)/folder(s)" << std::endl;

        if( !std::filesystem::create_directories( global_options->_paths.posts_dir ) )
            std::cerr << "Could not create directories: " << global_options->_paths.posts_dir.string() << std::endl;
        if( !std::filesystem::create_directories( global_options->_paths.index_dir ) )
            std::cerr << "Could not create directories: " << global_options->_paths.index_dir.string() << std::endl;

        if( !std::filesystem::create_directories( global_options->_paths.index_dir / global_options->_paths.index_sub_dirs.by_date ) )
            std::cerr << "Could not create index sub-directory: " << global_options->_paths.index_sub_dirs.by_date.string() << std::endl;
        if( !std::filesystem::create_directories( global_options->_paths.index_dir / global_options->_paths.index_sub_dirs.by_tag ) )
            std::cerr << "Could not create index sub-directory: " << global_options->_paths.index_sub_dirs.by_tag.string() << std::endl;

        return true;
    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

/**
 * Imports the HTML templates to use
 * @param options Option DTO (with the paths to the html template files)
 * @return Template DTO loaded and analysed for insertion points
 */
std::shared_ptr<blogator::dto::Template> blogator::fs::importTemplates( const dto::Index &master_index ) {
    auto templates = std::make_shared<dto::Template>();

    templates->_start.html       = fs::importHTML( master_index._paths.templates.start );
    templates->_post.html        = fs::importHTML( master_index._paths.templates.post );
    templates->_index.html       = fs::importHTML( master_index._paths.templates.index );
    templates->_tag_list.html    = fs::importHTML( master_index._paths.templates.tag_list );
    templates->_index_entry.html = fs::importHTML( master_index._paths.templates.index_entry );

    html::reader::findInsertPositions( *templates->_start.html, templates->_start.div_write_pos );
    html::reader::findInsertPositions( *templates->_post.html, templates->_post.div_write_pos );
    html::reader::findInsertPositions( *templates->_index.html, templates->_index.div_write_pos );
    html::reader::findInsertPositions( *templates->_tag_list.html, templates->_tag_list.div_write_pos );
    html::reader::findInsertPositions( *templates->_index_entry.html, templates->_index_entry.div_write_pos );

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