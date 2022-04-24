/**
 * This is a test harness to run all the html5-lib tokenizer tests from source JSON files
 *
 * Note: HTML5-lib is a UTF16 based parser whilst blogator is a UTF32 based parser so some of the
 *       test cases had to be removed since it only concerns UTF-16. These are (should) recreated
 *       in the `blogator::parser::encoding::Transcode` test suite.
 *
 * Test subjects:
 * - `blogator::parser::tokeniser::HTML5`: main subject of the tests
 * - `blogator::parser::encoding::Transcode::addToCodePoint(..)`: tests for some of the stream error messages
 * - `blogator::parser::logging::ParserLog`: where all the parsing errors get sent to (a callback is used to check the actual msgs during testing)
 */

#include "gtest/gtest.h"
#include "../../../src/parser/tokeniser/HTML5.h"
#include "../../../src/parser/logging/ParserLog.h"
#include "../../../src/parser/dto/Source.h"
#include "../../../src/parser/encoding/Transcode.h"

#include "../helpers/helpers.h"

using blogator::parser::tokeniser::HTML5;
using blogator::parser::token::html5::HTML5Tk;
using blogator::parser::specs::infra::TokeniserState;
using blogator::parser::logging::ParserLog;

TokeniserState getStateEnum( const std::string &str ) {
    static const std::map<std::string, TokeniserState> map = {
        { "Data state", TokeniserState::DATA },
        { "RCDATA state", TokeniserState::RCDATA },
        { "RAWTEXT state", TokeniserState::RAWTEXT },
        { "Script data state", TokeniserState::SCRIPT_DATA },
        { "PLAINTEXT state", TokeniserState::PLAIN_TEXT },
        { "Tag open state", TokeniserState::TAG_OPEN },
        { "End tag open state", TokeniserState::END_TAG_OPEN },
        { "Tag name state", TokeniserState::TAG_NAME },
        { "RCDATA less-than sign state", TokeniserState::RCDATA_LESS_THAN_SIGN },
        { "RCDATA end tag open state", TokeniserState::RCDATA_END_TAG_OPEN },
        { "RCDATA end tag name state", TokeniserState::RCDATA_END_TAG_NAME },
        { "RAWTEXT less-than sign state", TokeniserState::RAWTEXT_LESS_THAN_SIGN },
        { "RAWTEXT end tag open state", TokeniserState::RAWTEXT_END_TAG_OPEN },
        { "RAWTEXT end tag name state", TokeniserState::RAWTEXT_END_TAG_NAME },
        { "Script data less-than sign state", TokeniserState::SCRIPT_DATA_LESS_THAN_SIGN },
        { "Script data end tag open state", TokeniserState::SCRIPT_DATA_END_TAG_OPEN },
        { "Script data end tag name state", TokeniserState::SCRIPT_DATA_END_TAG_NAME },
        { "Script data escape start state", TokeniserState::SCRIPT_DATA_ESCAPE_START },
        { "Script data escape start dash state", TokeniserState::SCRIPT_DATA_ESCAPE_START_DASH },
        { "Script data escaped state", TokeniserState::SCRIPT_DATA_ESCAPED },
        { "Script data escaped dash state", TokeniserState::SCRIPT_DATA_ESCAPED_DASH },
        { "Script data escaped dash dash state", TokeniserState::SCRIPT_DATA_ESCAPED_DASH_DASH },
        { "Script data escaped less-than sign state", TokeniserState::SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN },
        { "Script data escaped end tag open state", TokeniserState::SCRIPT_DATA_ESCAPED_END_TAG_OPEN },
        { "Script data escaped end tag name state", TokeniserState::SCRIPT_DATA_ESCAPED_END_TAG_NAME },
        { "Script data double escape start state", TokeniserState::SCRIPT_DATA_DOUBLE_ESCAPE_START },
        { "Script data double escaped state", TokeniserState::SCRIPT_DATA_DOUBLE_ESCAPED },
        { "Script data double escaped dash state", TokeniserState::SCRIPT_DATA_DOUBLE_ESCAPED_DASH },
        { "Script data double escaped dash dash state", TokeniserState::SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH },
        { "Script data double escaped less-than sign state", TokeniserState::SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN },
        { "Script data double escape end state", TokeniserState::SCRIPT_DATA_DOUBLE_ESCAPE_END },
        { "Before attribute name state", TokeniserState::BEFORE_ATTRIBUTE_NAME },
        { "Attribute name state", TokeniserState::ATTRIBUTE_NAME },
        { "After attribute name state", TokeniserState::AFTER_ATTRIBUTE_NAME },
        { "Before attribute value state", TokeniserState::BEFORE_ATTRIBUTE_VALUE },
        { "Attribute value (double-quoted) state", TokeniserState::ATTRIBUTE_VALUE_DOUBLE_QUOTED },
        { "Attribute value (single-quoted) state", TokeniserState::ATTRIBUTE_VALUE_SINGLE_QUOTED },
        { "Attribute value (unquoted) state", TokeniserState::ATTRIBUTE_VALUE_UNQUOTED },
        { "After attribute value (quoted) state", TokeniserState::AFTER_ATTRIBUTE_VALUE_QUOTED },
        { "Self-closing start tag state", TokeniserState::SELF_CLOSING_START_TAG },
        { "Bogus comment state", TokeniserState::BOGUS_COMMENT },
        { "Markup declaration open state", TokeniserState::MARKUP_DECLARATION_OPEN },
        { "Comment start state", TokeniserState::COMMENT_START },
        { "Comment start dash state", TokeniserState::COMMENT_START_DASH },
        { "Comment state", TokeniserState::COMMENT },
        { "Comment less-than sign state", TokeniserState::COMMENT_LESS_THAN_SIGN },
        { "Comment less-than sign bang state", TokeniserState::COMMENT_LESS_THAN_SIGN_BANG },
        { "Comment less-than sign bang dash state", TokeniserState::COMMENT_LESS_THAN_SIGN_BANG_DASH },
        { "Comment less-than sign bang dash dash state", TokeniserState::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH },
        { "Comment end dash state", TokeniserState::COMMENT_END_DASH },
        { "Comment end state", TokeniserState::COMMENT_END },
        { "Comment end bang state", TokeniserState::COMMENT_END_BANG },
        { "DOCTYPE state", TokeniserState::DOCTYPE },
        { "Before DOCTYPE name state", TokeniserState::BEFORE_DOCTYPE_NAME },
        { "DOCTYPE name state", TokeniserState::DOCTYPE_NAME },
        { "After DOCTYPE name state", TokeniserState::AFTER_DOCTYPE_NAME },
        { "After DOCTYPE public keyword state", TokeniserState::AFTER_DOCTYPE_PUBLIC_KEYWORD },
        { "Before DOCTYPE public identifier state", TokeniserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER },
        { "DOCTYPE public identifier (double-quoted) state", TokeniserState::DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED },
        { "DOCTYPE public identifier (single-quoted) state", TokeniserState::DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED },
        { "After DOCTYPE public identifier state", TokeniserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER },
        { "Between DOCTYPE public and system identifiers state", TokeniserState::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIER },
        { "After DOCTYPE system keyword state", TokeniserState::AFTER_DOCTYPE_SYSTEM_KEYWORD },
        { "Before DOCTYPE system identifier state", TokeniserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER },
        { "DOCTYPE system identifier (double-quoted) state", TokeniserState::DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED },
        { "DOCTYPE system identifier (single-quoted) state", TokeniserState::DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED },
        { "After DOCTYPE system identifier state", TokeniserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER },
        { "Bogus DOCTYPE state", TokeniserState::BOGUS_DOCTYPE },
        { "CDATA section state", TokeniserState::CDATA_SECTION },
        { "CDATA section bracket state", TokeniserState::CDATA_SECTION_BRACKET },
        { "CDATA section end state", TokeniserState::CDATA_SECTION_END },
        { "Character reference state", TokeniserState::CHARACTER_REFERENCE },
        { "Named character reference state", TokeniserState::NAMED_CHARACTER_REFERENCE },
        { "Ambiguous ampersand state", TokeniserState::AMBIGUOUS_AMPERSAND },
        { "Numeric character reference state", TokeniserState::NUMERIC_CHARACTER_REFERENCE },
        { "Hexadecimal character reference start state", TokeniserState::HEXADECIMAL_CHARACTER_REFERENCE_START },
        { "Decimal character reference start state", TokeniserState::DECIMAL_CHARACTER_REFERENCE_START },
        { "Hexadecimal character reference state", TokeniserState::HEXADECIMAL_CHARACTER_REFERENCE },
        { "Decimal character reference state", TokeniserState::DECIMAL_CHARACTER_REFERENCE },
        { "Numeric character reference end state", TokeniserState::NUMERIC_CHARACTER_REFERENCE_END }
    };

    if( !map.contains( str ) )
        throw std::invalid_argument( "TokeniserState not found: \"" + str + "\"" );

    return map.at( str );
}

