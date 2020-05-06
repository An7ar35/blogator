#include "gtest/gtest.h"
#include "../../src/dto/TableOfContents.h"

class TableOfContents_tests : public testing::Test {
  protected:
    void SetUp() override { toc = std::make_unique<blogator::dto::TableOfContents>(); }
    void TearDown() override {}
  public:
    std::unique_ptr<blogator::dto::TableOfContents> toc;
};

class TableOfContents_Heading_tests : public testing::Test {
  protected:
    void SetUp() override {
        heading = std::make_unique<blogator::dto::TableOfContents::Heading>(
            blogator::dto::TableOfContents::HeadingLevel::H1,
            "Heading text"
        );
    }
    void TearDown() override {}
  public:
    std::unique_ptr<blogator::dto::TableOfContents::Heading> heading;
};

TEST_F( TableOfContents_Heading_tests, ostream1 ) {
    heading->numbering = std::vector<int>( { 1 } );
    std::stringstream ss;
    ss << *heading;
    ASSERT_EQ( "1. Heading text", ss.str() );
}

TEST_F( TableOfContents_Heading_tests, ostream2 ) {
    heading->numbering = std::vector<int>( { 1, 5 } );
    std::stringstream ss;
    ss << *heading;
    ASSERT_EQ( "1.5 Heading text", ss.str() );
}

TEST_F( TableOfContents_Heading_tests, ostream3 ) {
    heading->numbering = std::vector<int>( { 0, 1, 5, 4, 6 } );
    std::stringstream ss;
    ss << *heading;
    ASSERT_EQ( "0.1.5.4.6 Heading text", ss.str() );
}

TEST_F( TableOfContents_Heading_tests, printNumbering1 ) {
    heading->numbering = std::vector<int>( { 1 } );
    ASSERT_EQ( "1.", heading->printNumbering() );
}

TEST_F( TableOfContents_Heading_tests, printNumbering2 ) {
    heading->numbering = std::vector<int>( { 1, 0, 3 } );
    ASSERT_EQ( "1.0.3", heading->printNumbering() );
}

TEST_F( TableOfContents_Heading_tests, printNumbering3 ) {
    heading->numbering = std::vector<int>( { 0, 1, 5, 4, 6 } );
    ASSERT_EQ( "0.1.5.4.6", heading->printNumbering() );
}

TEST_F( TableOfContents_Heading_tests, printID1 ) {
    heading->numbering = std::vector<int>( { 1 } );
    ASSERT_EQ( "heading_1", heading->printID() );
}

TEST_F( TableOfContents_Heading_tests, printID2 ) {
    heading->numbering = std::vector<int>( { 1, 0, 3 } );
    ASSERT_EQ( "heading_1_0_3", heading->printID()  );
}

TEST_F( TableOfContents_Heading_tests, printID3 ) {
    heading->numbering = std::vector<int>( { 0, 1, 5, 4, 6 } );
    ASSERT_EQ( "heading_0_1_5_4_6", heading->printID()  );
}

TEST_F( TableOfContents_Heading_tests, depth1 ) {
    heading->numbering = std::vector<int>( { 1, 2 } );
    ASSERT_EQ( heading->numbering.size(), heading->depth() );
}

TEST_F( TableOfContents_Heading_tests, depth2 ) {
    heading->numbering = std::vector<int>( { 0, 1, 5, 4, 6 } );
    ASSERT_EQ( 5, heading->depth() );
}

