#ifndef TEST_HARNESS_HELPERS_HELPERS_H
#define TEST_HARNESS_HELPERS_HELPERS_H

#include <ostream>
#include <vector>

#include "test-harness-sources.h"
#include "nlohmann/json.hpp"

#include "../helpers/CommonMarkTest.h"
#include "../helpers/TreeConstructionTest.h"

#include "../helpers/MarkdownToHtmlTest.h"
#include "../helpers/MarkdownTkTest.h"

#include "../../src/parser/token/html5/tokens.h"
#include "../../src/parser/token/markdown/tokens.h"
#include "../../src/parser/logging/ErrorObject.h"
#include "../../src/parser/dom/node/Document.h"

namespace test_harness {
    nlohmann::json loadJSONFromFile( const std::filesystem::path &path );
    std::set<std::filesystem::path> getTestFiles( const std::filesystem::path &test_dir, const std::string & file_ext );
    blogator::parser::U32Text transcodeInput( const std::string & raw, const std::filesystem::path & path );
}

namespace test_harness::commonmark_spec_tests {
    std::ostream & operator <<( std::ostream &os, const CommonMarkTest &test );
    std::vector<std::pair<CommonMarkTest, std::filesystem::path>> loadMarkdownTests( const std::filesystem::path &test_dir );
}

namespace test_harness::markdown {
    std::ostream & operator <<( std::ostream &os, const MarkdownTkTest &test );
    std::ostream & operator <<( std::ostream &os, const MarkdownToHtmlTest &test );
    std::ostream & operator <<( std::ostream &os, const ErrorDescription &err );
    std::string to_string( const blogator::parser::token::markdown::MarkdownTk & token );
    std::string to_string( const blogator::parser::logging::ErrorObject & error );

    std::vector<std::pair<MarkdownTkTest, std::filesystem::path>> loadTokeniserTests( const std::filesystem::path &test_dir );
    std::vector<std::pair<MarkdownToHtmlTest, std::filesystem::path>> loadTreeBuilderTests( const std::filesystem::path &test_dir );
    std::string printDocumentBody( blogator::parser::dom::node::Document & document );
}

namespace test_harness::html5lib_tests {
    std::ostream & operator <<( std::ostream &os, const TreeConstructionTest &test );
    std::string to_string( const TreeConstructionTest::Errors_t &errors );
    std::string to_string( blogator::parser::dom::node::Document & document, bool fragment );

    /* Formatters */
    std::ostream & jsonifyHtml5Tokens( std::ostream & os , const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> & tokens );
    std::ostream & jsonifyMarkdownTokens( std::ostream & os , const std::vector<std::unique_ptr<blogator::parser::token::markdown::MarkdownTk>> & tokens );
    std::ostream & jsonifyErrorObjects( std::ostream & os, const std::vector<blogator::parser::logging::ErrorObject> &err );
    std::string    formatErrorObject( const blogator::parser::logging::ErrorObject & err );

    /* Unicode un-escape */
    std::u32string unescape( const std::u32string & str );
    std::string    unescape( const std::string & u8str );

    /* File loaders */
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> loadJSONTests( const std::filesystem::path &test_dir );
    std::vector<std::pair<TreeConstructionTest, std::filesystem::path>> loadTreeConstructionTests( const std::filesystem::path &test_dir );
}

#endif //TEST_HARNESS_HELPERS_HELPERS_H