/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const blogator::parser::logging::ErrorObject & e ) {
        _errors.push_back( e );
    }

    [[nodiscard]] nlohmann::json jsonify() {
        std::stringstream ss;
        std::sort( _errors.begin(),
                   _errors.end(),
                   []( const auto & a, const auto & b ) -> bool { return a.textpos() < b.textpos(); }
        );
        test_harness::html5lib_tests::jsonifyErrorObjects( ss, _errors );
        auto s = ss.str();
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
    explicit MockTreeBuilder() :
        blogator::parser::dom::TreeBuilder()
    {};

    void dispatch( std::unique_ptr<HTML5Tk> tk ) override {
        if( tk->type() != blogator::parser::specs::infra::TokenType::END_OF_FILE ) {
            if( tk->type() == blogator::parser::specs::infra::TokenType::CHARACTER
                && !_tokens.empty()
                && _tokens.back()->type() == blogator::parser::specs::infra::TokenType::CHARACTER )
            {
                _tokens.back() = concatCharacterTk( std::move( _tokens.back() ), std::move( tk ) );
            } else {
                _tokens.emplace_back( std::move( tk ) );
            }
        }
    }

    [[nodiscard]] std::string toStr() const {
        std::stringstream  ss;
        test_harness::html5lib_tests::jsonifyHtml5Tokens( ss, _tokens );
        return ss.str();
    }

    [[nodiscard]]bool empty() const {
        return _tokens.empty();
    }

  private:
    std::vector<std::unique_ptr<HTML5Tk>> _tokens;

    static std::unique_ptr<HTML5Tk> concatCharacterTk( std::unique_ptr<HTML5Tk> tk1, std::unique_ptr<HTML5Tk> tk2 ) {
        return std::make_unique<blogator::parser::token::html5::CharacterTk>( ( tk1->text() + tk2->text() ), tk1->position() );
    }
};