TEST_F( TableOfContents_tests, finaliseToC1 ) {
    using blogator::dto::TableOfContents;
    /* Expected */
    auto a = TableOfContents::Heading( TableOfContents::HeadingLevel::H3, "Heading A" );
    a.numbering = std::vector<int>( { 0, 0, 1 } );
    auto b = TableOfContents::Heading( TableOfContents::HeadingLevel::H2, "Heading B" );
    b.numbering = std::vector<int>( { 0, 1 } );
    auto c = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading C" );
    c.numbering = std::vector<int>( { 1 } );
    auto d = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading D" );
    d.numbering = std::vector<int>( { 2 } );
    auto expected = std::list<TableOfContents::Heading>( { a, b, c, d } );
    /* ------- */
    toc->findHeading( 1, "<h3>Heading A</h3>" );
    toc->findHeading( 2, "<h2>Heading B</h2>" );
    toc->findHeading( 3, "<h1>Heading C</h1>" );
    toc->findHeading( 4, "<h1>Heading D</h1>" );
    toc->finaliseToC();
    auto &results = toc->headings();

    ASSERT_EQ( expected.size(), results.size() );

    auto exp = expected.cbegin();
    auto res = results.cbegin();
    while( exp != expected.cend() ) {
        EXPECT_EQ( *exp, res->second ) << "Headings did not match: " << *exp << " != " << res->second;
        ++exp; ++res;
    }
}

TEST_F( TableOfContents_tests, finaliseToC2 ) {
    using blogator::dto::TableOfContents;
    /* Expected */
    auto a = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading A" );
    a.numbering = std::vector<int>( { 1 } );
    auto b = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading B" );
    b.numbering = std::vector<int>( { 2 } );
    auto c = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading C" );
    c.numbering = std::vector<int>( { 3 } );
    auto expected = std::list<TableOfContents::Heading>( { a, b, c } );
    /* ------- */
    toc->findHeading( 1, "<h1>Heading A</h1>" );
    toc->findHeading( 2, "<h1>Heading B</h1>" );
    toc->findHeading( 3, "<h1>Heading C</h1>" );
    toc->finaliseToC();
    auto &results = toc->headings();

    ASSERT_EQ( expected.size(), results.size() );

    auto exp = expected.cbegin();
    auto res = results.cbegin();
    while( exp != expected.cend() ) {
        EXPECT_EQ( *exp, res->second ) << "Headings did not match: " << *exp << " != " << res->second;
        ++exp; ++res;
    }
}

TEST_F( TableOfContents_tests, finaliseToC3 ) {
    using blogator::dto::TableOfContents;
    /* Expected */
    auto a = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading A" );
    a.numbering = std::vector<int>( { 1 } );
    auto b = TableOfContents::Heading( TableOfContents::HeadingLevel::H2, "Heading B" );
    b.numbering = std::vector<int>( { 1, 1 } );
    auto c = TableOfContents::Heading( TableOfContents::HeadingLevel::H3, "Heading C" );
    c.numbering = std::vector<int>( { 1, 1, 1 } );
    auto d = TableOfContents::Heading( TableOfContents::HeadingLevel::H2, "Heading D" );
    d.numbering = std::vector<int>( { 1, 2 } );
    auto e = TableOfContents::Heading( TableOfContents::HeadingLevel::H3, "Heading E" );
    e.numbering = std::vector<int>( { 1, 2, 1 } );
    auto f = TableOfContents::Heading( TableOfContents::HeadingLevel::H1, "Heading F" );
    f.numbering = std::vector<int>( { 2 } );
    auto expected = std::list<TableOfContents::Heading>( { a, b, c, d, e, f } );
    /* ------- */
    toc->findHeading( 1, "<h1>Heading A</h1>" );
    toc->findHeading( 2, "<h2>Heading B</h2>" );
    toc->findHeading( 3, "<h3>Heading C</h3>" );
    toc->findHeading( 4, "<h2>Heading D</h2>" );
    toc->findHeading( 5, "<h3>Heading E</h3>" );
    toc->findHeading( 6, "<h1>Heading F</h1>" );
    toc->finaliseToC();
    auto &results = toc->headings();

    ASSERT_EQ( expected.size(), results.size() );

    auto exp = expected.cbegin();
    auto res = results.cbegin();
    while( exp != expected.cend() ) {
        EXPECT_EQ( *exp, res->second ) << "Headings did not match: " << *exp << " != " << res->second;
        ++exp; ++res;
    }
}

