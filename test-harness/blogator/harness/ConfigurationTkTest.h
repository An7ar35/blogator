#ifndef TEST_HARNESS_BLOGATOR_HARNESS_CONFIGURATIONTKTEST_H
#define TEST_HARNESS_BLOGATOR_HARNESS_CONFIGURATIONTKTEST_H

#include <filesystem>
#include <string>
#include <vector>

#include "ErrorDescription.h"
#include "nlohmann/json.hpp"

namespace test_harness::blogator {
    typedef nlohmann::json TokenDescription_t;

    /**
     * Configuration token test object
     * @param src Source file of test
     * @param description Description of test
     * @param input Raw text input
     * @param tokens Collection of expected tokens
     * @param errors Collection of expected errors
     */
    struct ConfigurationTkTest {
        typedef std::vector<TokenDescription_t> Tokens_t;
        typedef std::vector<ErrorDescription>   Errors_t;

        std::filesystem::path src         {};
        std::string           description {};
        std::string           input       {};
        Tokens_t              tokens      {};
        Errors_t              errors      {};
    };

    std::ostream & operator <<( std::ostream &os, const ConfigurationTkTest &test );
}

#endif //TEST_HARNESS_BLOGATOR_HARNESS_CONFIGURATIONTKTEST_H
