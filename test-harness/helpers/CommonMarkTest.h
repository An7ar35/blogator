#ifndef TEST_HARNESS_HELPERS_COMMONMARKTEST_H
#define TEST_HARNESS_HELPERS_COMMONMARKTEST_H

#include <filesystem>
#include <string>

namespace test_harness::commonmark_spec_tests {
    struct CommonMarkTest {
        std::filesystem::path src          {};
        size_t                example_no   { 0 };
        size_t                start_line   { 0 };
        size_t                end_line     { 0 };
        std::string           section_desc {};
        std::string           markdown     {};
        std::string           html_output  {};
    };
}

#endif //TEST_HARNESS_HELPERS_COMMONMARKTEST_H