#include "helper.h"

#include <fstream>
#include <iostream>

/**
 * Converts any 'uXXXX' unicode literal into a string representation 'U+XXXX'
 * @param str String to process
 * @return Processed string
 */
std::string blogator::tests::unicodeToRepresentation( const std::string & str ) {
    std::stringstream out;

    enum class State {
        CHARACTER,
        ESCAPED,
        DOUBLE_ESCAPED,
    };

    State state = State::CHARACTER;
    auto  it    = str.cbegin();

    while( it != str.cend() ) {
        switch( state ) {
            case State::CHARACTER: {
                switch( *it ) {
                    case '\?': { out << "U+003F"; } break;
                    case '\a': { out << "U+0007"; } break;
                    case '\b': { out << "U+0008"; } break;
                    case '\f': { out << "U+000C"; } break;
                    case '\n': { out << "U+000A"; } break;
                    case '\r': { out << "U+000D"; } break;
                    case '\t': { out << "U+0009"; } break;
                    case '\v': { out << "U+000B"; } break;
                    case '\\': { state = State::ESCAPED; } break;
                    case '\0': { out << "U+0000"; } break;
                    default  : { out << *it; } break;
                }
            } break;

            case State::ESCAPED: {
                switch( *it ) {
                    case '0': {
                        out << "U+0";
                        state = State::CHARACTER;
                    } break;

                    case '?': {
                        out << "U+003F";
                        state = State::CHARACTER;
                    } break;

                    case 'a': {
                        out << "U+0007";
                        state = State::CHARACTER;
                    } break;

                    case 'b': {
                        out << "U+0008";
                        state = State::CHARACTER;
                    } break;

                    case 'f': {
                        out << "U+000C";
                        state = State::CHARACTER;
                    } break;

                    case 'n': {
                        out << "U+000A";
                        state = State::CHARACTER;
                    } break;

                    case 'r': {
                        out << "U+000D";
                        state = State::CHARACTER;
                    } break;

                    case 't': {
                        out << "U+0009";
                        state = State::CHARACTER;
                    } break;

                    case 'v': {
                        out << "U+000B";
                        state = State::CHARACTER;
                    } break;

                    case 'u': { //'\u'
                        out << "U+";
                        state = State::CHARACTER;
                    } break;

                    case '\\': { //U+005C;
                        state = State::DOUBLE_ESCAPED;
                    } break;

                    case '\'': [[fallthrough]];
                    case '\"': [[fallthrough]];
                    default: {
                        out << *it;
                        state = State::CHARACTER;
                    } break;
                }
            } break;

            case State::DOUBLE_ESCAPED: {
                state = State::CHARACTER;
            } break;
        }

        std::advance( it, 1 );
    }

    return out.str();
}

std::u32string blogator::tests::unescape( const std::u32string &str ) {
    std::vector<char32_t> buffer;

    auto addToCharRefCode = []( uint32_t & crf, uint32_t c ) -> bool {
        crf *= 16;

        if( unicode::ascii::isdigit( c ) ) {
            crf += ( c - 0x0030 );
        } else if( unicode::ascii::islxdigit( c ) ) {
            crf += ( c - 0x0037 );
        } else if( unicode::ascii::isuxdigit( c ) ) {
            crf += ( c - 0x0057 );
        } else {
            return false;
        }

        return true;
    };

    bool escape = false;
    auto it     = str.cbegin();

    while( it != str.cend() ) {
        if( escape ) {
            uint32_t unicode = 0;

            while( it != str.cend() && addToCharRefCode( unicode, *it ) ) {
                ++it;
            }

            buffer.emplace_back( unicode );
            escape = false;

        } else {
            if( *it == '\\' && *std::next( it ) == 'u' ) {
                escape = true;
                std::advance( it, 2 ); //ignore '\\u'

            } else {
                buffer.emplace_back( *it );
                std::advance( it, 1 );
            }
        }
    }

    return { buffer.cbegin(), buffer.cend() };
}