TEST_F( TableOfContents_tests, finaliseToC4 ) {
    using blogator::dto::TableOfContents;
    /* Expected */
    auto a = TableOfContents::Heading( TableOfContents::HeadingLevel::H2, "Heading A" );
    a.numbering = std::vector<int>( { 1, } );
    auto b = TableOfContents::Heading( TableOfContents::HeadingLevel::H5, "Heading B" );
    b.numbering = std::vector<int>( { 1, 0, 0, 1 } );
    auto c = TableOfContents::Heading( TableOfContents::HeadingLevel::H5, "Heading C" );
    c.numbering = std::vector<int>( { 1, 0, 0, 2 } );
    auto d = TableOfContents::Heading( TableOfContents::HeadingLevel::H4, "Heading D" );
    d.numbering = std::vector<int>( { 1, 0, 1 } );
    auto e = TableOfContents::Heading( TableOfContents::HeadingLevel::H3, "Heading E" );
    e.numbering = std::vector<int>( { 1, 1 } );
    auto expected = std::list<TableOfContents::Heading>( { a, b, c, d, e } );
    /* ------- */
    toc->findHeading( 1, "<h2>Heading A</h2>" );
    toc->findHeading( 2, "<h5>Heading B</h5>" );
    toc->findHeading( 3, "<h5>Heading C</h5>" );
    toc->findHeading( 4, "<h4>Heading D</h4>" );
    toc->findHeading( 5, "<h3>Heading E</h3>" );
    toc->finaliseToC();
    auto &results = toc->headings();

    ASSERT_EQ( expected.size(), results.size() );

    auto exp = expected.cbegin();
    auto res = results.cbegin();
    while( exp != expected.cend() ) {
        EXPECT_EQ( *exp, res->second ) << "Headings did not match: " << *exp << " != " << res->second;
        ++exp; ++res;
    }
}


TEST_F( TableOfContents_tests, minHeadingLevel1 ) {
    toc = std::make_unique<blogator::dto::TableOfContents>( 4, 0 );
    ASSERT_EQ( blogator::dto::TableOfContents::HeadingLevel::H1, toc->minHeadingLevel() );
}

TEST_F( TableOfContents_tests, minHeadingLevel2 ) {
    toc = std::make_unique<blogator::dto::TableOfContents>( 5, 2 );
    ASSERT_EQ( blogator::dto::TableOfContents::HeadingLevel::H3, toc->minHeadingLevel() );
}

TEST_F( TableOfContents_tests, maxHeadingLevel1 ) {
    toc = std::make_unique<blogator::dto::TableOfContents>( 4, 0 );
    ASSERT_EQ( blogator::dto::TableOfContents::HeadingLevel::H4, toc->maxHeadingLevel() );
}

TEST_F( TableOfContents_tests, maxHeadingLevel2 ) {
    toc = std::make_unique<blogator::dto::TableOfContents>( 4, 2 );
    ASSERT_EQ( blogator::dto::TableOfContents::HeadingLevel::H6, toc->maxHeadingLevel() );
}

TEST_F( TableOfContents_tests, maxHeadingLevel3 ) {
    toc = std::make_unique<blogator::dto::TableOfContents>( 1, 5 );
    ASSERT_EQ( blogator::dto::TableOfContents::HeadingLevel::H6, toc->maxHeadingLevel() );
}

TEST_F( TableOfContents_tests, Constructor_properties_failure ) {
    EXPECT_THROW( toc = std::make_unique<blogator::dto::TableOfContents>( 5, 8 );, std::invalid_argument );
}

TEST_F( TableOfContents_tests, rootHeadingLevel ) {
    using blogator::dto::TableOfContents;

    toc->findHeading( 1, "<h3>Heading</h3>" );
    toc->findHeading( 1, "<h5>Heading</h5>" );
    toc->findHeading( 1, "<h4>Heading</h4>" );

    ASSERT_EQ( TableOfContents::HeadingLevel::H3, toc->rootHeadingLevel() );
}

