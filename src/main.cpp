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

    auto options = std::make_shared<dto::Options>();
    options->_paths.root_dir       = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/" );
    options->_paths.source_dir     = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/source" );
    options->_paths.css_dir        = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/css" );
    options->_paths.index_dir      = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/index" );
    options->_paths.posts_dir      = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/posts" );
    options->_paths.month_file     = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/source/template/months.txt" );
    options->_index.items_per_page = 2;
    options->_rss.root             = "www.domain.com";

    if( fs::setupEnvironment( options ) ) {
        auto index     = indexer::index( options );
        auto templates = fs::importTemplates( *index );

        try {
            templates->_months = fs::importMonthNames( options->_paths.month_file );
        } catch ( std::exception &e ) {
            std::cout << "Using default month strings (eng)." << std::endl;
        }

        std::cout << *index << std::endl;

        auto post_maker    = generator::PostMaker( index, templates, options );
        auto index_maker   = generator::IndexMaker( index, templates, options );
        auto landing_maker = generator::LandingMaker( index, templates, options );
        auto rss           = generator::RSS( options );
        auto generator     = generator::Generator( index, templates, options );

        generator.init( *index, post_maker, index_maker, landing_maker, rss );

    } else {
        std::cerr << "Environment setup failed." << std::endl;
    }

    return 0;
}