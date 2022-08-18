#include "ErrorDescription.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test ErrorDescription object
 * @return Output stream
 */
std::ostream & test_harness::blogator::operator <<( std::ostream &os, const test_harness::blogator::ErrorDescription & err ) {
    os << "{ description: \"" << err.description << "\", line: " << err.line << ", col: " << err.col << " }";
    return os;
}