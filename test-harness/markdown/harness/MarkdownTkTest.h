#ifndef TEST_HARNESS_MARKDOWN_HARNESS_MARKDOWNTEST_H
#define TEST_HARNESS_MARKDOWN_HARNESS_MARKDOWNTEST_H

#include <filesystem>
#include <string>
#include <vector>

#include "ErrorDescription.h"
#include "nlohmann/json.hpp"


namespace test_harness::markdown {
    typedef nlohmann::json TokenDescription_t;

    struct MarkdownTkTest {
        typedef std::vector<TokenDescription_t> Tokens_t;
        typedef std::vector<ErrorDescription>   Errors_t;

        std::filesystem::path src         {};
        std::string           description {};
        std::string           input       {};
        Tokens_t              tokens      {};
        Errors_t              errors      {};
    };

    std::ostream & operator <<( std::ostream &os, const MarkdownTkTest &test );
}

#endif //TEST_HARNESS_MARKDOWN_HARNESS_MARKDOWNTEST_H