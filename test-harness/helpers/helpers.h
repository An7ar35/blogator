#ifndef TEST_HARNESS_HELPERS_HELPERS_H
#define TEST_HARNESS_HELPERS_HELPERS_H

#include <vector>
#include <set>

#include "nlohmann/json.hpp"
#include "../../src/dto/U32Text.h"

namespace test_harness {
    nlohmann::json loadJSONFromFile( const std::filesystem::path &path );
    std::set<std::filesystem::path> getTestFiles( const std::filesystem::path &test_dir, const std::string & file_ext );
    blogator::U32Text transcodeInput( const std::string & raw, const std::filesystem::path & path );
    void printU32Buffer( std::ostream &os, blogator::U32Text & u32text );
}

#endif //TEST_HARNESS_HELPERS_HELPERS_H