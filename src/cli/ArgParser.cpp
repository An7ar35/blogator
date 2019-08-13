#include "ArgParser.h"

#include <iostream>

#include "../fs/ConfigReader.h"

/**
 * Constructor
 */
blogator::cli::ArgParser::ArgParser() :
    _display( MsgInterface::getInstance() )
{}

/**
 * Parse arguments
 * @param argc Number of arguments
 * @param argv Arguments
 * @return Parser's state post-parsing
 */
blogator::cli::ArgParser::State blogator::cli::ArgParser::parse( int argc, char **argv ) {
    bool error      { false };
    bool create_cfg { false };
    bool show_help  { false };

    if( argc == 1 ) { //No args passed
        return State::OK;
    }

    for( auto i = 1; i < argc; ++i ) {
        auto str    = std::string( argv[ i ] );
        auto flag   = isFlag( str );

        switch( flag ) {
            case Flag::HELP: //-h, -?, --help
                show_help = true;
                break;

            case Flag::CREATE_CONFIG: //-c, --create-config
                create_cfg = true;
                break;

            case Flag::SHOW_DEBUG_MSGS: //-d, --debug
                _display.setShowDebug( true );
                break;

            case Flag::INVALID:
                break;
        }

        if( flag == Flag::INVALID ) {
            auto dir_ok = setPath( str );

            if( !dir_ok ) {
                _display.error( "[CLI] argument not recognised: " + str );
                error = true;
            }
        }
    }

    if( error || show_help ) {
        printHelp();
        return ( error ? State::ERROR : State::STOP );
    } else if( create_cfg ) {
        createConfigFile();
        return State::STOP;
    } else {
        return State::OK;
    }
}

/**
 * Gets the working directory for the application
 * @return Working directory
 */
std::filesystem::path blogator::cli::ArgParser::getWorkingDirectory() const {
    if( _working_dir.empty() )
        return std::filesystem::current_path();
    else
        return _working_dir;
}

/**
 * Gets the system-based temporary directory's path for the application
 * @return Temporary folder
 */
std::filesystem::path blogator::cli::ArgParser::getTempDirectory() const {
    return std::filesystem::temp_directory_path() / "blogator";
}

/**
 * Creates a default configuration file as described in the fs::ConfigReader
 * @return Success in creating the file
 */
bool blogator::cli::ArgParser::createConfigFile() const {
    auto target = getWorkingDirectory() / "blogator.cfg";

    if( std::filesystem::exists( target ) ) {
        _display.warning( "There is already a config file in the working directory." );
        return false;
    }

    fs::ConfigReader::generateBlankConfigFile( target );
    return true;
}

/**
 * Sets the working directory if none is already set
 * @param str argument string representing a path
 * @return Validity of the string
 */
bool blogator::cli::ArgParser::setPath( const std::string &str ) {
    try {
        auto path = std::filesystem::path( str );

        if( path.has_filename() ) {
            _display.error( "[CLI] Directory path cannot be a file: " + str );
            return false;
        }

        if( !std::filesystem::exists( path ) ) {
            _display.error( "[CLI] Directory does not exist: " + str );
            return false;
        }

        if( !_working_dir.empty() ) {
            _display.error( "[CLI] Only one working directory must be given: " + str );
            return false;
        }

        _working_dir = path;
        return true;

    } catch( std::exception &e ) {
        _display.error( "[CLI] Invalid path: " + str );
        return false;
    }
}

/**
 * Gets the associated Flag enum for a string representing a flag
 * @param str Flag as a string
 * @return Flag enum (Flag::INVALID if str is not a valid flag)
 */
blogator::cli::ArgParser::Flag blogator::cli::ArgParser::isFlag( const std::string &str ) const {
    try {
        return _flag_lookup.at( str );
    } catch( std::out_of_range &e ) {
        return Flag::INVALID;
    }
}

/**
 * Prints out the CLI help
 */
void blogator::cli::ArgParser::printHelp() const {
    std::string prog_name = "blogator";
    std::stringstream out;

    out << "Usage:\n"
        << "  " << prog_name << " [option] <directory>\n"
        << "\n"
        << "Options:\n"
        << "  -h, --help           Displays this information.\n"
        << "  -d, --debug          Turns on the debug messages.\n"
        << "  -c, --create-config  Creates an example config file in the working directory.\n"
        << "\n"
        << "<directory>            [optional] Working directory of the site from which to\n"
        << "                       generate the blog. If no path is provided then the working\n"
        << "                       directory will be set to wherever " << prog_name << " is run from.\n";

    std::cout << out.str() << std::endl;
}