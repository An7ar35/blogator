#include <gtest/gtest.h>
#include "../../src/fs/fs.h"

TEST( fs_tests, adaptRelPath ) { //TODO
    auto rel  = std::filesystem::path( "../../img/pic.jpeg" );
    auto from = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/templates/index/index.html" );
    auto to   = std::filesystem::path( "/home/alwyn/git_repos/corbreuse/preservons-corbreuse.bitbucket.io/index/tags.html" );
    auto source_norm = ( from.parent_path() / rel ).lexically_normal();
    auto target_rel = source_norm.lexically_relative( to.parent_path() );

    std::cout << "origin_file = " << from.parent_path() << std::endl;
    std::cout << "target_file   = " << to.parent_path() << std::endl;
    std::cout << "source rel  = " << source_norm << std::endl;
    std::cout << "target_rel  = " << target_rel << std::endl;
    if( !std::filesystem::exists( source_norm ) )
        std::cout << "WARNING: Relative path in file *** line x col y does not point to an existing file" << std::endl;
}