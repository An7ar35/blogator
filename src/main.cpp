#include <iostream>
#include <dirent.h>
#include <regex>
#include <filesystem>

#include "cli/ArgParser.h"
#include "indexer/indexer.h"
#include "fs/fs.h"
#include "output/output.h"

namespace blogator {
    void printheader();
}

int main( int argc, char **argv ) {
    using namespace blogator;
    std::setlocale( LC_ALL, "" );

    printheader();

    auto &display = cli::MsgInterface::getInstance();
    auto parser   = cli::ArgParser();
    auto state    = parser.parse( argc, argv );

    if( state == cli::ArgParser::State::STOP )
        return 0;
    if( state == cli::ArgParser::State::ERROR )
        return 1;

    try {
        std::cout << "> Reading configuration file...\n";
        auto options = fs::importOptions( parser.getWorkingDirectory() / "blogator.cfg" );
        options->setupAbsolutePaths( parser.getWorkingDirectory() );
        options->setTempPath( parser.getTempDirectory() );

        std::cout << "> Setting up environment";
        fs::setupEnvironment( options );

        auto index     = indexer::index( options );
        auto templates = fs::importTemplates( *index, *options );
        output::generate( index, templates, options );

        fs::checkTemplateRelPaths( *index, *templates, *options );

    } catch( std::exception &e ) {
        display.error( e.what() );
        return 1;
    }

    return 0;
}

/**
 * Prints the Title of the application to the terminal
 */
void blogator::printheader() {
    auto title = std::string( dto::Options::BLOGATOR_NAME ) + " v" + std::string( dto::Options::BLOGATOR_VERSION );
    auto left  = std::string( "|                 " );
    auto right = std::string( "                 |" );
    auto width = left.size() + title.size() + right.size();

    std::stringstream ss;
    ss << "·";
    for( auto i = 0; i < width - 2; ++i ) ss << "-";
    ss << "·";
    std::cout << ss.str() << "\n"
              << left << title << right << "\n"
              << ss.str() << std::endl;
}