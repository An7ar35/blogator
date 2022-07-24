#ifndef TEST_HARNESS_HELPERS_MARKDOWNTOHTMLTEST_H
#define TEST_HARNESS_HELPERS_MARKDOWNTOHTMLTEST_H

#include <filesystem>
#include <string>

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
}

#endif //TEST_HARNESS_HELPERS_MARKDOWNTOHTMLTEST_H
