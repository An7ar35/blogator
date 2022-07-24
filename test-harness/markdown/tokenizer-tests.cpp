/**
 * This is a test harness to run all the Markdown tokenizer tests from source JSON files
 *
 * Test subjects:
 * - `blogator::parser::tokeniser::Markdown`: main subject of the tests
 * - `blogator::parser::logging::ParserLog`: where all the parsing errors get sent to (a callback is used to check the actual msgs during testing)
 */
#include "gtest/gtest.h"
#include "../../../src/parser/dom/TreeBuilder.h"
#include "../../../src/parser/tokeniser/Markdown.h"
#include "../../../src/parser/logging/ParserLog.h"
#include "../../../src/parser/dto/Source.h"
#include "../../../src/parser/encoding/Transcode.h"

#include "../helpers/helpers.h"

using blogator::parser::dom::TreeBuilder;
using blogator::parser::tokeniser::Markdown;
using blogator::parser::token::markdown::MarkdownTk;
using blogator::parser::specs::markdown::TokeniserState;
using blogator::parser::logging::ParserLog;

/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const blogator::parser::logging::ErrorObject & e ) {
        _errors.push_back( e );
    }

    [[nodiscard]] const std::vector<blogator::parser::logging::ErrorObject> & errors() const {
        return _errors;
    }

  private:
    std::vector<blogator::parser::logging::ErrorObject> _errors;
};

/**
 * MOCK MarkdownToHtml class
 */
class MockMarkdownToHtml : public blogator::parser::builder::MarkdownToHtml {
  public:
    explicit MockMarkdownToHtml() :
        blogator::parser::builder::MarkdownToHtml()
    {}

    void dispatch( std::unique_ptr<MarkdownTk> tk ) override {
        if( tk->type() != blogator::parser::specs::markdown::TokenType::END_OF_FILE ) {
            if( tk->type() == blogator::parser::specs::markdown::TokenType::CHARACTER
                && !_tokens.empty()
                && _tokens.back()->type() == blogator::parser::specs::markdown::TokenType::CHARACTER )
            {
                _tokens.back() = concatCharacterTk( std::move( _tokens.back() ), std::move( tk ) );
            } else {
                _tokens.emplace_back( std::move( tk ) );
            }
        }
    }

    [[nodiscard]] const std::vector<std::unique_ptr<MarkdownTk>> & tokens() const {
        return _tokens;
    }

    [[nodiscard]]bool empty() const {
        return _tokens.empty();
    }

  private:
    std::vector<std::unique_ptr<MarkdownTk>> _tokens;

    static std::unique_ptr<MarkdownTk> concatCharacterTk( std::unique_ptr<MarkdownTk> tk1, std::unique_ptr<MarkdownTk> tk2 ) {
        return std::make_unique<blogator::parser::token::markdown::CharacterTk>( ( tk1->text() + tk2->text() ), tk1->position() );
    }
};

/**
 * Run a test
 * @param test JSON-formatted test
 * @param path Source file path of the test
 * @return Assert result
 */
testing::AssertionResult runMarkdownTokeniserTest( const test_harness::markdown::MarkdownTkTest &test, const std::filesystem::path &path ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher   error_catcher;
    MockMarkdownToHtml mock_markdown_to_html;

    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    std::u32string raw_txt = converter_U8toU32.from_bytes( test.input );

    auto input     = blogator::parser::U32Text( path, raw_txt );
    auto tokeniser = Markdown( mock_markdown_to_html );

    tokeniser.parse( input );

    // TEST CHECKS

    const auto & tokens_received = mock_markdown_to_html.tokens();
    bool         failed_tk       = false;

    if( test.tokens.size() == tokens_received.size() ) {
        auto expected_tk_it = test.tokens.cbegin();
        auto received_tk_it = tokens_received.cbegin();

        while( expected_tk_it != test.tokens.cend() && received_tk_it != tokens_received.cend() ) {
            std::stringstream ss;
            ss << *(*received_tk_it);

            const auto received_json = nlohmann::json::parse( ss.str() );

            if( received_json != *expected_tk_it ) {
                failed_tk = true;
                break;
            }

            ++expected_tk_it;
            ++received_tk_it;
        }

    } else {
        failed_tk = true;
    }

    const auto & errors_received = error_catcher.errors();
    bool         failed_err      = false;

    if( test.errors.size() == errors_received.size() ) {
        auto expected_err_it = test.errors.cbegin();
        auto received_err_it = errors_received.cbegin();

        while( expected_err_it != test.errors.cend() && received_err_it != errors_received.cend() ) {

            if( received_err_it->error()        != expected_err_it->description ||
                received_err_it->textpos().line != expected_err_it->line        ||
                received_err_it->textpos().col  != expected_err_it->col )
            {
                failed_err = true;
                break;
            }

            ++expected_err_it;
            ++received_err_it;
        }

    } else {
        failed_err = true;
    }

    // TEST PRINTING

    std::stringstream tk_message;

    if( failed_tk ) {
        tk_message << tokens_received.size() << "/" << test.tokens.size() << "\n"
                   << "\treceived:\n";

        for( const auto & tk : tokens_received ) {
            std::stringstream ss;
            ss << (*tk);
            tk_message << "\t\t" << nlohmann::json::parse( ss.str() ) << "\n";
        }

        tk_message << "\texpected:\n";

        for( const auto & tk : test.tokens ) {
            tk_message << "\t\t" << tk << "\n";
        }

    } else {
        tk_message << tokens_received.size() << "/" << test.tokens.size() << "\n";
    }

    std::stringstream err_message;

    if( failed_err ) {
        err_message << errors_received.size() << "/" << test.errors.size() << "\n"
                    << "\treceived:\n";

        for( const auto & err : errors_received ) {
            err_message << "\t\t" << test_harness::markdown::to_string( err ) << "\n";
        }

        err_message << "\texpected:\n";

        for( const auto & err : test.errors ) {
            err_message << "\t\t" << err << "\n";
        }

    } else {
        err_message << errors_received.size() << "/" << test.errors.size() << "\n";
    }

    // TEST ASSERTION

    if( failed_err || failed_tk ) {
        return testing::AssertionFailure() << "Failed test\n"
                                           << "Description: " << test.description << "\n"
                                           << "Input .....: " << test.input << "\n"
                                           << "Tokens ....: " << tk_message.str()
                                           << "Errors ....: " << err_message.str()
                                           << "\n";
    }

    return testing::AssertionSuccess();
}


class parser_tokeniser_Markdown_Tests : public testing::TestWithParam<std::pair<test_harness::markdown::MarkdownTkTest, std::filesystem::path>> {};

TEST_P( parser_tokeniser_Markdown_Tests, markdown_tests ) {
    auto test = GetParam();
    EXPECT_TRUE( runMarkdownTokeniserTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    MarkdownTokeniserTestInstance,
    parser_tokeniser_Markdown_Tests,
    ::testing::ValuesIn(
        test_harness::markdown::loadTokeniserTests( test_harness::MARKDOWN_TOKENIZER_TEST_PATH ) )
);