#include "TreeConstructionTest.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param test TreeConstructionTest object
 * @return Output stream
 */
std::ostream & test_harness::html5lib_tests::operator <<( std::ostream &os, const test_harness::html5lib_tests::TreeConstructionTest &test ) {
    os << test.src.filename().string() << ":" << test.line << ", #"
       << test.id << ", ";

    if( test.scripting ) {
        os << "script: on";
    } else {
        os << "script: off";
    }

    if( test.is_fragment ) {
        os << ", context: '";
        if( !test.ctx_prefix.empty() ) {
            os << test.ctx_prefix << ":";
        }
        os << test.ctx_local_name << "'";
    }

    return os;
}

/*
 * Stringify a list of errors
 * @param errors List of errors
 * @return String
 */
std::string test_harness::html5lib_tests::to_string( const test_harness::html5lib_tests::TreeConstructionTest::Errors_t &errors ) {
    std::stringstream ss;

    for( size_t i = 0, n = 1; i < errors.size(); ++i, ++n ) {
        ss << "[" << i << "]: " << errors[i];

        if( n != errors.size() ) {
            ss << "\n";
        }
    }

    return ss.str();
}