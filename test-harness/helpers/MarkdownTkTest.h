#ifndef TEST_HARNESS_HELPERS_MARKDOWNTEST_H
#define TEST_HARNESS_HELPERS_MARKDOWNTEST_H

#include <filesystem>
#include <string>

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
}

#endif //TEST_HARNESS_HELPERS_MARKDOWNTEST_H