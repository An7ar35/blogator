#ifndef TEST_HARNESS_BLOGATOR_HARNESS_H
#define TEST_HARNESS_BLOGATOR_HARNESS_H

#include <vector>

#include "ConfigurationTkTest.h"
#include "ConfigurationParserTest.h"

#include "../../../src/reporter/dto/ReporterObject.h"

namespace test_harness::blogator {
    std::string to_string( const ::blogator::reporter::ReporterObject & error );

    std::vector<std::pair<ConfigurationTkTest, std::filesystem::path>> loadTokeniserTests( const std::filesystem::path &test_dir );
};


#endif //TEST_HARNESS_BLOGATOR_HARNESS_H