#ifndef TEST_HARNESS_HTML5LIB_TESTS_HELPERS_H
#define TEST_HARNESS_HTML5LIB_TESTS_HELPERS_H

#include <ostream>
#include <vector>

#include "nlohmann/json.hpp"

#include "test-harness-sources.h"
#include "../../src/parser/token/html5/tokens.h"
#include "../../src/parser/logging/ErrorObject.h"
#include "../../src/parser/dom/node/Document.h"

namespace test_harness::html5lib_tests::helpers {
    struct TreeConstructionTest {
        typedef std::string              Data_t;
        typedef std::vector<std::string> Errors_t;
        typedef std::string              Output_t;

        size_t                id              {};
        size_t                line            {};
        std::filesystem::path src             {};
        Data_t                data            {};
        bool                  is_fragment     { false };
        Data_t                ctx_prefix      {};
        Data_t                ctx_local_name  {};
        Errors_t              errors          {};
        bool                  scripting       { false };
        Output_t              expected_output {};
    };

    std::ostream & operator <<( std::ostream &os, const TreeConstructionTest &test );
    std::string to_string( const TreeConstructionTest::Errors_t &errors );
    std::string to_string( blogator::parser::dom::node::Document & document, bool fragment );

    /* Formatters */
    std::ostream & jsonifyHtml5Tokens( std::ostream & os , const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> & tokens );
    std::ostream & jsonifyErrorObjects( std::ostream & os, const std::vector<blogator::parser::logging::ErrorObject> &err );
    std::string    formatErrorObject( const blogator::parser::logging::ErrorObject & err );

    /* Unicode un-escape */
    std::u32string unescape( const std::u32string & str );
    std::string    unescape( const std::string & u8str );

    /* File loaders */
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> loadJSONTests( const std::filesystem::path &test_dir );
    std::vector<std::pair<TreeConstructionTest, std::filesystem::path>> loadTreeConstructionTests( const std::filesystem::path &test_dir );
}

#endif //TEST_HARNESS_HTML5LIB_TESTS_HELPERS_H