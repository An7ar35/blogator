#include "harness.h"

#include <iostream>

#include "../../../src/unicode/utf8.h"
#include "../../helpers/helpers.h"
#include "../../../src/parser/dom/node/Element.h"

/**
 * Loads a 'commonmark-spec' json test file into a collection of tests
 * @param test_dir Source filepath
 * @return CommonMarkTest collection
 */
std::vector<std::pair<test_harness::commonmark_spec_tests::CommonMarkTest, std::filesystem::path>> test_harness::commonmark_spec_tests::loadMarkdownTests( const std::filesystem::path &test_dir ) {
    static const auto FIELDS = std::vector<std::string>( {
        "example",
        "start_line",
        "end_line",
        "markdown",
        "html",
        "section"
    } );

    std::vector<std::pair<test_harness::commonmark_spec_tests::CommonMarkTest, std::filesystem::path>> test_collection;

    auto   test_files      = getTestFiles( test_dir, ".json" );
    size_t malformed_tests = 0;

    for( const auto & file : test_files ) {
        auto   source    = test_harness::loadJSONFromFile( file );
        size_t tests     = 0;

        if( !source.is_array() ) {
            std::cerr << "Unexpected JSON format (need root array) in: " << file.string() << std::endl;

        } else {
            for( const auto &test: source ) {
                auto err = false;
                auto obj = CommonMarkTest();

                for( const auto & field : FIELDS ) {
                    if( !test.contains( field ) ) {
                        std::cerr << "ERROR: field \"" << field << "\" not found for test " << tests << " in " << file.string() << std::endl;
                        err = true;
                        ++malformed_tests;
                        break;
                    }
                }

                if( !err ) {
                    obj.src = file;
                    obj.example_no = test.at( "example" ).get<unsigned>();
                    obj.start_line = test.at( "start_line" ).get<unsigned>();
                    obj.end_line = test.at( "end_line" ).get<unsigned>();
                    obj.markdown = test.at( "markdown" );
                    obj.html_output = test.at( "html" );
                    obj.section_desc = test.at( "section" );

                    test_collection.emplace_back( std::make_pair( std::move( obj ), file ) );

                    ++tests;
                }
            }

            std::cout << "> Found " << tests << " tests." << std::endl;
        }
    }

    std::cout << "> TOTAL TESTS FOUND: " << test_collection.size() << " (" << test_dir << ")" << std::endl;

    if( malformed_tests > 0 ) {
        std::cerr << "> MALFORMED TESTS FOUND: " << malformed_tests << " (" << test_dir << ")" << std::endl;
    }

    return test_collection;
}

/**
 * Prints an HTML document <body> content
 * @param document Document root
 * @return HTML content of the <body> tag
 */
std::string test_harness::commonmark_spec_tests::printDocumentBody( blogator::parser::dom::node::Document &document ) {
    using           blogator::unicode::utf8::convert;
    using namespace blogator::parser::dom::node;

    std::stringstream ss;

    auto body_elements = document.getElementsByTagName( U"body" );

    if( !body_elements.empty() ) {
        const auto & children = body_elements.at( 0 )->childNodes();

        for( const auto & c : children ) {
            ss << *c;
        }
    }

    return ss.str();
}