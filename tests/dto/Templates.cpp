#include "gtest/gtest.h"
#include "../../src/dto/Template.h"
#include "../../src/dto/Template.cpp"
#include "../../src/dto/Templates.h"
#include "../../src/dto/Templates.cpp"

TEST( Template_tests, extractRelativePaths0 ) {
    using blogator::dto::HTML;
    using blogator::dto::Templates;

    auto html = HTML();
    html._lines.emplace_back( "" );
    ASSERT_TRUE( Templates::extractRelativePaths( html ).empty() );
}

TEST( Template_tests, extractRelativePaths1 ) {
    using blogator::dto::HTML;
    using blogator::dto::Templates;

    auto html = HTML();
    html._lines.emplace_back( R"(<link rel="stylesheet" type="text/css" href="css/main.css" />)" );
    auto result = Templates::extractRelativePaths( html );

    ASSERT_EQ( 1, result.size() );
    ASSERT_EQ( 0, result.cbegin()->first.line );
    ASSERT_EQ( 45, result.cbegin()->first.col );
    ASSERT_EQ( "css/main.css", result.cbegin()->second );
}

TEST( Template_tests, extractRelativePaths2 ) {
    using blogator::dto::HTML;
    using blogator::dto::Templates;

    auto html = HTML();
    html._lines.emplace_back( R"(<link rel="stylesheet" type="text/css" href="css/main.css" /><link rel="stylesheet" type="text/css" href="css/landing.css" />)" );
    auto result = Templates::extractRelativePaths( html );

    ASSERT_EQ( 2, result.size() );
    auto r1 = result.cbegin();
    ASSERT_EQ( 0, r1->first.line );
    ASSERT_EQ( 45, r1->first.col );
    ASSERT_EQ( "css/main.css", r1->second );
    auto r2 = std::next( r1 );
    ASSERT_EQ( 0, r2->first.line );
    ASSERT_EQ( 106, r2->first.col );
    ASSERT_EQ( "css/landing.css", r2->second );
}

TEST( Template_tests, extractRelativePaths3 ) {
    using blogator::dto::HTML;
    using blogator::dto::Templates;

    auto html = HTML();
    html._lines.emplace_back( R"(<script type="text/javascript" src="js/sticky-top-navbar.js"></script>)" );
    html._lines.emplace_back( "lorum ipsson text with a dir/path/filename.ext" );
    html._lines.emplace_back( "<img src=\"/absolute/path/rock.jpg\"/>" );
    html._lines.emplace_back( R"(<link rel="stylesheet" type="text/css" href="css/main.css" /><link rel="stylesheet" type="text/css" href="css/landing.css" />)" );
    auto result = Templates::extractRelativePaths( html );

    ASSERT_EQ( 3, result.size() );
    auto r0 = result.cbegin();
    ASSERT_EQ( 0, r0->first.line );
    ASSERT_EQ( 36, r0->first.col );
    ASSERT_EQ( "js/sticky-top-navbar.js", r0->second );
    auto r1 = std::next( r0 );
    ASSERT_EQ( 3, r1->first.line );
    ASSERT_EQ( 45, r1->first.col );
    ASSERT_EQ( "css/main.css", r1->second );
    auto r2 = std::next( r1 );
    ASSERT_EQ( 3, r2->first.line );
    ASSERT_EQ( 106, r2->first.col );
    ASSERT_EQ( "css/landing.css", r2->second );
}

TEST( Template_tests, extractRelativePaths4 ) {
    using blogator::dto::HTML;
    using blogator::dto::Templates;

    auto html = HTML();
    html._lines.emplace_back( R"(    Le site est <a href="../../pages/liens.html">enfin</a> en ligne!!!<a href="../../pages/liens.html">LINK!</a>)" );
    html._lines.emplace_back( R"(                    <img src="../img/icons/calendar_white.svg" alt="Calendar icon"> <!-- CCBY4.0, src: https://www.svgrepo.com/svg/116079/newspaper -->)" );
    auto result = Templates::extractRelativePaths( html );

    ASSERT_EQ( 3, result.size() );
    auto r0 = result.cbegin();
    ASSERT_EQ( 0, r0->first.line );
    ASSERT_EQ( 25, r0->first.col );
    ASSERT_EQ( "../../pages/liens.html", r0->second );
    auto r1 = std::next( r0 );
    ASSERT_EQ( 0, r1->first.line );
    ASSERT_EQ( 57, r1->first.col );
    ASSERT_EQ( "../../pages/liens.html", r1->second );
    auto r2 = std::next( r1 );
    ASSERT_EQ( 1, r2->first.line );
    ASSERT_EQ( 30, r2->first.col );
    ASSERT_EQ( "../img/icons/calendar_white.svg", r2->second );
}