TEST_F( TableOfContents_tests, leafHeadingLevel ) {
    using blogator::dto::TableOfContents;

    toc->findHeading( 1, "<h3>Heading</h3>" );
    toc->findHeading( 1, "<h5>Heading</h5>" );
    toc->findHeading( 1, "<h4>Heading</h4>" );

    ASSERT_EQ( TableOfContents::HeadingLevel::H5, toc->leafHeadingLevel() );
}

TEST_F( TableOfContents_tests, findHeadings1 ) {
    using blogator::dto::TableOfContents;

    toc->findHeading( 1, "  <h2>Sub-Heading</h2>   " );
    ASSERT_EQ( 1, toc->headings().size() );
    const auto &heading = toc->headings().cbegin();

    ASSERT_EQ( TableOfContents::HeadingLevel::H2, heading->second.level );
    ASSERT_EQ( "Sub-Heading", heading->second.str );
    ASSERT_EQ( 1, heading->first.line );
    ASSERT_EQ( 5, heading->first.col );
    ASSERT_EQ( 1, heading->second.str_pos.line );
    ASSERT_EQ( 6, heading->second.str_pos.col );
}

TEST_F( TableOfContents_tests, findHeadings2 ) {
    using blogator::dto::TableOfContents;

    toc->findHeading( 1, "<h3>Sub-sub-Heading</h3><h4>Another heading</h4>" );
    ASSERT_EQ( 1, toc->headings().size() );
    const auto &heading = toc->headings().cbegin();

    ASSERT_EQ( TableOfContents::HeadingLevel::H3, heading->second.level );
    ASSERT_EQ( "Sub-sub-Heading", heading->second.str );
    ASSERT_EQ( 1, heading->first.line );
    ASSERT_EQ( 3, heading->first.col );
    ASSERT_EQ( 1, heading->second.str_pos.line );
    ASSERT_EQ( 4, heading->second.str_pos.col );
}


TEST_F( TableOfContents_tests, findHeadings3 ) {
    using blogator::dto::TableOfContents;

    toc->findHeading( 1, "<h3 >Inconsistent tags</h2>   <h4 >Good tags</h4>" );
    ASSERT_EQ( 1, toc->headings().size() );
    const auto &heading = toc->headings().cbegin();

    ASSERT_EQ( TableOfContents::HeadingLevel::H4, heading->second.level );
    ASSERT_EQ( "Good tags", heading->second.str );
    ASSERT_EQ( 1, heading->first.line );
    ASSERT_EQ( 33, heading->first.col );
    ASSERT_EQ( 1, heading->second.str_pos.line );
    ASSERT_EQ( 35, heading->second.str_pos.col );
}

TEST_F( TableOfContents_tests, findHeadings4 ) {
    toc->findHeading( 1, "<h2 class=\"myclass\">heading text</h2>" );
    ASSERT_TRUE( toc->headings().empty() );
}

TEST_F( TableOfContents_tests, findHeadings5 ) {
    toc->findHeading( 1, "<h2 id=\"myid\">heading text</h2>" );
    ASSERT_TRUE( toc->headings().empty() );
}

TEST_F( TableOfContents_tests, findTocBlock1 ) {
    using blogator::dto::InsertPosition;
    ASSERT_FALSE( toc->tocBlockExists() );
    toc->findTocBlock( 5, "<div class=\"invalid-toc\"></div>", "auto-toc" );
    ASSERT_FALSE( toc->tocBlockExists() );
}

TEST_F( TableOfContents_tests, findTocBlock2 ) {
    using blogator::dto::InsertPosition;
    auto ip = InsertPosition( 5, 22 );
    ASSERT_FALSE( toc->tocBlockExists() );
    toc->findTocBlock( ip.line, "<div class=\"auto-toc\"></div>", "auto-toc" );
    ASSERT_EQ( 1, toc->tocPositions().size() );
    ASSERT_EQ( ip, toc->tocPositions().front() );
}