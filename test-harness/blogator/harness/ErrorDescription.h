#ifndef TEST_HARNESS_BLOGATOR_HARNESS_ERRORDESCRIPTION_H
#define TEST_HARNESS_BLOGATOR_HARNESS_ERRORDESCRIPTION_H

#include <string>
#include <ostream>

namespace test_harness::blogator {
    struct ErrorDescription {
        std::string description;
        size_t      line;
        size_t      col;
    };

    std::ostream & operator <<( std::ostream &os, const ErrorDescription &err );
}

#endif //TEST_HARNESS_BLOGATOR_HARNESS_ERRORDESCRIPTION_H