#include "harness.h"

#include "../../helpers/helpers.h"
#include "../../src/unicode/utf8.h"
#include "../../../src/parser/dom/node/Element.h"

/**
 * Pretty print a MarkdownTk to match the format of the "TokenDescription" struct
 * @param token MarkdownTk instance
 * @return String
 */
std::string test_harness::markdown::to_string( const blogator::parser::token::markdown::MarkdownTk & token ) {
    std::stringstream ss;
    ss << token;
    return ss.str();
}

/**
 * Pretty print a ErrorObject to match the format of the "ErrorDescription" struct
 * @param token ErrorObject instance
 * @return String
 */
std::string test_harness::markdown::to_string( const blogator::reporter::ReporterObject & error ) {
    std::stringstream ss;
    ss << "{ description: \"" << error.error() << "\", line: " << error.textpos().line << ", col: " << error.textpos().col << " }";
    return ss.str();
}

/**
 * Loads a 'markdown' json test file into a collection of tests
 * @param test_dir Source filepath
 * @return CommonMarkTest collection
 */
std::vector<std::pair<test_harness::markdown::MarkdownTkTest, std::filesystem::path>> test_harness::markdown::loadTokeniserTests( const std::filesystem::path &test_dir ) {
    static const auto FIELDS = std::vector<std::string>( {
        "description",
        "input",
        "tokens",
        "errors",
    } );

    static const auto TOKEN_FIELDS = std::vector<std::string>( {
        "type",
    } );

    static const auto ERROR_FIELDS = std::vector<std::string>( {
        "description",
        "line",
        "col",
    } );

    std::vector<std::pair<test_harness::markdown::MarkdownTkTest, std::filesystem::path>> test_collection;

    auto   test_files     = getTestFiles( test_dir, ".json" );
    size_t missing_fields = 0;

    for( const auto & file : test_files ) {
        auto   source = test_harness::loadJSONFromFile( file );
        size_t tests  = 0;

        if( !source.contains( "tests" ) ) {
            std::cerr << "Unexpected JSON format (need root 'tests' array) in: " << file.string() << std::endl;

        } else {
            for( const auto & test: source.at( "tests" ) ) {
                auto err = false;
                auto obj = MarkdownTkTest();

                for( const auto & field : FIELDS ) {
                    if( !test.contains( field ) ) {
                        std::cerr << "ERROR: field \"" << field << "\" not found for test " << tests << " in " << file.string() << std::endl;
                        err = true;
                        ++missing_fields;
                        break;
                    }
                }

                for( const auto & token : test.at( "tokens" ) ) {
                    for( const auto &tk_fields: TOKEN_FIELDS ) {
                        if( !token.contains( tk_fields ) ) {
                            std::cerr << "ERROR: token field \"" << tk_fields << "\" not found for test " << tests << " in " << file.string() << std::endl;
                            err = true;
                            ++missing_fields;
                            break;
                        }
                    }
                }

                for( const auto & token : test.at( "errors" ) ) {
                    for( const auto &tk_fields: ERROR_FIELDS ) {
                        if( !token.contains( tk_fields ) ) {
                            std::cerr << "ERROR: error field \"" << tk_fields << "\" not found for test " << tests << " in " << file.string() << std::endl;
                            err = true;
                            ++missing_fields;
                            break;
                        }
                    }
                }

                if( !err ) {
                    obj.src         = file;
                    obj.description = test.at( "description" );
                    obj.input       = test.at( "input" );

                    for( const auto & token : test.at( "tokens" ) ) {
                        auto & desc = obj.tokens.emplace_back( token );
                    }

                    for( const auto & error : test.at( "errors" ) ) {
                        obj.errors.emplace_back( ErrorDescription { error.at( "description" ), error.at( "line" ), error.at( "col" ) } );
                    }

                    test_collection.emplace_back( std::make_pair( std::move( obj ), file ) );

                    ++tests;
                }
            }

            std::cout << "> Found " << tests << " tests." << std::endl;
        }
    }

    std::cout << "> TOTAL TESTS FOUND: " << test_collection.size() << " (" << test_dir << ")" << std::endl;

    if( missing_fields > 0 ) {
        std::cerr << "> MISSING FIELDS IN TESTS (SKIPPED) FOUND: " << missing_fields << " (" << test_dir << ")" << std::endl;
    }

    return test_collection;
}

/**
 * Loads a 'markdown' json test file into a collection of tests
 * @param test_dir Source filepath
 * @return MarkdownToHtmlTest collection
 */
std::vector<std::pair<test_harness::markdown::MarkdownToHtmlTest, std::filesystem::path>> test_harness::markdown::loadTreeBuilderTests( const std::filesystem::path &test_dir ) {
    static const auto FIELDS = std::vector<std::string>( {
        "description",
        "markdown",
        "html",
        "errors",
    } );

    static const auto ERROR_FIELDS = std::vector<std::string>( {
        "description",
        "line",
        "col",
    } );

    std::vector<std::pair<test_harness::markdown::MarkdownToHtmlTest, std::filesystem::path>> test_collection;

    auto   test_files     = getTestFiles( test_dir, ".json" );
    size_t missing_fields = 0;

    for( const auto & file : test_files ) {
        auto   source = test_harness::loadJSONFromFile( file );
        size_t tests  = 0;

        if( !source.contains( "tests" ) ) {
            std::cerr << "Unexpected JSON format (need root 'tests' array) in: " << file.string() << std::endl;

        } else {
            for( const auto & test: source.at( "tests" ) ) {
                auto err = false;
                auto obj = MarkdownToHtmlTest();

                for( const auto & field : FIELDS ) {
                    if( !test.contains( field ) ) {
                        std::cerr << "ERROR: field \"" << field << "\" not found for test " << tests << " in " << file.string() << std::endl;
                        err = true;
                        ++missing_fields;
                        break;
                    }
                }

                for( const auto & token : test.at( "errors" ) ) {
                    for( const auto &tk_fields: ERROR_FIELDS ) {
                        if( !token.contains( tk_fields ) ) {
                            std::cerr << "ERROR: error field \"" << tk_fields << "\" not found for test " << tests << " in " << file.string() << std::endl;
                            err = true;
                            ++missing_fields;
                            break;
                        }
                    }
                }

                if( !err ) {
                    obj.id          = ++tests;
                    obj.src         = file;
                    obj.description = test.at( "description" );
                    obj.markdown    = test.at( "markdown" );
                    obj.html_output = test.at( "html" );

                    for( const auto & error : test.at( "errors" ) ) {
                        obj.errors.emplace_back( ErrorDescription { error.at( "description" ), error.at( "line" ), error.at( "col" ) } );
                    }

                    test_collection.emplace_back( std::make_pair( std::move( obj ), file ) );
                }
            }

            std::cout << "> Found " << tests << " tests." << std::endl;
        }
    }

    std::cout << "> TOTAL TESTS FOUND: " << test_collection.size() << " (" << test_dir << ")" << std::endl;

    if( missing_fields > 0 ) {
        std::cerr << "> MISSING FIELDS IN TESTS (SKIPPED) FOUND: " << missing_fields << " (" << test_dir << ")" << std::endl;
    }

    return test_collection;
}

/**
 * Prints an HTML document <body> content
 * @param document Document root
 * @return HTML content of the <body> tag
 */
std::string test_harness::markdown::printDocumentBody( blogator::parser::dom::node::Document &document ) {
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