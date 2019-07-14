#include <iostream>
#include <dirent.h>
#include <regex>
#include <filesystem>

#include "indexer/indexer.h"
#include "output/Generator.h"
#include "fs/fs.h"

int main() {
    using namespace blogator;
    std::setlocale( LC_ALL, "" );

    /* TODO args
     * -p (--preview) do a 'dump' collation for a post (create in a /tmp folder inside the root path and add a 'preview' in the file name)
     * -c (--create-config) generate a bog standard config file in the root of the site (see {path})
     * {path} : root of site, if not use running path as root
     * -h (--help) show options descriptions
     * */
    try {
        std::cout << "> Reading configuration file...\n";
        auto options = fs::importOptions( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blogator.cfg" );
        options->setupAbsolutePaths( std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io" ) );
                                    //TODO pass ^^^^ from cmd line args
        std::cout << "> Setting up environment";

        fs::setupEnvironment( options );
        auto index         = indexer::index( options );
        auto templates     = fs::importTemplates( *index, *options );
        auto post_maker    = output::page::Posts( index, templates, options );
        auto index_maker   = output::page::Indices( index, templates, options );
        auto landing_maker = output::page::Landing( index, templates, options );
        auto rss           = output::feed::RSS( index, templates, options );
        auto generator     = output::Generator( index, templates, options );

        generator.init( post_maker, index_maker, landing_maker, rss );

        fs::checkTemplateRelPaths( *index, *templates, *options );

    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}