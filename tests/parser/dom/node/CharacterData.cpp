#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/CharacterData.h"
#include "../../../src/parser/dom/exception/DOMException.h"

#include "../../../src/unicode/utf8.h" //for DOMString_t (u32) -> u8 stream output

using namespace blogator::parser::dom::node;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::exception::DOMException;
using           blogator::parser::dom::exception::DOMExceptionType;
using           blogator::parser::dom::DOMString_t;

TEST( parser_dom_node_CharacterData_Tests, Constructor ) {
    auto txt  = DOMString_t( U"comment string" );
    auto node = CharacterData( NodeType::TEXT_NODE, txt );

    ASSERT_EQ( NodeType::TEXT_NODE, node.nodeType() );
    ASSERT_EQ( node.data(), txt );
}

TEST( parser_dom_node_CharacterData_Tests, data ) {
    auto txt  = DOMString_t( U"comment string" );
    auto node = CharacterData( NodeType::TEXT_NODE, txt );

    ASSERT_EQ( node.data(), txt );
}

TEST( parser_dom_node_CharacterData_Tests, substringData_valid ) {
    struct TestCase {
        const DOMString_t str;
        const size_t      offset;
        const size_t      length;
        const DOMString_t expected;
    };

    auto test_cases = std::vector<TestCase>( {
        { U"",   0, 0, U""   },
        { U"ab", 0, 1, U"a"  },
        { U"ab", 1, 1, U"b"  },
        { U"ab", 0, 2, U"ab" },
    } );

    for( size_t test_id = 0; test_id < test_cases.size(); ++test_id ) {
        const auto & test = test_cases[test_id];
        const auto   node = CharacterData( NodeType::TEXT_NODE, test.str );
        const auto result = node.substringData( test.offset, test.length );

        ASSERT_EQ( result, test.expected )
            << "Failed sub-test-case #" << ( test_id + 1 )  << ": \n"
            << "\texpected: \"" << blogator::unicode::utf8::convert( test.expected ) << "\"\n"
            << "\tgot     : \"" << blogator::unicode::utf8::convert( result ) << "\"\n";
    }
}

TEST( parser_dom_node_CharacterData_Tests, substringData_fail ) {
    using blogator::parser::dom::exception::RangeError;
    const auto txt  = DOMString_t( U"abc" );
    const auto node = CharacterData( NodeType::TEXT_NODE, txt );

    ASSERT_THROW( std::ignore = node.substringData( txt.size() + 1, 1 ), RangeError );
}

TEST( parser_dom_node_CharacterData_Tests, appendData ) {
    struct TestCase {
        const std::string description;
        const DOMString_t init_str;
        const DOMString_t append_str;
        const DOMString_t expected;
    };

    auto test_cases = std::vector<TestCase>( {
        { "Append to nothing", U"", U"abc", U"abc" },
        { "Append w/ nothing", U"abc", U"", U"abc" },
        { "Append normal", U"abc", U"def", U"abcdef" },
    } );

    for( size_t test_id = 0; test_id < test_cases.size(); ++test_id ) {
        const auto & test = test_cases[test_id];
        auto node = CharacterData( NodeType::TEXT_NODE, test.init_str );
        auto returned = node.appendData( test.append_str );

        ASSERT_EQ( returned, test.expected )
            << "Failed sub-test-case #" << ( test_id + 1 )  << ": '" << test.description << "'\n"
            << "\texpected: \"" << blogator::unicode::utf8::convert( test.expected ) << "\"\n"
            << "\tgot     : \"" << blogator::unicode::utf8::convert( returned ) << "\"\n";
    }
}

