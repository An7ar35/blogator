#include <gtest/gtest.h>
#include "../../src/fs/fs.h"

TEST( fs_tests, adaptRelPath ) {
    auto rel  = std::filesystem::path( "../../img/pic.jpeg" );
    auto from = std::filesystem::path( "/home/user/repos/websites/site1_folder/templates/index/index.html" );
    auto to   = std::filesystem::path( "/home/user/repos/websites/site1_folder/index/tags.html" );

    auto result   = blogator::fs::adaptRelPath( from, to, rel );
    auto expected = std::filesystem::path( "../img/pic.jpeg" );

    ASSERT_TRUE( result.compare( expected ) == 0 );
}