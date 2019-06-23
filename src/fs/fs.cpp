#include "fs.h"
#include <iostream>
#include <fstream>
#include <regex>

#include "../exception/file_access_failure.h"

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
    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return false;
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
 * Imports the strings to use for each months of the year
 * @param file_path Path for month file
 * @return Imported key-value pairs from file
 * @throws blogator::exception::file_access_failure when access to source file fails
 * @throws std::runtime_error when any of the data checks fails
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

    if( !file.is_open() )
        throw exception::file_access_failure( "Month string file \"" + file_path.string() + "\" could not be opened." );

    //Import months from file
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

    //Validate the imported months
    auto validated = 0;
    for( auto i = 1; i <= 12; ++i )
        validated += ( month_map.find( i ) == month_map.end() ) ? 0 : 1;

    if( validated < 12 )
        throw std::runtime_error( std::to_string( validated ) + "/12 month strings valid." );

    return month_map;
}