TEST( parser_dom_node_CharacterData_Tests, insertData_valid ) {
    struct TestCase {
        const std::string description;
        const DOMString_t init_str;
        const DOMString_t insert_str;
        const size_t      offset;
        const DOMString_t expected;
    };

    auto test_cases = std::vector<TestCase>( {
        { "Insert nothing", U"1234", U"", 0, U"1234" },
        { "Insert 1 front", U"1234", U"a", 0, U"a1234" },
        { "Insert 1 back", U"1234", U"a", 4, U"1234a" },
        { "Insert 4 middle", U"1234", U"abcd", 2, U"12abcd34" },
    } );

    for( size_t test_id = 0; test_id < test_cases.size(); ++test_id ) {
        const auto & test = test_cases[test_id];
        auto node = CharacterData( NodeType::TEXT_NODE, test.init_str );
        auto returned = node.insertData( test.offset, test.insert_str );

        ASSERT_EQ( returned, test.expected )
            << "Failed sub-test-case #" << ( test_id + 1 )  << ": '" << test.description << "'\n"
            << "\texpected: \"" << blogator::unicode::utf8::convert( test.expected ) << "\"\n"
            << "\tgot     : \"" << blogator::unicode::utf8::convert( returned ) << "\"\n";
    }
}

TEST( parser_dom_node_CharacterData_Tests, insertData_fail ) {
    using blogator::parser::dom::exception::RangeError;
    const auto txt  = DOMString_t( U"1234" );
    auto node = CharacterData( NodeType::TEXT_NODE, txt );
    ASSERT_THROW( node.insertData( 5, U"abcd" ), RangeError );
}

TEST( parser_dom_node_CharacterData_Tests, deleteData_valid ) {
    struct TestCase {
        const std::string description;
        const DOMString_t init_str;
        const size_t      offset;
        const size_t      count;
        const DOMString_t expected;
    };

    auto test_cases = std::vector<TestCase>( {
        { "delete 0", U"1234", 0, 0, U"1234" },
        { "delete 0 front", U"1234", 0, 0, U"1234" },
        { "delete 0 back", U"1234", 4, 0, U"1234" },
        { "delete 0 mid", U"1234", 2, 0, U"1234" },
        { "delete 1 front", U"1234", 0, 1, U"234" },
        { "delete 2 front", U"1234", 0, 2, U"34" },
        { "delete 4 front", U"1234", 0, 4, U"" },
        { "delete 5 back", U"1234", 0, 5, U"" }, //over-delete
        { "delete 1 back", U"1234", 3, 1, U"123" },
        { "delete 2 back", U"1234", 3, 2, U"123" },
        { "delete 1 mid", U"1234", 2, 1, U"124" },
        { "delete 2 mid", U"1234", 2, 2, U"12" },
    } );

    for( size_t test_id = 0; test_id < test_cases.size(); ++test_id ) {
        const auto & test = test_cases[test_id];
        auto node = CharacterData( NodeType::TEXT_NODE, test.init_str );
        auto returned = node.deleteData( test.offset, test.count );

        ASSERT_EQ( returned, test.expected )
            << "Failed sub-test-case #" << ( test_id + 1 )  << ": '" << test.description << "'\n"
            << "\texpected: \"" << blogator::unicode::utf8::convert( test.expected ) << "\"\n"
            << "\tgot     : \"" << blogator::unicode::utf8::convert( returned ) << "\"\n";
    }
}

TEST( parser_dom_node_CharacterData_Tests, deleteData_fail ) {
    using blogator::parser::dom::exception::RangeError;
    const auto txt  = DOMString_t( U"1234" );
    auto node = CharacterData( NodeType::TEXT_NODE, txt );
    ASSERT_THROW( node.deleteData( 5, 2 ), RangeError );
}

