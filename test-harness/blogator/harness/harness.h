#ifndef TEST_HARNESS_BLOGATOR_HARNESS_H
#define TEST_HARNESS_BLOGATOR_HARNESS_H

#include <vector>

#include "ConfigurationTkTest.h"
#include "ConfigurationParserTest.h"

#include "../../../src/reporter/dto/ReporterObject.h"

namespace blogator::configuration {
    class Configuration;
}

namespace test_harness::blogator {
    std::string to_string( const ::blogator::reporter::ReporterObject & error );

    std::vector<std::pair<ConfigurationTkTest, std::filesystem::path>> loadConfigTokeniserTests( const std::filesystem::path &test_dir );
    std::vector<std::pair<ConfigurationParserTest, std::filesystem::path>> loadConfigParserTests( const std::filesystem::path &test_dir );

    nlohmann::json jsonifyConfiguration( const ::blogator::configuration::Configuration & cfg );
    std::ostream & jsonifyConfiguration( std::ostream & os, const ::blogator::configuration::Configuration & cfg );
};


#endif //TEST_HARNESS_BLOGATOR_HARNESS_H