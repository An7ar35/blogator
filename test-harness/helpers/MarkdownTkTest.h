#ifndef TEST_HARNESS_HELPERS_MARKDOWNTEST_H
#define TEST_HARNESS_HELPERS_MARKDOWNTEST_H

#include <filesystem>
#include <string>

namespace test_harness::markdown {
    struct TokenDescription {
        std::string type;
        std::string text;
    };

    struct ErrorDescription {
        std::string description;
        size_t      line;
        size_t      col;
    };

    struct MarkdownTkTest {
        typedef std::vector<TokenDescription> Tokens_t;
        typedef std::vector<ErrorDescription> Errors_t;

        std::filesystem::path src         {};
        std::string           description {};
        std::string           input       {};
        Tokens_t              tokens      {};
        Errors_t              errors      {};
    };
}

#endif //TEST_HARNESS_HELPERS_MARKDOWNTEST_H