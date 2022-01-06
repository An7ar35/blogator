#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/CharacterData.h"

#include "../../../src/unicode/utf8.h" //for DOMString_t (u32) -> u8 stream output

using namespace blogator::parser::dom::node;
using           blogator::parser::dom::NodeType;
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

    for( auto test_id = 0; test_id < test_cases.size(); ++test_id ) {
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

    for( auto test_id = 0; test_id < test_cases.size(); ++test_id ) {
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

    for( auto test_id = 0; test_id < test_cases.size(); ++test_id ) {
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

    for( auto test_id = 0; test_id < test_cases.size(); ++test_id ) {
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

    for( auto test_id = 0; test_id < test_cases.size(); ++test_id ) {
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

TEST( parser_dom_node_CharacterData_Tests, length ) {
    auto txt  = DOMString_t( U"comment string" );
    auto node = CharacterData( NodeType::TEXT_NODE, txt );

    ASSERT_EQ( node.length(), txt.length() );
}

TEST( parser_dom_node_CharacterData_Tests, cloneNode ) {
    std::vector<DOMString_t> strings = { U"A", U"AA", U"AB", U"AC", U"ACA" };

    auto original = CharacterData( NodeType::TEXT_NODE, strings[0] );
    original.appendChild( std::make_unique<CharacterData>( NodeType::TEXT_NODE, strings[1] ) );
    original.appendChild( std::make_unique<CharacterData>( NodeType::TEXT_NODE, strings[2] ) );
    auto * child = original.appendChild( std::make_unique<CharacterData>( NodeType::TEXT_NODE, strings[3] ) );
    child->appendChild( std::make_unique<CharacterData>( NodeType::TEXT_NODE, strings[4] ) );

    //[CharacterData] 'A'
    //  |-[CharacterData] 'AA'
    //  |-[CharacterData] 'AB'
    //  |-[CharacterData] 'AC'
    //      |-[CharacterData] 'ACA'

    auto clone = original.cloneNode( true );

    ASSERT_EQ( original.childNodes().size(), 3 );
    ASSERT_EQ( clone->childNodes().size(), original.childNodes().size() );
    const auto * recast_clone = dynamic_cast<CharacterData *>( clone.get() );
    ASSERT_EQ( recast_clone->data(), original.data() );

    auto * origi_child_it = dynamic_cast<CharacterData *>( original.firstChild() );
    auto * clone_child_it = dynamic_cast<CharacterData *>( clone->firstChild() );

    for( int i = 0; i < original.childNodes().size(); ++i ) {
        ASSERT_EQ( origi_child_it->nodeType(), clone_child_it->nodeType() );
        ASSERT_NE( origi_child_it, clone_child_it ); //make sure ptr addresses are different
        ASSERT_EQ( origi_child_it->data(), clone_child_it->data() );

        if( i < original.childNodes().size() - 1 ) {
            origi_child_it = dynamic_cast<CharacterData *>( origi_child_it->nextSibling() );
            clone_child_it = dynamic_cast<CharacterData *>( clone_child_it->nextSibling() );
        }
    }

    ASSERT_EQ( origi_child_it->childNodes().size(), 1 );
    ASSERT_EQ( origi_child_it->childNodes().size(),
               clone_child_it->childNodes().size() );
    ASSERT_EQ( origi_child_it->firstChild()->nodeType(),
               clone_child_it->firstChild()->nodeType() );
    ASSERT_EQ( dynamic_cast<CharacterData *>( origi_child_it->firstChild() )->data(),
               dynamic_cast<CharacterData *>( clone_child_it->firstChild() )->data() );
}