std::string blogator::tests::unescape( const std::string &u8str ) {
    enum class State {
        CHARACTER,
        SINGLE_ESCAPE,
        DOUBLE_ESCAPE,
        UNICODE_REF,
        UNICODE_REF_END,
    };

    std::stringstream ss;
    auto     it              = u8str.begin();
    State    state           = State::CHARACTER;
    uint32_t hex_ref         = 0;
    int      char_to_consume = 4;

    while( it != u8str.cend() ) {
        size_t bytes = blogator::unicode::utf8::bytes( *it );

        switch( state ) {
            case State::CHARACTER: {
                if( *it == '\\' ) {
                    state = State::SINGLE_ESCAPE;
                    std::advance( it, 1 );
                } else {
                    while( it != u8str.cend() && ( bytes-- ) > 0 ) {
                        ss << *it;
                        std::advance( it, 1 );
                    }
                }
            } break;

            case State::SINGLE_ESCAPE: {
                if( *it == '\\' ) {
                    state = State::DOUBLE_ESCAPE;
                    std::advance( it, 1 );
                } else {
                    ss << "\\";
                    state = State::CHARACTER;
                }
            } break;

            case State::DOUBLE_ESCAPE: {
                if( *it == 'u' ) {
                    state = State::UNICODE_REF;
                    std::advance( it, 1 );
                } else {
                    ss << "\\\\";
                    state = State::CHARACTER;
                }
            } break;

            case State::UNICODE_REF: {
                hex_ref         = 0; //reset
                char_to_consume = 4; //reset

                while( it != u8str.cend() && char_to_consume > 0 ) {
                    if( unicode::ascii::isdigit( *it ) ) {
                        hex_ref *= 16;
                        hex_ref += ( *it - 0x30 );
                        std::advance( it, 1 );
                    } else if( unicode::ascii::isuxdigit( *it ) ) {
                        hex_ref *= 16;
                        hex_ref += ( *it - 0x0037 );
                        std::advance( it, 1 );
                    } else if( unicode::ascii::islxdigit( *it ) ) {
                        hex_ref *= 16;
                        hex_ref += ( *it - 0x0057 );
                        std::advance( it, 1 );
                    } else {
                        break;
                    }

                    --char_to_consume;
                }

                if( char_to_consume < 4 ) {
                    state = State::UNICODE_REF_END;
                } else {
                    ss << "\\u"; //reconsume as characters
                    state = State::CHARACTER;
                }

            } break;

            case State::UNICODE_REF_END: {
                if( hex_ref == 0 ) {
                    ss << "\\u0000";
                } else {
                    unicode::utf8::convert( ss, hex_ref );
                }

                state = State::CHARACTER;
            }
        }
    }

    return ss.str();
}


std::u32string blogator::tests::escape( const std::u32string &str ) {
    std::vector<char32_t> buffer;

    for( char32_t c : str) {
        if( unicode::utf32::isnonchar( c ) || unicode::utf32::issurrogate( c ) ) {
            auto hex = unicode::utf32::toxunicode( c );
            buffer.insert( buffer.end(), hex.cbegin(), hex.cend() );
        } else {
            buffer.emplace_back( c );
        }
    }

    return { buffer.cbegin(), buffer.cend() };
}

/**
 * JSON-ify a collection of HTML5 tokens into a stream
 * @param os Output stream
 * @param tokens Tokens
 * @return Output stream
 */
std::ostream & blogator::tests::jsonifyHtml5Tokens( std::ostream &os, const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> &tokens ) {
    os << "[";

    for( auto it = tokens.cbegin(); it != tokens.cend(); ++ it ) {
            os << *( *it );
        if( std::next( it ) != tokens.cend() ) {
            os << ", ";
        }
    }

    os << "]";
    return os;
}

/**
 * JSON-ify a collection of HTML5 error objects into a stream
 * @param os Output stream
 * @param err Error objects
 * @return Output stream
 */
std::ostream & blogator::tests::jsonifyHtml5Errors( std::ostream & os, const std::vector<blogator::parser::logging::ErrorObject> &err ) {
    os << "[";

    for( auto it = err.cbegin(); it != err.cend(); ++ it ) {
        auto code = it->error();
        auto pos  = it->textpos();
        std::replace( code.begin(), code.end(), ' ', '-' );
        std::transform( code.begin(), code.end(), code.begin(), []( auto c ) { return std::tolower( c ); } );
        os << R"({ "code": ")" << code << R"(", "line": )" << pos.line << ",  \"col\": " << pos.col << " }";
        if( std::next( it ) != err.cend() ) {
            os << ", ";
        }
    }

    os << "]";
    return os;
}

/**
 * Gets all '.test' files found in a directory path
 * @param test_dir Directory path
 * @return Collection of test file paths
 */
std::vector<std::filesystem::path> blogator::tests::getTestFiles( const std::filesystem::path &test_dir ) {
    auto test_files = std::vector<std::filesystem::path>();

    for (const std::filesystem::path & file : std::filesystem::directory_iterator( test_dir ) ) {
        if( file.extension() == ".test" ) {
            test_files.push_back( file );
        }
    }

    return test_files;
}

/**
 * Loads a json file into a JSON object
 * @param path Source filepath
 * @return JSON
 */
nlohmann::json blogator::tests::loadJSON( const std::filesystem::path &path ) {
    std::cout << "Loading test file: " << path << std::endl;
    std::fstream file;
    file.open( path, std::ios::in );
    auto json = nlohmann::json();
    file >> json;
    file.close();
    return json;
}

/**
 * Loads and add all tests from found files
 * @param test_dir Directory path
 * @return Collection of { test, source path }
 */
std::vector<std::pair<nlohmann::json, std::filesystem::path>> blogator::tests::loadJSONTests( const std::filesystem::path &test_dir ) {
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> test_collection;

    auto test_files = getTestFiles( test_dir );

    for( const auto & file : test_files ) {
        auto source = loadJSON( file );

        if( !source.contains( "tests" ) ) {
            std::cerr << "Can't find root key 'tests' in source json file: " << file << std::endl;
        } else {
            size_t tests = 0;

            for( const auto & test : source.at( "tests" ) ) {
                test_collection.emplace_back( std::make_pair( test, file ) );
                ++tests;
            }

            std::cout << "> Found " << tests << " tests." << std::endl;
        }
    }

    std::cout << "> TOTAL TESTS FOUND: " << test_collection.size() << " (" << test_dir << ")" << std::endl;

    return test_collection;
}