#ifndef TEST_HARNESS_HELPERS_MARKDOWNTEST_H
#define TEST_HARNESS_HELPERS_MARKDOWNTEST_H

namespace test_harness::commonmark_spec_tests {
    struct MarkdownTest {
        typedef std::string String_t;
        typedef std::string Output_t;

        std::filesystem::path src          {};
        size_t                example_no   { 0 };
        size_t                start_line   { 0 };
        size_t                end_line     { 0 };
        String_t              section_desc {};
        String_t              markdown     {};
        Output_t              html_output  {};
    };
}

#endif //TEST_HARNESS_HELPERS_MARKDOWNTEST_H