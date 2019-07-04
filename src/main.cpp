#include <iostream>
#include <dirent.h>
#include <regex>
#include <filesystem>
#include <eadlib/cli/parser/Parser.h>

#include "indexer/indexer.h"
#include "generator/Generator.h"
#include "fs/fs.h"

int main() {
    using namespace blogator;
    std::setlocale( LC_ALL, "" );

    /* TODO args
     * -p (--preview) do a 'dump' collation for a post (create in a /tmp folder inside the root path and add a 'preview' in the file name)
     * -c (--create-config) generate a bog standard config file in the root of the site (see {path})
     * {path} : root of site, if not use running path as root
     * */
    try {
        std::cout << "> Reading configuration file...\n";
        auto options = fs::importOptions( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/blogator.cfg" );
        options->setupAbsolutePaths( std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog" ) );
                                    //TODO pass ^^^^ from cmd line args
        std::cout << "> Setting up environment...\n";
        if( fs::setupEnvironment( options ) ) {
            std::cout << "> Indexing sources...\n";
            auto index     = indexer::index( options );
            std::cout << *index << std::endl;
            std::cout << "> Importing templates\n";
            auto templates = fs::importTemplates( *index );

            auto post_maker    = generator::PostMaker( index, templates, options );
            auto index_maker   = generator::IndexMaker( index, templates, options );
            auto landing_maker = generator::LandingMaker( index, templates, options );
            auto rss           = generator::RSS( index, templates, options );
            auto generator     = generator::Generator( index, templates, options );

            std::cout << "> Generating html...\n";
            generator.init( *index, post_maker, index_maker, landing_maker, rss );

        } else {
            std::cerr << "Environment setup failed." << std::endl;
        }

    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}