#ifndef TEST_HARNESS_HTML5LIB_TESTS_HARNESS_HARNESS_H
#define TEST_HARNESS_HTML5LIB_TESTS_HARNESS_HARNESS_H

#include "nlohmann/json.hpp"

#include "TreeConstructionTest.h"
#include "../../src/parser/dom/node/Document.h"
#include "../../src/parser/token/html5/HTML5Tk.h"
#include "../../src/reporter/dto/ReporterObject.h"

namespace test_harness::html5lib_tests {
    typedef std::vector<std::pair<test_harness::html5lib_tests::TreeConstructionTest, std::filesystem::path>> TreeConstructionTests_t;
    typedef std::vector<test_harness::html5lib_tests::TreeConstructionTest>                                   TreeConstructionTestCollection_t;

    std::string to_string( blogator::parser::dom::node::Document & document, bool fragment );

    /* Formatters */
    std::ostream & jsonifyHtml5Tokens( std::ostream & os , const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> & tokens );
    std::ostream & jsonifyErrorObjects( std::ostream & os, const std::vector<blogator::reporter::ReporterObject> &err );

    /* Unicode un-escape */
    std::u32string unescape( const std::u32string & str );
    std::string    unescape( const std::string & u8str );

    /* File loaders */
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> loadJSONTests( const std::filesystem::path &test_dir );
    std::vector<std::pair<TreeConstructionTest, std::filesystem::path>> loadTreeConstructionTests( const std::filesystem::path &test_dir );
}

#endif //TEST_HARNESS_HTML5LIB_TESTS_HARNESS_HARNESS_H