/**
 * This is a test harness to run all the MarkdownToHtml tests from source JSON files
 *
 * Test subjects:
 * - `blogator::parser::tokeniser::Markdown`: Input is pipelined to this to get to MarkdownToHtml
 * - `blogator::parser::builder::MarkdownToHtml`: main subject of the tests
 * - `blogator::parser::tokeniser::HTML5`: Output of MarkdownToHtml is fed into the HTML pipeline to deal with the tree construction
 * - `blogator::parser::dom::TreeBuilder`: ^^^ (see above)
 * - `blogator::reporter::ParserLog`: where all the parsing errors get sent to (a callback is used to check the actual msgs during testing)
 */
#include "gtest/gtest.h"
#include "../../../src/reporter/ParseReporter.h"
#include "../../../src/parser/dom/TreeBuilder.h"
#include "../../../src/parser/tokeniser/HTML5.h"
#include "../../../src/parser/tokeniser/Markdown.h"

#include "test-harness-sources.h"
#include "../helpers/helpers.h"
#include "harness/harness.h"

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
testing::AssertionResult runMarkdownTreeBuilderTest( const test_harness::markdown::MarkdownToHtmlTest &test, const std::filesystem::path &path ) {
    //TODO design tests

    using namespace blogator::parser::dom;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher error_catcher;

    blogator::reporter::ParseReporter::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    auto src             = test_harness::transcodeInput( test.markdown, path );
    auto md2html         = blogator::parser::builder::MarkdownToHtml();
    auto md_tokeniser    = blogator::parser::tokeniser::Markdown( md2html );
    auto tree_builder    = blogator::parser::dom::TreeBuilder();
    auto html5_tokeniser = blogator::parser::tokeniser::HTML5( tree_builder );

    md2html.init( path );
    md_tokeniser.parse( src );

    auto raw_html = md2html.reset();

    std::string pre_html5_tokeniser_html; //raw HTML passed from the MarkdownToHTML builder to the HTML5 pipeline
    {
        std::stringstream ss;
        test_harness::printU32Buffer( ss, *raw_html );
        pre_html5_tokeniser_html = ss.str();
    }

    tree_builder.init( TreeBuilder::createHtmlDocument( U"", path ) );
    tree_builder.setStrictChecking( false ); //some tests have some funky tags/attributes
    html5_tokeniser.parse( *raw_html );

    const auto   document        = tree_builder.reset();
    const auto   returned_u8     = test_harness::markdown::printDocumentBody( *document );
    const bool   output_match    = ( returned_u8 == test.html_output );
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

    if( !output_match || failed_err ) {
        std::stringstream ss;

        ss << "Failed test #" << test.id << ")\n"
           << "File .............: " << test.src.filename().string() << "\n"
           << "Description ......: " << test.description << "\n"
           << "Input ............: " << test.markdown << "\n";

        if( output_match ) {
            ss << "Output match .....: YES\n";
        } else {
            ss << "Output match .....: NO\n"
               << "Pre-HTML tokenizer: " << "\n" << pre_html5_tokeniser_html << "\n"
               << "Expected .........: " << "\n" << test.html_output << "\n"
               << "Actual ...........: " << "\n" << returned_u8 << "\n";
        }

        std::stringstream err_message;

        if( failed_err ) {
            ss << "Errors ...........: " << errors_received.size() << "/" << test.errors.size() << "\n"
               << "\treceived:\n";

            for( const auto & err : errors_received ) {
                ss << "\t\t" << test_harness::markdown::to_string( err ) << "\n";
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

class parser_builder_MarkdownTreeBuilder_Tests : public testing::TestWithParam<std::pair<test_harness::markdown::MarkdownToHtmlTest, std::filesystem::path>> {};

TEST_P( parser_builder_MarkdownTreeBuilder_Tests, markdown_tree_builder_tests ) {
    auto test = GetParam();
    EXPECT_TRUE( runMarkdownTreeBuilderTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_SUITE_P(
    MarkdownTreeBuilderTestInstance,
    parser_builder_MarkdownTreeBuilder_Tests,
    ::testing::ValuesIn( test_harness::markdown::loadTreeBuilderTests( test_harness::MARKDOWN_TREECONSTRUCTION_TEST_PATH ) )
);