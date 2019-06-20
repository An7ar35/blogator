#include "fs.h"
#include <iostream>
#include <fstream>
#include <regex>

#include "exception/bad_file_source.h"

/**
 * Sets-up the base directories for generating the posts/index
 * @param global_options Global blogator options
 * @return Success
 */
bool blogator::fs::setupEnvironment( const blogator::dto::Options &global_options ) {
    try {
        std::uintmax_t purge_count = 0;
        purge_count += std::filesystem::remove_all( global_options._paths.posts_dir );
        purge_count += std::filesystem::remove_all( global_options._paths.index_dir );

        std::cout << "Purged " << purge_count << " file(s)/folder(s)" << std::endl;

        if( !std::filesystem::create_directories( global_options._paths.posts_dir ) )
            std::cerr << "Could not create directories: " << global_options._paths.posts_dir.string() << std::endl;
        if( !std::filesystem::create_directories( global_options._paths.index_dir ) )
            std::cerr << "Could not create directories: " << global_options._paths.index_dir.string() << std::endl;

        return true;
    } catch( exception::bad_file_source &e ) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

/**
 * Imports lines from an HTML file
 * @param file_path Path of file
 * @return HTML DTO object
 * @throws std::ios::failure when file could not be opened
 */
std::unique_ptr<blogator::dto::HTML> blogator::fs::importHTML( const std::filesystem::path &file_path ) {
    auto html = std::make_unique<blogator::dto::HTML>();

    std::string   line;
    std::ifstream file( file_path.string() );

    if( file.is_open() ) {
        while( getline( file, line ) ) {
            html->_lines.emplace_back( line );
        }

        file.close();

    } else {
        throw std::ios::failure( "Could not open: " + file_path.string() );
    }

    return std::move( html );
}

/**
 * Imports the strings to use for each months of the year
 * @param file_path Path for month file
 * @return Imported key-value pairs from file
 */
std::unordered_map<unsigned, std::string> blogator::fs::importMonthNames( const std::filesystem::path &file_path ) {
    /**
     * [LAMBDA] check if char is a space
     */
    static auto isSpace = []( char ch ) {
        return std::isspace<char>( ch, std::locale::classic() );
    };

    std::unordered_map<unsigned, std::string> month_map;
    std::string   line;
    std::ifstream file( file_path );

    if( file.is_open() ) {
        while( getline( file, line ) ) {
            line.erase( std::remove_if( line.begin(), line.end(), isSpace ), line.end() );

            auto separator = line.find( '=' );
            if( separator != std::string::npos ) {
                auto month_i = std::stoi( line.substr( 0, separator ) );
                auto month_s = line.substr( separator + 1 );
                month_map.insert( std::make_pair( month_i, month_s ) );
            }
        }

        file.close();
    } else {
        std::cerr << "Month string file \"" << file_path << "\" not found." << std::endl;
    }

    return month_map;
}