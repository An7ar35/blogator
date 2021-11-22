#ifndef BLOGATOR_TESTS_HELPER_H
#define BLOGATOR_TESTS_HELPER_H

#include <filesystem>
#include <vector>

#include "nlohmann/json.hpp"

#include "cmake_test_vars.h"
#include "../../src/parser/token/html5/tokens.h"
#include "../../src/parser/logging/ErrorObject.h"
#include "../../src/unicode/unicode.h"

namespace blogator::tests {
    /* Formatters */
    std::ostream & jsonifyHtml5Tokens( std::ostream & os , const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> & tokens );
    std::ostream & jsonifyErrorObjects( std::ostream & os, const std::vector<blogator::parser::logging::ErrorObject> &err );

    /* Unicode un-escape */
    std::u32string unescape( const std::u32string & str );
    std::string unescape( const std::string & u8str );

    /* File loaders */
    std::vector<std::filesystem::path> getTestFiles( const std::filesystem::path & test_dir );
    nlohmann::json loadJSON( const std::filesystem::path & path );
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> loadJSONTests( const std::filesystem::path &test_dir );
    std::string loadFile( const std::filesystem::path & path );
}

#endif //BLOGATOR_TESTS_HELPER_H