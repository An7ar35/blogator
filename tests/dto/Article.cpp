#include "gtest/gtest.h"
#include "../../src/dto/Article.h"

TEST( Article_Tests, CaseInsensitiveCompare ) {
    auto compare = blogator::dto::Article::CaseInsensitiveCompare();

    ASSERT_FALSE( compare( "", "" ) );

    ASSERT_FALSE( compare( "a", "A" ) );
    ASSERT_FALSE( compare( "A", "a" ) );
    ASSERT_FALSE( compare( "a", "a" ) );

    ASSERT_FALSE( compare( "b", "A" ) );
    ASSERT_FALSE( compare( "B", "a" ) );
    ASSERT_FALSE( compare( "b", "a" ) );

    ASSERT_TRUE( compare( "a", "b" ) );
    ASSERT_TRUE( compare( "a", "aa" ) );
    ASSERT_TRUE( compare( "a", "AA" ) );

    ASSERT_TRUE( compare( "C", "compare" ) );
    ASSERT_FALSE( compare( "compare", "comp" ) );

    ASSERT_FALSE( compare( "same", "SAME" ) );
    ASSERT_FALSE( compare( "SAME", "same" ) );
    ASSERT_FALSE( compare( "same", "same" ) );
}