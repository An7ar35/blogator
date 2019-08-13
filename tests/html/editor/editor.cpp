#include "gtest/gtest.h"
#include "../../../src/html/editor/editor.h"

TEST( html_editor_tests, removeHyperlinks0 ) {
    using blogator::html::editor::removeHyperlinks;
    std::string line = "some text > with > no hyperlink <span>M</span> ";
    ASSERT_EQ( 0, removeHyperlinks( line ) );
}

TEST( html_editor_tests, removeHyperlinks1 ) {
    using blogator::html::editor::removeHyperlinks;
    std::string line     = "some text <a href=\"../hello.png\">link</a> and some extra text.";
    std::string expected = "some text link and some extra text.";
    ASSERT_EQ( 1, removeHyperlinks( line ) );
    ASSERT_EQ( expected, line );
}

TEST( html_editor_tests, removeHyperlinks2 ) {
    using blogator::html::editor::removeHyperlinks;
    std::string line     = "some text <a href=\"../hello.png\">link _ with a space </a>12345<a href='../../../pages/projects.html'>EADlib</a>6789.";
    std::string expected = "some text link _ with a space 12345EADlib6789.";
    ASSERT_EQ( 2, removeHyperlinks( line ) );
    ASSERT_EQ( expected, line );
}

TEST( html_editor_tests, removeHyperlinks3 ) {
    using blogator::html::editor::removeHyperlinks;
    std::string line     = "some text <a href=\"../hello.png\">link _ with a space </a>12345<a href='../../../pages/projects.html'>EADlib</a not closed.";
    std::string expected = "some text link _ with a space 12345<a href='../../../pages/projects.html'>EADlib</a not closed.";
    ASSERT_EQ( 1, removeHyperlinks( line ) );
    ASSERT_EQ( expected, line );
}