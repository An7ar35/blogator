#include "harness.h"

#include <fstream>

#include "../../src/string/helpers.h"
#include "../../src/unicode/ascii.h"
#include "../../src/unicode/utf8.h"
#include "../../src/parser/dom/node/Attr.h"
#include "../../src/parser/dom/node/CDATASection.h"
#include "../../src/parser/dom/node/Comment.h"
#include "../../src/parser/dom/node/Document.h"
#include "../../src/parser/dom/node/DocumentFragment.h"
#include "../../src/parser/dom/node/DocumentType.h"
#include "../../src/parser/dom/node/Element.h"
#include "../../src/parser/dom/node/Text.h"

#include "../../helpers/helpers.h"

using namespace test_harness::html5lib_tests;

//local methods pre-declaration
static TreeConstructionTestCollection_t loadTreeConstructionDat( const std::filesystem::path &path );

/**
 * Stringify a Document's DOM tree in the HTML5Lib format
 * @param document Root document node of DOM tree
 * @param fragment Flag to print as a fragment (i.e.: children of <html> node)
 * @return Output string (utf-8)
 */
std::string test_harness::html5lib_tests::to_string( blogator::parser::dom::node::Document & document, bool fragment ) {
    using           blogator::unicode::utf8::convert;
    using namespace blogator::parser::dom::node;

    static const auto NS_DESIGNATOR = std::unordered_map<blogator::parser::specs::infra::Namespace, std::string>( {
        { blogator::parser::specs::infra::Namespace::HTML, "" },
        { blogator::parser::specs::infra::Namespace::SVG, "svg" },
        { blogator::parser::specs::infra::Namespace::MATHML, "math" },
        { blogator::parser::specs::infra::Namespace::UNKNOWN, "unknown" },
        { blogator::parser::specs::infra::Namespace::OTHER, "other" },
    } );

    std::stringstream ss;

    auto printIndent = [&ss, &fragment]( size_t depth ) {
        ss << "| ";

        if( fragment && depth > 0 ) {
            --depth;
        }

        for( size_t i = 1; i < depth; ++i ) {
            ss << "  ";
        }
    };

    auto it = document.begin();

    if( fragment ) {
        while( it->nodeType() != blogator::parser::dom::NodeType::ELEMENT_NODE ||
               dynamic_cast<Element *>( it.node() )->elementType() != blogator::parser::specs::infra::Element::HTML5_HTML )
        {
            ++it;
        }

        ++it; //for the first child if <html>
    }

    while( it != document.end() ) {
        switch( it->nodeType() ) {
            case blogator::parser::dom::NodeType::ELEMENT_NODE: {
                const auto * node  = dynamic_cast<const Element *>( it.node() );
                const auto   depth = node->depth();
                const auto   ns    = node->getNamespaceEnum();

                printIndent( depth );
                switch( ns ) {
                    case blogator::parser::specs::infra::Namespace::MATHML: [[fallthrough]];
                    case blogator::parser::specs::infra::Namespace::SVG:    [[fallthrough]];
                    case blogator::parser::specs::infra::Namespace::OTHER:  [[fallthrough]];
                    case blogator::parser::specs::infra::Namespace::UNKNOWN: {
                        ss << "<" << NS_DESIGNATOR.at( ns ) << " " << convert( node->qualifiedName() ) << ">\n";
                    } break;

                    case blogator::parser::specs::infra::Namespace::XLINK: [[fallthrough]];
                    case blogator::parser::specs::infra::Namespace::XML:   [[fallthrough]];
                    case blogator::parser::specs::infra::Namespace::XMLNS: [[fallthrough]];
                    default: {
                        ss << "<" << convert( node->qualifiedName() ) << ">\n";
                    } break;
                }

                //Note: In the `html5lib` tests, if the namespace is not none (i.e.: not a URI of "", so essentially SVG and MathML),
                //      the attribute prefix and name are separated by a space and otherwise by a ':'.
                //      (ref: https://github.com/html5lib/html5lib-python/blob/ad3bde76737e07b0369ac64c1dd2231e857e2baa/html5lib/treebuilders/dom.py#L181)
                switch( ns ) {
                    case blogator::parser::specs::infra::Namespace::MATHML: [[fallthrough]];
                    case blogator::parser::specs::infra::Namespace::SVG: {
                        for( auto attr_it = node->attributes().cbegin(); attr_it != node->attributes().cend(); ++attr_it ) {
                            printIndent( depth + 1 );

                            if( !attr_it->second->prefix().empty() ) {
                                ss << convert( attr_it->second->prefix() ) << " ";
                            }

                            ss << convert( attr_it->second->name() ) << "=\""
                               << ( attr_it->second->hasValue() ? convert(  *attr_it->second->value() ) : "" ) << "\"\n";
                        }
                    } break;

                    default: {
                        for( auto attr_it = node->attributes().cbegin(); attr_it != node->attributes().cend(); ++attr_it ) {
                            printIndent( depth + 1 );
                            ss << convert( attr_it->second->nodeName() ) << "=\""
                               << ( attr_it->second->hasValue() ? convert(  *attr_it->second->value() ) : "" ) << "\"\n";
                        }
                    } break;
                }
            } break;

            case blogator::parser::dom::NodeType::TEXT_NODE: {
                const auto * node  = dynamic_cast<const Text *>( it.node() );

                printIndent( node->depth() );
                ss << "\"" << convert( node->data() ) << "\"\n";
            } break;

            case blogator::parser::dom::NodeType::COMMENT_NODE: {
                const auto * node  = dynamic_cast<const Comment *>( it.node() );

                printIndent( node->depth() );
                ss << "<!-- " << convert( node->data() ) << " -->\n";
            } break;

            case blogator::parser::dom::NodeType::DOCUMENT_NODE: {
                //ss << "#document\n";
            } break;

            case blogator::parser::dom::NodeType::DOCUMENT_TYPE_NODE: {
                const auto * node  = dynamic_cast<const DocumentType *>( it.node() );

                printIndent( node->depth() );
                ss << "<!DOCTYPE " << convert( node->name() );

                if( !node->systemId().empty() || !node->publicId().empty() ) {
                    ss << " \"" << convert( node->publicId() ) << "\" "
                       << "\"" << convert( node->systemId() ) << "\"";
                }

                ss << ">\n";
            } break;

            case blogator::parser::dom::NodeType::DOCUMENT_FRAGMENT_NODE: {
                const auto * node  = dynamic_cast<const DocumentFragment *>( it.node() );

                printIndent( node->depth() );
                ss << "content\n";
            } break;

            case blogator::parser::dom::NodeType::CDATA_SECTION_NODE:     [[fallthrough]];
            default: { ss << "!!! Unexpected node type (" << it->nodeType() << ")\n"; } break;
        }

        ++it;
    }

    return ss.str();
}