TEST( parser_dom_node_CharacterData_Tests, replaceData_valid ) {
    struct TestCase {
        const std::string description;
        const DOMString_t init_str;
        const DOMString_t replacement;
        const size_t      offset;
        const size_t      length;
        const DOMString_t expected;
    };

    auto test_cases = std::vector<TestCase>( {
        { "Delete 0", U"1234", U"", 0, 0, U"1234" },
        { "Delete 1", U"1234", U"", 0, 1, U"234" },
        { "Delete 2", U"1234", U"", 0, 2, U"34" },
        { "Delete 3", U"1234", U"", 0, 3, U"4" },
        { "Delete 4", U"1234", U"", 0, 4, U"" },
        { "Delete 5", U"1234", U"", 0, 5, U"" },
        { "Delete 2 w/ offset 1", U"1234", U"", 1, 2, U"14" },
        { "Delete 2 w/ offset max", U"1234", U"", 4, 2, U"1234" },
        { "Offset 0", U"1234", U"abc", 0, 3, U"abc4" },
        { "Offset 1", U"1234", U"abc", 1, 3, U"1abc" },
        { "Offset 2", U"1234", U"abc", 2, 3, U"12abc" },
        { "Offset 3", U"1234", U"abc", 3, 3, U"123abc" },
        { "Offset 4", U"1234", U"abc", 4, 3, U"1234abc" },
        { "Length 0", U"1234", U"abc", 0, 0, U"abc1234" },
        { "Length 1", U"1234", U"abc", 0, 1, U"abc234" },
        { "Length 2", U"1234", U"abc", 0, 2, U"abc34" },
        { "Length 3", U"1234", U"abc", 0, 3, U"abc4" },
        { "Length 4", U"1234", U"abc", 0, 4, U"abc" },
    } );

    for( size_t test_id = 0; test_id < test_cases.size(); ++test_id ) {
        const auto & test = test_cases[test_id];
        auto node = CharacterData( NodeType::TEXT_NODE, test.init_str );
        auto returned = node.replaceData( test.offset, test.length, test.replacement );

        ASSERT_EQ( returned, test.expected )
            << "Failed sub-test-case #" << ( test_id + 1 )  << ": '" << test.description << "'\n"
            << "\texpected: \"" << blogator::unicode::utf8::convert( test.expected ) << "\"\n"
            << "\tgot     : \"" << blogator::unicode::utf8::convert( returned ) << "\"\n";
    }
}

TEST( parser_dom_node_CharacterData_Tests, replaceData_fail ) {
    using blogator::parser::dom::exception::RangeError;
    const auto txt  = DOMString_t( U"1234" );
    auto node = CharacterData( NodeType::TEXT_NODE, txt );
    ASSERT_THROW( node.replaceData( 5, 3, U"abc" ), RangeError );
}

TEST( parser_dom_node_CharacterData_Tests, insertBefore_fail_0 ) { //parent not a Document, DocumentFragment, or Element node
    auto parent = std::make_unique<CharacterData>( NodeType::TEXT_NODE, U"data" );
    auto insert = std::make_unique<Node>();

    try {
        dynamic_cast<Node *>( parent.get() )->insertBefore( insert, nullptr );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType.";
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_CharacterData_Tests, swap_0 ) {
    auto node1 = CharacterData( NodeType::TEXT_NODE, U"node1" );
    auto node2 = CharacterData( NodeType::TEXT_NODE, U"node2" );

    blogator::parser::dom::node::swap( dynamic_cast<Node &>( node1 ), dynamic_cast<Node &>( node2 ) );

    ASSERT_EQ( node1.data(), U"node2" );
    ASSERT_EQ( node2.data(), U"node1" );
}

TEST( parser_dom_node_CharacterData_Tests, swap_1 ) {
    auto node1 = CharacterData( NodeType::TEXT_NODE, U"node1" );
    auto node2 = CharacterData( NodeType::TEXT_NODE, U"node2" );

    blogator::parser::dom::node::swap( node1, node2 );

    ASSERT_EQ( node1.data(), U"node2" );
    ASSERT_EQ( node2.data(), U"node1" );
}

TEST( parser_dom_node_CharacterData_Tests, output_stream ) {
    const auto comment_str = DOMString_t( U"this is a string\non multiple lines\n..." );
    const auto node        = CharacterData( NodeType::TEXT_NODE, comment_str );

    std::stringstream expected_ss;
    expected_ss << "this is a string\non multiple lines\n...";

    std::stringstream received_ss;
    received_ss << node;

    ASSERT_EQ( received_ss.str(), expected_ss.str() );
}