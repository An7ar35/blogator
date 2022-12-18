#include "harness.h"

#include <iostream>

#include "../../helpers/helpers.h"
#include "../../../src/configuration/dto/Configuration.h"

/**
 * Pretty print a ErrorObject to match the format of the "ErrorDescription" struct
 * @param token ErrorObject instance
 * @return String
 */
std::string test_harness::blogator::to_string( const ::blogator::reporter::ReporterObject & error ) {
    std::stringstream ss;
    ss << "{ description: \"" << error.error() << "\", line: " << error.textpos().line << ", col: " << error.textpos().col << " }";
    return ss.str();
}

/**
 * Loads a 'blogator configuration' json test file into a collection of tests
 * @param test_dir Source filepath
 * @return ConfigurationTkTest collection
 */
std::vector<std::pair<test_harness::blogator::ConfigurationTkTest, std::filesystem::path>> test_harness::blogator::loadConfigTokeniserTests( const std::filesystem::path &test_dir ) {
    static const auto FIELDS = std::vector<std::string>( {
        "description",
        "input",
        "tokens",
        "errors",
    } );

    static const auto TOKEN_FIELDS = std::vector<std::string>( {
        "type",
        "text",
        "position",
    } );

    static const auto ERROR_FIELDS = std::vector<std::string>( {
        "description",
        "line",
        "col",
    } );

    std::vector<std::pair<test_harness::blogator::ConfigurationTkTest, std::filesystem::path>> test_collection;

    auto   test_files     = getTestFiles( test_dir, ".json" );
    size_t missing_fields = 0;

    for( const auto & file : test_files ) {
        auto   source = test_harness::loadJSONFromFile( file );
        size_t tests  = 0;

        if( !source.contains( "tests" ) ) {
            std::cerr << "Unexpected JSON format (need root 'tests' array) in: " << file.string() << std::endl;

        } else {
            for( const auto & test : source.at( "tests" ) ) {
                auto err = false;
                auto obj = ConfigurationTkTest();

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
                        std::ignore = obj.tokens.emplace_back( token );
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
 * Loads a 'blogator configuration' json test file into a collection of tests
 * @param test_dir Source filepath
 * @return ConfigurationParserTest collection
 */
std::vector<std::pair<test_harness::blogator::ConfigurationParserTest, std::filesystem::path>> test_harness::blogator::loadConfigParserTests( const std::filesystem::path &test_dir ) {
    static const auto FIELDS = std::vector<std::string>( {
        "description",
        "input",
        "entries",
        "errors",
    } );

    static const auto ENTRY_FIELDS = std::vector<std::string>( {
        "key",
        "value",
    } );

    static const auto ERROR_FIELDS = std::vector<std::string>( {
        "description",
        "line",
        "col",
    } );

    std::vector<std::pair<test_harness::blogator::ConfigurationParserTest, std::filesystem::path>> test_collection;

    auto   test_files     = getTestFiles( test_dir, ".json" );
    size_t missing_fields = 0;

    for( const auto & file : test_files ) {
        auto   source = test_harness::loadJSONFromFile( file );
        size_t tests  = 0;

        if( !source.contains( "tests" ) ) {
            std::cerr << "Unexpected JSON format (need root 'tests' array) in: " << file.string() << std::endl;

        } else {
            for( const auto & test : source.at( "tests" ) ) {
                auto err = false;
                auto obj = ConfigurationParserTest();

                for( const auto & field : FIELDS ) {
                    if( !test.contains( field ) ) {
                        std::cerr << "ERROR: field \"" << field << "\" not found for test " << tests << " in " << file.string() << std::endl;
                        err = true;
                        ++missing_fields;
                        break;
                    }
                }

                for( const auto & token : test.at( "entries" ) ) {
                    for( const auto &tk_fields: ENTRY_FIELDS ) {
                        if( !token.contains( tk_fields ) ) {
                            std::cerr << "ERROR: entry field \"" << tk_fields << "\" not found for test " << tests << " in " << file.string() << std::endl;
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

                    for( const auto & token : test.at( "entries" ) ) {
                        obj.entries.emplace_back( token );
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
 * Format configuration into a JSON formatted array (`[ { "key": "...", "value": "..." }, ... ]`)
 * @param os Output stream
 * @param cfg Configuration object
 * @return Output stream
 */
std::ostream & test_harness::blogator::jsonifyConfiguration( std::ostream & os, const ::blogator::configuration::Configuration &cfg ) {
    const auto kv_collection = cfg.printToCollection( { ::blogator::configuration::Value::Type_e::NAME } );

    os << "[";

    size_t i = 0;
    for( const auto & [key, value] : kv_collection ) {
        os << R"({"key": ")" << key << R"(", "value": )" << value << R"(})";

        if( ++i != kv_collection.size() ) {
            os << ", ";
        }
    }

    os << "]";

    return os;
}

/**
 * Format configuration into a JSON array containing key-value pairs
 * @param cfg Configuration object
 * @return JSON array
 */
nlohmann::json test_harness::blogator::jsonifyConfiguration( const ::blogator::configuration::Configuration &cfg ) {
    std::stringstream ss;
    jsonifyConfiguration( ss, cfg );
    return nlohmann::json::parse( ss.str() );
}