/**
 * JSON-ify a collection of HTML5 tokens into a stream
 * @param os Output stream
 * @param tokens Tokens
 * @return Output stream
 */
std::ostream & test_harness::html5lib_tests::jsonifyHtml5Tokens( std::ostream &os, const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> &tokens ) {
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
 * JSON-ify a collection of error objects into a stream
 * @param os Output stream
 * @param err Error objects
 * @return Output stream
 */
std::ostream & test_harness::html5lib_tests::jsonifyErrorObjects( std::ostream & os, const std::vector<blogator::reporter::ReporterObject> &err ) {
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

std::u32string test_harness::html5lib_tests::unescape( const std::u32string &str ) {
    std::vector<char32_t> buffer;

    auto addToCharRefCode = []( uint32_t & crf, uint32_t c ) -> bool {
        crf *= 16;

        if( blogator::unicode::ascii::isdigit( c ) ) {
            crf += ( c - 0x0030 );
        } else if( blogator::unicode::ascii::islxdigit( c ) ) {
            crf += ( c - 0x0037 );
        } else if( blogator::unicode::ascii::isuxdigit( c ) ) {
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


std::string test_harness::html5lib_tests::unescape( const std::string &u8str ) {
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
        size_t bytes = blogator::unicode::utf8::bytelength( *it );

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
                    if( blogator::unicode::ascii::isdigit( *it ) ) {
                        hex_ref *= 16;
                        hex_ref += ( *it - 0x30 );
                        std::advance( it, 1 );
                    } else if( blogator::unicode::ascii::isuxdigit( *it ) ) {
                        hex_ref *= 16;
                        hex_ref += ( *it - 0x0037 );
                        std::advance( it, 1 );
                    } else if( blogator::unicode::ascii::islxdigit( *it ) ) {
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
                    blogator::unicode::utf8::convert( ss, hex_ref );
                }

                state = State::CHARACTER;
            }
        }
    }

    return ss.str();
}

/**
 * Loads and add all tests from found files
 * @param test_dir Directory path
 * @return Collection of { test, source path }
 */
std::vector<std::pair<nlohmann::json, std::filesystem::path>> test_harness::html5lib_tests::loadJSONTests( const std::filesystem::path &test_dir ) {
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> test_collection;

    auto test_files = getTestFiles( test_dir, ".test" );

    for( const auto & file : test_files ) {
        auto source = test_harness::loadJSONFromFile( file );

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

/**
 * Loads and add all tests from found files
 * @param test_dir Directory path
 * @return Collection of { test, source path }
 */
TreeConstructionTests_t test_harness::html5lib_tests::loadTreeConstructionTests( const std::filesystem::path &test_dir ) { //TODO
    TreeConstructionTests_t test_collection;

    auto test_files = getTestFiles( test_dir, ".dat" );

    for( const auto & file : test_files ) {
        auto source = loadTreeConstructionDat( file );

        if( source.empty() ) {
            std::cerr << "No tests found in source 'dat' file: " << file << std::endl;
        } else {
            for( const auto & test : source ) {
                test_collection.emplace_back( std::make_pair( test, file ) );
            }

            std::cout << "> Found " << source.size() << " tests." << std::endl;
        }
    }

    std::cout << "> TOTAL TESTS FOUND: " << test_collection.size() << " (" << test_dir << ")" << std::endl;

    return test_collection;
}

/**
 * Loads a tree construction 'dat' file into a collection of tests
 * @param path Source filepath
 * @return TreeConstructionTests_t collection
 */
TreeConstructionTestCollection_t loadTreeConstructionDat( const std::filesystem::path &path ) { //TODO
    using test_harness::html5lib_tests::TreeConstructionTest;

    static constexpr uint8_t SCRIPTING_FLAG_NONE       = { 0b00 };
    static constexpr uint8_t SCRIPTING_FLAG_ON         = { 0b10 };
    static constexpr uint8_t SCRIPTING_FLAG_OFF        = { 0b01 };
    static constexpr uint8_t SCRIPTING_FLAG_ON_AND_OFF = { 0b11 };

    enum State {
        READY,
        DATA,
        ERRORS,
        NEW_ERRORS,        //(optional)
        DOCUMENT_FRAGMENT, //(optional)
        SCRIPT_OFF,        //(optional)
        SCRIPT_ON,         //(optional)
        DOCUMENT,
    } state = READY;

    static const auto STATE_MAPPING = std::unordered_map<std::string, State>( {
        { "#data", DATA },
        { "#errors", ERRORS },
        { "#new-errors", NEW_ERRORS },
        { "#document-fragment", DOCUMENT_FRAGMENT },
        { "#script-off", SCRIPT_OFF },
        { "#script-on", SCRIPT_ON },
        { "#document", DOCUMENT },
    } );

    auto test_collection = TreeConstructionTestCollection_t();

    std::cout << "Loading test file: " << path << std::endl;
    std::fstream file;
    file.open( path, std::ios::in );

    std::string line;

    size_t  test_id       = 0;
    size_t  line_no       = 1;
    auto    test          = std::unique_ptr<TreeConstructionTest>();
    uint8_t scripting     = SCRIPTING_FLAG_NONE;
    size_t  sequential_lf = 0;

    /**
     * [LAMBDA] Dispatches the current test to the collection
     */
    const auto dispatchTest = [&]() {
        test->id  = test_id++;
        test->src = path;

        if( scripting == SCRIPTING_FLAG_NONE ) { // no scripting specified so do both on/off
            scripting = SCRIPTING_FLAG_ON_AND_OFF;
        }

        if( scripting == SCRIPTING_FLAG_ON ) { //if test is solely with scripting on, skip as not implemented.
            std::cerr << "> Test #" << test_id << " (line " << test->line << ") ignored as only for scripting enabled." << std::endl;

        } else {
            if( scripting & SCRIPTING_FLAG_ON ) {
                auto & ref = test_collection.emplace_back( *test );
                ref.scripting = true;
            }

            if( scripting & SCRIPTING_FLAG_OFF ) {
                auto & ref = test_collection.emplace_back( *test );
                ref.scripting = false;
            }
        }

        //reset
        test.reset();
        scripting = SCRIPTING_FLAG_NONE;
    };

    while( std::getline( file, line ) ) {
        if( line.front() == '#' ) {
            const auto it = STATE_MAPPING.find( line );

            if( it != STATE_MAPPING.end() ) {
                state = it->second;

                switch( state ) { //single liner actions
                    case DATA: {
                        if( test ) {
                            dispatchTest();
                        }

                        test       = std::make_unique<TreeConstructionTest>();
                        test->line = line_no;
                    } break;

                    case SCRIPT_OFF: { scripting |= SCRIPTING_FLAG_OFF; } break;
                    case SCRIPT_ON:  { scripting |= SCRIPTING_FLAG_ON;  } break;
                    case DOCUMENT:   { sequential_lf = 0;               } break;
                    default: break;
                };
            } else {
                std::cerr << "[test_harness::html5lib_tests::loadTreeConstructionDat( " << path << " )] "
                          << "Key (#) not recognised: " << line << std::endl;

                state = READY;
            }

        } else {
            switch( state ) {
                case DATA: {
                    if( !test->data.empty() ) {
                        test->data.append( "\n" );
                    }

                    test->data.append( line.begin(), line.end() );
                } break;

                case ERRORS:     [[fallthrough]];
                case NEW_ERRORS: {
                    test->errors.emplace_back( line );
                } break;

                case DOCUMENT_FRAGMENT: {
                    test->is_fragment = true;
                    auto words = blogator::string::split<char>( line, ' ', true );

                    if( words.size() == 1 ) {
                        test->ctx_local_name = words[0];

                    } else if( words.size() == 2 ) {
                        test->ctx_prefix     = words[0];
                        test->ctx_local_name = words[1];

                    } else {
                        std::cerr << "[test_harness::html5lib_tests::loadTreeConstructionDat( " << path << " )] "
                                  << "'#document-fragment' section not parsed: \"" << line << std::endl;
                    }
                } break;

                case DOCUMENT: {
                    if( line.empty() ) {
                        ++sequential_lf;
                    } else {
                        if( sequential_lf > 0 ) {
                            for( size_t i = 0; i <  sequential_lf; ++i ) {
                                test->expected_output += "\n";
                            }

                            sequential_lf = 0;
                        }

                        test->expected_output += ( line + "\n" );
                    }
                } break;

                default: break;
            }
        }

        ++line_no;
    }

    if( test ) {
        dispatchTest();
    }

    file.close();
    return test_collection;
}