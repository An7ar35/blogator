/**
 * This is a test harness to run all the html5-lib tree construction tests from source DAT files
 *
 * Note: this assumes the transcoder and tokeniser are working so if an error pops up on the test
 *       for those deal with them first as it may sort out the problem here!
 *
 * Test subjects:
 * - `blogator::parser::encoding::Transcode`: Source data is run through that first
 * - `blogator::parser::tokeniser::HTML5`   : UTF32 encoded data is then fed through the tokeniser
 * - `blogator::parser::dom::TreeBuilder`   : Finally, the tokens are fed into the tree constructor (main subject of the tests)
 * - `blogator::parser::logging::ParserLog` : where all the parsing errors get sent to (a callback is used to check the actual msgs during testing)
 */

#include "gtest/gtest.h"
#include "../../../src/parser/encoding/Transcode.h"
#include "../../../src/parser/dom/TreeBuilder.h"
#include "../../../src/parser/tokeniser/HTML5.h"
#include "../../../src/parser/logging/ParserLog.h"
#include "../../../src/parser/dom/node/Element.h"

#include "../helpers/helpers.h"

using blogator::parser::dom::TreeBuilder;
using blogator::parser::token::html5::HTML5Tk;
using blogator::parser::logging::ParserLog;

/**
 * Resolves the test prefix into a Namespace
 * @param prefix Prefix string
 * @return Namespace type
 */
blogator::parser::specs::infra::Namespace resolveNamespace( const std::u32string &prefix ) {
    if( prefix == U"svg" ) {
        return blogator::parser::specs::infra::Namespace::SVG;
    } else if( prefix == U"math" ) {
        return blogator::parser::specs::infra::Namespace::MATHML;
    } else {
        return blogator::parser::specs::infra::Namespace::HTML;
    }
}

/**
 * Checks the presence of 'Error::UNKNOWN' or 'Error::NONE' in list of errors
 * @param errors List of errors
 * @return Presence of 'UNKNOWN' or 'NONE' type of errors
 */
bool hasUnknownErrors( const std::vector<blogator::parser::logging::ErrorObject> & errors ) {
    return ( std::find_if( errors.cbegin(),
                           errors.cend(),
                           []( const auto & err ) {
                                return err.errcode() == static_cast<int>( blogator::parser::specs::infra::ErrorCode::NONE )
                                    || err.errcode() == static_cast<int>( blogator::parser::specs::infra::ErrorCode::UNKNOWN );
                           } ) != errors.cend()
    );
}

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
 * [HELPER] Uses Blogator's transcoder to convert the source U8 test input into U32
 * @param raw  Original raw UTF-8 text
 * @param path Source path
 * @return Processed/checked UTF-32 text
 * @throws std::runtime_error when transcoding fails
 */
blogator::parser::U32Text transcodeInput( const std::string & raw, const std::filesystem::path & path ) {
    std::vector<char32_t> out;
    std::stringstream     ss;

    ss << raw;

    auto source = blogator::parser::Source( ss, path, blogator::parser::encoding::Format::UTF8 );

    if( !blogator::parser::encoding::Transcode::convert( source, out ) ) {
        throw std::runtime_error( "Failed to transcode u8 source data to u32" );
    }

    return { path, out };
}

/**
 * Run a test
 * @param test Tree construction '.dat'-formatted test
 * @param path Source file path of the test
 * @return Assert result
 */
testing::AssertionResult runTest( const test_harness::html5lib_tests::TreeConstructionTest &test, const std::filesystem::path &path ) { //TODO
    using namespace blogator::parser::dom;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter_U8toU32;

    ParserLogCatcher error_catcher;

    blogator::parser::logging::ParserLog::attachOutputCallback( [&]( auto err ){ error_catcher.log( err ); } );

    auto src             = transcodeInput( test.data, path );
    auto builder         = blogator::parser::dom::TreeBuilder( test.scripting );
    auto tokeniser       = blogator::parser::tokeniser::HTML5( builder );
    auto context_element = std::unique_ptr<node::Element>();

    if( test.is_fragment ) {
        const auto ns       = resolveNamespace( converter_U8toU32.from_bytes( test.ctx_prefix ) );
        auto       ctx_name = converter_U8toU32.from_bytes( test.ctx_local_name );
        const auto element  = blogator::parser::specs::infra::resolveElementType( ns, ctx_name );

        context_element = std::make_unique<node::Element>( element );

        builder.init( context_element.get() );
    } else {
        builder.init( TreeBuilder::createHtmlDocument( U"", path ) );
    }

    builder.setStrictChecking( false ); //some tests have some funky tags/attributes
    tokeniser.parse( src );

    auto builder_errors = builder.errors();
    auto document       = builder.reset();
    auto returned_u8    = test_harness::html5lib_tests::to_string( *document, test.is_fragment );

    const bool output_match       = ( returned_u8 == test.expected_output );
    const bool error_count_match  = ( error_catcher.count() == test.errors.size() );
    const bool has_unknown_errors = hasUnknownErrors( error_catcher.errors() );

    if( !output_match || has_unknown_errors /* || !error_count_match /* || test.errors.size() > 0 */ ) {
        const auto context = ( !test.ctx_prefix.empty() ? ( test.ctx_prefix + ":" + test.ctx_local_name ) : test.ctx_local_name ) + "\n";

        std::stringstream ss;

        ss << "Failed test (HTML " << ( test.is_fragment ? "Fragment" : "Document" ) << ")\n"
           << "File .............: " << test.src.filename().string() << ":" << test.line << "\n"
           << "Input ............: " << "\n" << test.data << "\n";

        if( test.is_fragment ) {
            ss << "Context ..........: " << context;
        }

        ss << "Scripting ........: " << ( test.scripting ? "true" : "false" ) << "\n";

        if( output_match ) {
            ss << "Output match .....: YES\n";
        } else {
            ss << "Output match .....: NO\n"
               << "Expected .........: " << "\n" << test.expected_output << "\n"
               << "Actual ...........: " << "\n" << returned_u8 << "\n";
        }

        if( builder_errors > 0 || !test.errors.empty() ) {
            ss << "Errors (rcv/exp) .: " << builder_errors << "/" << test.errors.size() << "\n"
               << "Expected errors ..: " << "\n" << test_harness::html5lib_tests::to_string( test.errors ) << "\n"
               << "Received errors ..: " << "\n" << error_catcher;
        } else {
            ss << "Errors (rcv/exp) .: " << builder_errors << "/" << test.errors.size();
        }

        return testing::AssertionFailure() << ss.str();
    }

    return testing::AssertionSuccess();
}

class parser_dom_TreeBuilder_Tests : public testing::TestWithParam<std::pair<test_harness::html5lib_tests::TreeConstructionTest, std::filesystem::path>> {};

TEST_P( parser_dom_TreeBuilder_Tests, html5lib_tests ) {
    auto test = GetParam();
    EXPECT_TRUE( runTest( test.first, test.second ) ) << "File name: " << test.second;
}

INSTANTIATE_TEST_CASE_P(
    TreeBuilderTestInstance,
    parser_dom_TreeBuilder_Tests,
    ::testing::ValuesIn( test_harness::html5lib_tests::loadTreeConstructionTests( test_harness::HTML5LIB_TREECONSTRUCTION_TEST_PATH ) )
);