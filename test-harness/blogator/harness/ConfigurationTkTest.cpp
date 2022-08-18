#include "ConfigurationTkTest.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test ConfigurationTkTest object
 * @return Output stream
 */
std::ostream & test_harness::blogator::operator <<( std::ostream &os, const test_harness::blogator::ConfigurationTkTest & test ) {
    os << test.description;
    return os;
}