#include "MarkdownToHtmlTest.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test MarkdownToHtmlTest object
 * @return Output stream
 */
std::ostream & test_harness::markdown::operator <<( std::ostream &os, const test_harness::markdown::MarkdownToHtmlTest &test ) {
    os << test.description;
    return os;
}