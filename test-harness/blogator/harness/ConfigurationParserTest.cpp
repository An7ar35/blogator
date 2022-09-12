#include "ConfigurationParserTest.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test ConfigurationParserTest object
 * @return Output stream
 */
std::ostream & test_harness::blogator::operator <<( std::ostream &os, const test_harness::blogator::ConfigurationParserTest & test ) {
    os << test.description;
    return os;
}