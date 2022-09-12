#ifndef TEST_HARNESS_BLOGATOR_HARNESS_CONFIGURATIONPARSERTEST_H
#define TEST_HARNESS_BLOGATOR_HARNESS_CONFIGURATIONPARSERTEST_H

#include <filesystem>
#include <string>
#include <vector>

#include "ErrorDescription.h"
#include "nlohmann/json.hpp"

namespace test_harness::blogator {
    typedef nlohmann::json ConfigDescription_t;

    /**
     * Configuration parser test object
     * @param src Source file of test
     * @param description Test description
     * @param input Raw text input
     * @param entries Collection of expected configuration entries
     * @param errors Collection of expected errors
     */
    struct ConfigurationParserTest {
        typedef std::vector<ConfigDescription_t> ConfigEntries_t;
        typedef std::vector<ErrorDescription>    Errors_t;

        std::filesystem::path src         {};
        std::string           description {};
        std::string           input       {};
        ConfigEntries_t       entries     {};
        Errors_t              errors      {};
    };

    std::ostream & operator <<( std::ostream &os, const ConfigurationParserTest &test );
}

#endif //TEST_HARNESS_BLOGATOR_HARNESS_CONFIGURATIONPARSERTEST_H
