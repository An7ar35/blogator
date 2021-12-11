#include "File.h"

using namespace blogator::logger::output;

/**
 * Constructor
 * @param file_path Output file path
 */
File::File( const std::filesystem::path &file_path ) :
    LogOutput( LogOutputType::FILE, file_path.string() ),
    _path( file_path )
{}

/**
 * Destructor
 */
File::~File() {
    if( _out->is_open() ) {
        close();
    }
}

/**
 * Opens output
 * @throws std::runtime_error when output stream fails to open
 */
void File::open() {
    _out->open( _path, std::ios::out | std::ios::ate | std::ios::app ); //std::ios_base::binary | std::ios_base::out

    if( !_out->is_open() ) {
        throw( std::runtime_error( "[Logger] Unable to open file output stream: " + _path.string() ) );
    }
}

/**
 * Closes output
 * @throws std::runtime_error when output stream fails to close
 */
void File::close() {
    if( _out ) {
        _out->flush();
        _out->close();

        if( _out->fail() ) {
            throw( std::runtime_error( "[Logger] Failed to close output stream: " + _path.string() ) );
        }
    }
}

/**
 * Writes line to output
 * @param lvl Logging level (unused)
 * @param msg Line
 */
void File::write( LogLevel lvl, const std::string &msg ) {
    (*_out) << msg << "\n";
}