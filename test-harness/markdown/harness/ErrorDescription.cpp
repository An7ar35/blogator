#include "ErrorDescription.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test ErrorDescription object
 * @return Output stream
 */
std::ostream & test_harness::markdown::operator <<( std::ostream &os, const test_harness::markdown::ErrorDescription & err ) {
    os << "{ description: \"" << err.description << "\", line: " << err.line << ", col: " << err.col << " }";
    return os;
}