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
    options._root_dir   = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog" );
    options._source_dir = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/source" );
    options._css_dir    = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/css" );
    options._index_dir  = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/index" );
    options._posts_dir  = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/blog/posts" );

    if( fs::setupEnvironment( options ) ) {
        auto index = indexer::index( options );
        std::cout << *index << std::endl;

        auto generator = Generator( options );
        generator.init( *index );
    } else {
        std::cerr << "Environment setup failed." << std::endl;
    }

    return 0;
}