/**
 * [HELPER] Passes each code points in a U32 string through the Transcoder global error checks
 * @param raw  Original raw UTF-32 text
 * @param path Source path
 * @return Processed/checked UTF-32 text
 */
std::u32string preprocess( std::u32string &raw, const std::filesystem::path &path ) {
    std::vector<uint32_t> processed_txt;
    std::stringstream     ss; //not actually used

    auto     source  = blogator::parser::Source( ss, path, blogator::parser::encoding::Format::UTF32_LE );
    uint32_t prev_cp = 0x00;

    for( auto cp : raw ) {
        /* This will send errors to the parser logger where appropriate */
        blogator::parser::encoding::Transcode::addCodePoint( source, prev_cp, cp, processed_txt );
        prev_cp = cp;
    }

    return { processed_txt.begin(), processed_txt.end() };
}

/**
 * Run a test
 * @param test JSON-formatted test
 * @param path Source file path of the test
 * @return Assert result
 */
testing::AssertionResult runHTML5TokeniserTest( const nlohmann::json &test, const std::filesystem::path &path ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    MockTreeBuilder  mock_tree_builder;
    ParserLogCatcher error_catcher;

    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    std::u32string raw_txt = converter_U8toU32.from_bytes( test.at( "input" ) );

    if( test.contains( "doubleEscaped" ) ) {
        raw_txt = test_harness::html5lib_tests::unescape( raw_txt );
    }

    auto text        = blogator::parser::U32Text( path, preprocess( raw_txt, path ) );
    auto init_states = std::vector<std::pair<TokeniserState, std::string>>();

    if( test.contains( "initialStates" ) && !test.at( "initialStates" ).empty() ) { //"[ ... ]"
        for( const auto & state : test.at( "initialStates" ) ) {
            init_states.emplace_back( getStateEnum( state ), state );
        }
    } else {
        init_states.emplace_back( TokeniserState::DATA, "DATA state" );
    }

    for( size_t i = 0; i < init_states.size(); ++i ) {
        auto tokeniser = HTML5(
            mock_tree_builder,
            init_states[i].first,
            ( test.contains( "lastStartTag" ) ? converter_U8toU32.from_bytes( test.at( "lastStartTag" ) ) : U"" )
        );

        tokeniser.parse( text );

        nlohmann::json expected_output = test.at( "output" );
        nlohmann::json actual_errors   = error_catcher.jsonify();
        std::string    actual_output   = mock_tree_builder.toStr();

        if( test.contains( "doubleEscaped" ) ) {
            auto expected_str = test_harness::html5lib_tests::unescape( to_string( test.at( "output" ) ) );
            expected_output = nlohmann::json::parse( expected_str );
            actual_output   = test_harness::html5lib_tests::unescape( actual_output );
        }

        if( nlohmann::json::parse( actual_output ) != expected_output ) {
            return testing::AssertionFailure() << "Failed test - input-output mismatch\n"
                                               << "Description: " << test.at( "description" ) << "\n"
                                               << "Init state.: " << init_states[i].second << " (" << (i + 1) << "/" << init_states.size() << ")\n"
                                               << "Input .....: " << test.at( "input" ) << "\n"
                                               << "Expected ..: " << test.at( "output" ) << "\n"
                                               << "Actual ....: " << actual_output;
        }

        if( test.contains( "errors" ) && ( test.at( "errors") != actual_errors ) ) {
            return testing::AssertionFailure() << "Failed test - error mismatch\n"
                                               << "Description: " << test.at( "description" ) << "\n"
                                               << "Expected ..: " << test.at( "errors" ) << "\n"
                                               << "Actual ....: " << actual_errors;
        }
    }

    return testing::AssertionSuccess();
}

class parser_tokeniser_HTML5_Tests : public testing::TestWithParam<std::pair<nlohmann::json, std::filesystem::path>> {};

TEST_P( parser_tokeniser_HTML5_Tests, html5lib_tests) {
    auto test = GetParam();
    EXPECT_TRUE( runHTML5TokeniserTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    HTML5TokeniserTestInstance,
    parser_tokeniser_HTML5_Tests,
    ::testing::ValuesIn( test_harness::html5lib_tests::loadJSONTests( test_harness::HTML5LIB_TOKENIZER_TEST_PATH ) )
);