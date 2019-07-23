#include <iostream>
#include <dirent.h>
#include <regex>
#include <filesystem>

#include "cli/ArgParser.h"
#include "indexer/indexer.h"
#include "fs/fs.h"
#include "output/output.h"

namespace blogator {
    void printHeader( const dto::BlogatorInfo & info );
}

int main( int argc, char **argv ) {
    using namespace blogator;
    std::setlocale( LC_ALL, "" );

    const dto::BlogatorInfo info = { "Blogator", "An7ar35", "1.0b", "https://an7ar35.bitbucket.io" };
    printHeader( info );

    auto &display = cli::MsgInterface::getInstance();
    auto parser   = cli::ArgParser();
    auto state    = parser.parse( argc, argv );

    if( state == cli::ArgParser::State::STOP )
        return 0;
    if( state == cli::ArgParser::State::ERROR )
        return 1;

    try {
        std::cout << "> Reading configuration file...\n";
        auto options = fs::importOptions( parser.getWorkingDirectory() / "blogator.cfg", info );
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
void blogator::printHeader( const dto::BlogatorInfo & info ) {
    auto printBorder = []( size_t width ) {
        std::cout << "·";
        for( auto i = 0; i < width - 2; ++i )
            std::cout << "~";
        std::cout << "·\n";
    };

    const auto title   = info._name + " v" + info._version;
    const auto credit  = "by " + info._author + " - " + info._url;
    const auto width   = std::max( title.size(), credit.size() ) + 2;

    printBorder( width );
    std::cout << " " << title << "\n"
              << " " << credit << "\n";
    printBorder( width );
}