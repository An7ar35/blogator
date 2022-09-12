/**
 * This is a test harness to run all the configuration Parser tests from source JSON files
 *
 * Test subjects:
 * - `blogator::configuration::parser::Tokeniser`: Input is pipelined to this to get to the Parser
 * - `blogator::configuration::parser::Parser`: main subject of the tests
 * - `blogator::configuration::Configuration: Container object of the parsed configuration
 * - `blogator::reporter::ParserLog`: where all the parsing errors get sent to (a callback is used to check the actual msgs during testing)
 */
#include "gtest/gtest.h"
#include "../../../src/reporter/ParseReporter.h"
#include "../../../src/configuration/parser/Tokeniser.h"
#include "../../../src/configuration/parser/Parser.h"

#include "test-harness-sources.h"
#include "../helpers/helpers.h"
#include "harness/harness.h"

using blogator::reporter::ParseReporter;


/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const blogator::reporter::ReporterObject & e ) {
        _errors.push_back( e );
    }

    friend std::ostream & operator <<( std::ostream &os, const ParserLogCatcher &log_catcher ) {
        for( size_t i = 0, n = 1; i < log_catcher._errors.size(); ++i, ++n ) {
            os << "[" << i << "]: " << log_catcher._errors[i];

            if( n != log_catcher._errors.size() ) {
                os << "\n";
            }
        }

        return os;
    }

    [[nodiscard]] const std::vector<blogator::reporter::ReporterObject> & errors() const {
        return _errors;
    };

    [[nodiscard]] size_t count() const {
        return _errors.size();
    }

  private:
    std::vector<blogator::reporter::ReporterObject> _errors;
};

/**
 * Run a test
 * @param test Commonmark test
 * @param path Source file path of the test
 * @return Assert result
 */
testing::AssertionResult runConfigurationParserTest( const test_harness::blogator::ConfigurationParserTest &test, const std::filesystem::path &path ) {
    using namespace blogator::configuration;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher error_catcher;

    blogator::reporter::ParseReporter::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    std::u32string raw_txt = converter_U8toU32.from_bytes( test.input );

    auto input     = ::blogator::U32Text( path, raw_txt );
    auto parser    = parser::Parser();
    auto tokeniser = parser::Tokeniser( parser );

    parser.init( path );
    tokeniser.parse( input );

    const auto   output_config    = parser.reset();
    const auto   returned_entries = test_harness::blogator::jsonifyConfiguration( *output_config );
    const bool   output_match     = ( returned_entries == test.entries );
    const auto & errors_received  = error_catcher.errors();
    bool         failed_err       = false;

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

    if( !output_match || failed_err ) {
        std::stringstream ss;

        ss << "Failed test\n"
           << "File .............: " << test.src.filename().string() << "\n"
           << "Description ......: " << test.description << "\n"
           << "Input ............: " << test.input << "\n";

        if( output_match ) {
            ss << "Output match .....: YES\n";
        } else {
            ss << "Output match .....: NO\n"
               << "Expected .........: " << "\n";
            for( const auto & e : test.entries ) {
                ss << e << "\n";
            }
            ss << "Actual ...........: " << "\n";
            for( const auto & e : returned_entries ) {
                ss << e << "\n";
            }
        }

        std::stringstream err_message;

        if( failed_err ) {
            ss << "Errors ...........: " << errors_received.size() << "/" << test.errors.size() << "\n"
               << "\treceived:\n";

            for( const auto & err : errors_received ) {
                ss << "\t\t" << test_harness::blogator::to_string( err ) << "\n";
            }

            ss << "\texpected:\n";

            for( const auto & err : test.errors ) {
                ss << "\t\t" << err << "\n";
            }

        } else {
            ss << "Errors ...........: " << errors_received.size() << "/" << test.errors.size() << "\n";
        }

        return testing::AssertionFailure() << ss.str();
    }

    return testing::AssertionSuccess();
}

class configuration_parser_Parser_Tests : public testing::TestWithParam<std::pair<test_harness::blogator::ConfigurationParserTest, std::filesystem::path>> {};

TEST_P( configuration_parser_Parser_Tests, configuration_parser_tests ) {
    auto test = GetParam();
    EXPECT_TRUE( runConfigurationParserTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    ConfigurationParserTestInstance,
    configuration_parser_Parser_Tests,
    ::testing::ValuesIn( test_harness::blogator::loadConfigParserTests( test_harness::BLOGATOR_CONFIGURATION_PARSER_TEST_PATH ) )
);