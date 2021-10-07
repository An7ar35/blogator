#include "gtest/gtest.h"
#include "../../../src/parser/tokeniser/HTML5.h"
#include "../../../src/parser/logging/ParserLog.h"

#include "../../helper.h"

using blogator::parser::tokeniser::HTML5;
using blogator::parser::token::html5::HTML5Tk;
using blogator::parser::specs::html5::TokeniserState;
using blogator::parser::dom::HtmlState_e;
using blogator::parser::dom::HtmlNamespace_e;
using blogator::parser::logging::ParserLog;

/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const blogator::parser::logging::ErrorObject & e ) {
        _errors.push_back( e );
    }

    [[nodiscard]] nlohmann::json jsonify() const {
        std::stringstream ss;
        blogator::tests::jsonifyHtml5Errors( ss, _errors );
        return nlohmann::json::parse( ss.str() );
    }

  private:
    std::vector<blogator::parser::logging::ErrorObject> _errors;
};

/**
 * MOCK TreeBuilder class
 */
class MockTreeBuilder : public blogator::parser::dom::TreeBuilder {
  public:
    void addToken( std::unique_ptr<HTML5Tk> tk ) override {
        _tokens.emplace_back( std::move( tk ) );
    }

    [[nodiscard]] nlohmann::json jsonify() const {
        std::stringstream  ss;
        blogator::tests::jsonifyHtml5Tokens( ss, _tokens );
        return nlohmann::json::parse( ss.str() );
    }

    [[nodiscard]]bool empty() const {
        return _tokens.empty();
    }

  private:
    std::vector<std::unique_ptr<HTML5Tk>> _tokens;
};


/**
 * Runs all tests
 * @param test Collection of JSON-formatted tests
 * @return Assert result
 */
testing::AssertionResult runTest( const nlohmann::json &test ) {
    MockTreeBuilder  mock_tree_builder;
    ParserLogCatcher error_catcher;

    std::u32string raw_txt   = blogator::unicode::utf32::convert( test.at( "input" ) );
    auto           text      = blogator::parser::U32Text( raw_txt );
    auto           tokens    = blogator::parser::Tokens_t();
    auto           tokeniser = HTML5( mock_tree_builder );

    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );
    tokeniser.parse( text );

    auto actual_tokens = mock_tree_builder.jsonify();
    auto actual_errors = error_catcher.jsonify();

    if( actual_tokens != test.at( "output" ) ) {
        return testing::AssertionFailure() << "Failed test - input-output mismatch\n"
                                           << "Description: " << test.at( "description" ) << "\n"
                                           << "Input .....: " << test.at( "input" ) << "\n"
                                           << "Expected ..: " << test.at( "output" ) << "\n"
                                           << "Actual ....: " << actual_tokens;
    }

    if( test.contains( "errors" ) && ( test.at( "errors") != actual_errors ) ) {
        return testing::AssertionFailure() << "Failed test - error mismatch\n"
                                           << "Description: " << test.at( "description" ) << "\n"
                                           << "Expected ..: " << test.at( "errors" ) << "\n"
                                           << "Actual ....: " << actual_errors;
    }

    return testing::AssertionSuccess();
}

class HTML5TokeniserTest  : public testing::TestWithParam<std::pair<nlohmann::json, std::filesystem::path>> {};

TEST_P(HTML5TokeniserTest, html5lib_tests) {
    auto test = GetParam();
    EXPECT_TRUE( runTest( test.first ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    HTML5TokeniserTestInstance,
    HTML5TokeniserTest,
    ::testing::ValuesIn( blogator::tests::loadJSONTests( blogator::tests::HTML5LIB_TOKENIZER_TEST_PATH ) )
);