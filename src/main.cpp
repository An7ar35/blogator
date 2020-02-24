#include <iostream>
#include <dirent.h>
#include <filesystem>

#include "project_version.h" //Generated from CMake

#include "cli/ArgParser.h"
#include "indexer/indexer.h"
#include "fs/fs.h"
#include "output/output.h"

namespace blogator {
    void printHeader();
}

int main( int argc, char **argv ) {
    using namespace blogator;
    std::setlocale( LC_ALL, "" );

    printHeader();

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
void blogator::printHeader() {
    auto printBorder = []( size_t width ) {
        std::cout << "·";
        for( auto i = 0; i < width - 2; ++i )
            std::cout << "~";
        std::cout << "·\n";
    };

    using blogator::cmake::AUTHOR;
    using blogator::cmake::URL;
    using blogator::cmake::VERSION;

    const auto title   = "Blogator v" + VERSION;
    const auto credit  = "by " + AUTHOR + " - " + URL;
    const auto width   = std::max( title.size(), credit.size() ) + 2;

    printBorder( width );
    std::cout << " " << title << "\n"
              << " " << credit << "\n";
    printBorder( width );
}