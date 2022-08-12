#include "MarkdownTkTest.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test MarkdownTest object
 * @return Output stream
 */
std::ostream & test_harness::markdown::operator <<( std::ostream &os, const test_harness::markdown::MarkdownTkTest & test ) {
    os << test.description;
    return os;
}