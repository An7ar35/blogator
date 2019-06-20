#include <iostream>
#include <dirent.h>
#include <regex>
#include <filesystem>

#include "indexer/indexer.h"
#include "generator/Generator.h"
#include "fs/fs.h"

int main() {
    using namespace blogator;

    auto options = dto::Options();
    options._paths.root_dir   = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog" );
    options._paths.source_dir = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/source" );
    options._paths.css_dir    = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/css" );
    options._paths.index_dir  = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/index" );
    options._paths.posts_dir  = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/posts" );
    options._paths.month_file = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/months.txt" );

    if( fs::setupEnvironment( options ) ) {
        auto index = indexer::index( options );
        std::cout << *index << std::endl;
        auto months = fs::importMonthNames( options._paths.month_file );

        auto generator = Generator( options );
        generator.useMonthStrings( months );
        generator.init( *index );

    } else {
        std::cerr << "Environment setup failed." << std::endl;
    }

    return 0;
}