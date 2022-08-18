/**
 * This is a test harness to run all the configuration tokenizer tests from source JSON files
 *
 * Test subjects:
 * - `blogator::configuration::parser::Tokeniser`: main subject of the tests
 * - `blogator::reporter::ParserLog`: where all the parsing errors get sent to (a callback is used to check the actual msgs during testing)
 */
#include "gtest/gtest.h"
#include "../../../src/reporter/ParseReporter.h"
#include "../../../src/dto/U32Text.h"
#include "../../../src/configuration/parser/Tokeniser.h"
#include "../../../src/configuration/parser/Parser.h"
#include "../../../src/configuration/parser/token/ConfigTk.h"

#include "test-harness-sources.h"
#include "harness/harness.h"

using ::blogator::configuration::parser::Tokeniser;
using ::blogator::configuration::parser::token::ConfigTk;
using ::blogator::configuration::parser::specs::TokeniserState;
using ::blogator::reporter::ParseReporter;

/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const blogator::reporter::ReporterObject & e ) {
        _errors.push_back( e );
    }

    [[nodiscard]] const std::vector<blogator::reporter::ReporterObject> & errors() const {
        return _errors;
    }

  private:
    std::vector<blogator::reporter::ReporterObject> _errors;
};

/**
 * MOCK MarkdownToHtml class
 */
class MockConfigurationParser : public ::blogator::configuration::parser::Parser {
  public:
    explicit MockConfigurationParser() :
        blogator::configuration::parser::Parser()
    {}

    void dispatch( std::unique_ptr<ConfigTk> tk ) override {
        if( tk->type() != ::blogator::configuration::parser::specs::TokenType::END_OF_FILE ) {
            if( tk->type() == ::blogator::configuration::parser::specs::TokenType::CHARACTER
                && !_tokens.empty()
                && _tokens.back()->type() == ::blogator::configuration::parser::specs::TokenType::CHARACTER )
            {
                _tokens.back() = concatCharacterTk( std::move( _tokens.back() ), std::move( tk ) );
            } else {
                _tokens.emplace_back( std::move( tk ) );
            }
        }
    }

    [[nodiscard]] const std::vector<std::unique_ptr<ConfigTk>> & tokens() const {
        return _tokens;
    }

    [[nodiscard]]bool empty() const {
        return _tokens.empty();
    }

  private:
    std::vector<std::unique_ptr<ConfigTk>> _tokens;

    static std::unique_ptr<ConfigTk> concatCharacterTk( std::unique_ptr<ConfigTk> tk1, std::unique_ptr<ConfigTk> tk2 ) {
        return std::make_unique<::blogator::configuration::parser::token::ConfigTk>(
            ::blogator::configuration::parser::specs::TokenType::CHARACTER,
            ( tk1->text() + tk2->text() ),
            tk1->position()
        );
    }
};

/**
 * Run a test
 * @param test JSON-formatted test
 * @param path Source file path of the test
 * @return Assert result
 */
testing::AssertionResult runConfigurationTokeniserTest( const test_harness::blogator::ConfigurationTkTest &test, const std::filesystem::path &path ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher        error_catcher;
    MockConfigurationParser mock_configuration_parser;

    blogator::reporter::ParseReporter::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    std::u32string raw_txt = converter_U8toU32.from_bytes( test.input );

    auto input     = ::blogator::U32Text( path, raw_txt );
    auto tokeniser = Tokeniser( mock_configuration_parser );

    tokeniser.parse( input );

    // TEST CHECKS

    const auto & tokens_received = mock_configuration_parser.tokens();
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
            err_message << "\t\t" << test_harness::blogator::to_string( err ) << "\n";
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

    return testing::AssertionSuccess(); //TODO
}

class configuration_parser_Tokeniser_Tests : public testing::TestWithParam<std::pair<test_harness::blogator::ConfigurationTkTest, std::filesystem::path>> {};

TEST_P( configuration_parser_Tokeniser_Tests, tokeniser_tests ) {
    auto test = GetParam();
    EXPECT_TRUE( runConfigurationTokeniserTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    ConfigurationTokeniserTestInstance,
    configuration_parser_Tokeniser_Tests,
    ::testing::ValuesIn( test_harness::blogator::loadTokeniserTests( test_harness::BLOGATOR_CONFIGURATION_TOKENIZER_TEST_PATH ) )
);