//TODO description and notes

#include "gtest/gtest.h"
#include "../../../src/reporter/ParseReporter.h"
#include "../../../src/parser/dom/TreeBuilder.h"
#include "../../../src/parser/tokeniser/HTML5.h"
#include "../../../src/parser/tokeniser/Markdown.h"

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
testing::AssertionResult runMarkdownTest( const test_harness::commonmark_spec_tests::CommonMarkTest &test, const std::filesystem::path &path ) {
    using namespace blogator::parser::dom;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher error_catcher;

    blogator::reporter::ParseReporter::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    auto input_src           = test_harness::transcodeInput( test.markdown, path );
    auto md2html             = blogator::parser::builder::MarkdownToHtml();
    auto md_tokeniser        = blogator::parser::tokeniser::Markdown( md2html );
    auto md_tree_builder     = blogator::parser::dom::TreeBuilder();
    auto html5_tokeniser     = blogator::parser::tokeniser::HTML5( md_tree_builder );

    auto expected_output_src = test_harness::transcodeInput( test.html_output, path );
    auto exp_tree_builder    = blogator::parser::dom::TreeBuilder();
    auto exp_tokeniser       = blogator::parser::tokeniser::HTML5( exp_tree_builder );

    md2html.init( path );
    md_tokeniser.parse( input_src );

    auto raw_html = md2html.reset();

    md_tree_builder.init( TreeBuilder::createHtmlDocument( U"", path ) );
    md_tree_builder.setStrictChecking( false ); //some tests have some funky tags/attributes
    html5_tokeniser.parse( *raw_html );

    exp_tree_builder.init( TreeBuilder::createHtmlDocument( U"", path ) );
    exp_tree_builder.setStrictChecking( false ); //some tests have some funky tags/attributes
    exp_tokeniser.parse( expected_output_src );

    const auto   output_document    = md_tree_builder.reset();
    const auto   expected_document  = exp_tree_builder.reset();
    const auto   returned_u8        = test_harness::commonmark_spec_tests::printDocumentBody( *output_document ) + "\n";
    const auto   expected_u8        = test_harness::commonmark_spec_tests::printDocumentBody( *expected_document );
    const bool   output_match       = ( returned_u8 == expected_u8 );
    const auto & errors_received    = error_catcher.errors();
    bool         failed_err         = false;

    if( !output_match ) {
        return testing::AssertionFailure()
            << "Failed test #" << test.example_no << "\n"
            << "File ...............: " << test.src.filename().string() << ":" << test.start_line << "\n"
            << "Section ............: " << test.section_desc << "\n"
            << "Input ..............: " << "\n" << test.markdown << "\n"
            << "Expected (raw)......: " << "\n" << test.html_output << "\n"
            << "Expected (processed): " << "\n" << expected_u8 << "\n"
            << "Actual .............: " << "\n" << returned_u8 << "\n"; //TODO
    }

    return testing::AssertionSuccess();
}

class parser_tokeniser_CommonMarkSpec_Tests : public testing::TestWithParam<std::pair<test_harness::commonmark_spec_tests::CommonMarkTest, std::filesystem::path>> {};

TEST_P( parser_tokeniser_CommonMarkSpec_Tests, commonmark_spec_tests) {
    auto test = GetParam();
    EXPECT_TRUE( runMarkdownTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_SUITE_P(
    MarkdownTokeniserTestInstance,
    parser_tokeniser_CommonMarkSpec_Tests,
    ::testing::ValuesIn( test_harness::commonmark_spec_tests::loadMarkdownTests( test_harness::COMMONMARK_SPEC_TEST_PATH ) )
);