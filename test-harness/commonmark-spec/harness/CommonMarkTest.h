#ifndef TEST_HARNESS_COMMONMARK_SPEC_HARNESS_COMMONMARKTEST_H
#define TEST_HARNESS_COMMONMARK_SPEC_HARNESS_COMMONMARKTEST_H

#include <filesystem>
#include <string>
#include <ostream>

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

    std::ostream & operator <<( std::ostream &os, const CommonMarkTest &test );
}

#endif //TEST_HARNESS_COMMONMARK_SPEC_HARNESS_COMMONMARKTEST_H