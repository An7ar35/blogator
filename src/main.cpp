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

    try {
        auto options = fs::importOptions( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/source/template/config.cfg" );
        options->_paths.root_dir       = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/" );
        options->_paths.source_dir     = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/source" );
        options->_paths.css_dir        = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/css" );
        options->_paths.index_dir      = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/index" );
        options->_paths.posts_dir      = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/posts" );

        if( fs::setupEnvironment( options ) ) {
            auto index     = indexer::index( options );
            auto templates = fs::importTemplates( *index );

            std::cout << *index << std::endl;

            auto post_maker    = generator::PostMaker( index, templates, options );
            auto index_maker   = generator::IndexMaker( index, templates, options );
            auto landing_maker = generator::LandingMaker( index, templates, options );
            auto rss           = generator::RSS( index, templates, options );
            auto generator     = generator::Generator( index, templates, options );

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