#ifndef TEST_HARNESS_MARKDOWN_ERRORDESCRIPTION_H
#define TEST_HARNESS_MARKDOWN_ERRORDESCRIPTION_H

#include <string>

namespace test_harness::markdown {
    struct ErrorDescription {
        std::string description;
        size_t      line;
        size_t      col;
    };
}

#endif //TEST_HARNESS_MARKDOWN_ERRORDESCRIPTION_H
