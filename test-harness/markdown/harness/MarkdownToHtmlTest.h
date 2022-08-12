#ifndef TEST_HARNESS_MARKDOWN_HARNESS_MARKDOWNTOHTMLTEST_H
#define TEST_HARNESS_MARKDOWN_HARNESS_MARKDOWNTOHTMLTEST_H

#include <filesystem>
#include <string>
#include <vector>

#include "ErrorDescription.h"

namespace test_harness::markdown {
    struct MarkdownToHtmlTest {
        typedef std::vector<ErrorDescription> Errors_t;

        std::filesystem::path src         {};
        size_t                id          { 0 };
        std::string           description {};
        std::string           markdown    {};
        std::string           html_output {};
        Errors_t              errors      {};
    };

    std::ostream & operator <<( std::ostream &os, const MarkdownToHtmlTest &test );
}

#endif //TEST_HARNESS_MARKDOWN_HARNESS_MARKDOWNTOHTMLTEST_H
