#include "CommonMarkTest.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test CommonMarkTest object
 * @return Output stream
 */
std::ostream & test_harness::commonmark_spec_tests::operator <<( std::ostream &os, const test_harness::commonmark_spec_tests::CommonMarkTest &test ) {
    os << test.src.filename().string() << ":" << test.start_line << ", #"
       << test.example_no << ", ";
    return os;
}