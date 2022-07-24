//TODO description and notes

#include "gtest/gtest.h"
#include "../../../src/parser/encoding/Transcode.h"
#include "../../../src/parser/dom/TreeBuilder.h"
#include "../../../src/parser/tokeniser/Markdown.h"
#include "../../../src/parser/logging/ParserLog.h"
#include "../../../src/parser/dom/node/Element.h"

#include "../helpers/helpers.h"

using blogator::parser::logging::ParserLog;


/**
 * Parsing log catcher
 */
class ParserLogCatcher {
  public:
    void log( const blogator::parser::logging::ErrorObject & e ) {
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

    [[nodiscard]] const std::vector<blogator::parser::logging::ErrorObject> & errors() const {
        return _errors;
    };

    [[nodiscard]] size_t count() const {
        return _errors.size();
    }

  private:
    std::vector<blogator::parser::logging::ErrorObject> _errors;
};

/**
 * Run a test
 * @param test Commonmark test
 * @param path Source file path of the test
 * @return Assert result
 */
testing::AssertionResult runMarkdownTest( const test_harness::commonmark_spec_tests::CommonMarkTest &test, const std::filesystem::path &path ) {
    using namespace blogator::parser::dom;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher error_catcher;

    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    auto src             = test_harness::transcodeInput( test.markdown, path );
    auto builder         = blogator::parser::dom::TreeBuilder();
    auto md2html         = blogator::parser::builder::MarkdownToHtml();
    auto tokeniser       = blogator::parser::tokeniser::Markdown( md2html );

    md2html.init( path );
    builder.init( TreeBuilder::createHtmlDocument( U"", path ) );
    builder.setStrictChecking( false ); //some tests have some funky tags/attributes

    try {
        tokeniser.parse( src );
    } catch( ... ) {
        return testing::AssertionFailure() << "Exception thrown";
    }

    auto builder_errors = builder.errors();
    auto document       = builder.reset();

    std::stringstream html_output;
    html_output << *document;

    const auto returned_u8        = html_output.str();
    const bool output_match       = ( returned_u8 == test.html_output );


    return testing::AssertionFailure()
        << "Failed test #" << test.example_no << "\n"
        << "File .............: " << test.src.filename().string() << ":" << test.start_line << "\n"
        << "Input ............: " << "\n" << test.markdown << "\n"
        << "Expected .........: " << "\n" << test.html_output << "\n"
        << "Actual ...........: " << "\n" << returned_u8 << "\n"; //TODO
}

class parser_tokeniser_CommonMarkSpec_Tests : public testing::TestWithParam<std::pair<test_harness::commonmark_spec_tests::CommonMarkTest, std::filesystem::path>> {};

TEST_P( parser_tokeniser_CommonMarkSpec_Tests, commonmark_spec_tests) {
    auto test = GetParam();
    EXPECT_TRUE( runMarkdownTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    MarkdownTokeniserTestInstance,
    parser_tokeniser_CommonMarkSpec_Tests,
    ::testing::ValuesIn( test_harness::commonmark_spec_tests::loadMarkdownTests( test_harness::COMMONMARK_SPEC_TEST_PATH ) )
);