#include "gtest/gtest.h"
#include "../../../src/parser/dom/filter/NodeFilter.h"

#include <vector>
#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/CDATASection.h"
#include "../../../src/parser/dom/node/Comment.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/DocumentFragment.h"
#include "../../../src/parser/dom/node/DocumentType.h"
#include "../../../src/parser/dom/node/Element.h"
#include "../../../src/parser/dom/node/Text.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeFilter;

TEST( parser_dom_NodeFilter_Tests, whatToShow ) {
    struct TestCase {
        std::string   description;
        unsigned long input;
        unsigned long expected;
    };

    auto tests = std::vector<TestCase>( {
        { "SHOW_ALL", NodeFilter::SHOW_ALL, 0xFFFFFFFF },
        { "SHOW_ELEMENT", NodeFilter::SHOW_ELEMENT, 0x1 },
        { "SHOW_ATTRIBUTE", NodeFilter::SHOW_ATTRIBUTE, 0x2 },
        { "SHOW_TEXT", NodeFilter::SHOW_TEXT, 0x4 },
        { "SHOW_CDATA_SECTION", NodeFilter::SHOW_CDATA_SECTION, 0x8 },
        { "SHOW_COMMENT", NodeFilter::SHOW_COMMENT, 0x80 },
        { "SHOW_DOCUMENT", NodeFilter::SHOW_DOCUMENT, 0x100 },
        { "SHOW_DOCUMENT_TYPE", NodeFilter::SHOW_DOCUMENT_TYPE, 0x200 },
        { "SHOW_DOCUMENT_FRAGMENT", NodeFilter::SHOW_DOCUMENT_FRAGMENT, 0x400 },
    } );

    for( const auto & test : tests ) {
        auto filter = NodeFilter( test.input );
        ASSERT_EQ( filter.whatToShow(), test.expected ) << "Failed on '" << test.description << "'.";
    }
}

TEST( parser_dom_NodeFilter_Tests, validate ) {
    auto attr      = std::make_unique<node::Attr>( U"", U"" );
    auto cdata     = std::make_unique<node::CDATASection>( U"" );
    auto comment   = std::make_unique<node::Comment>( U"" );
    auto doc       = std::make_unique<node::Document>();
    auto doc_frag  = std::make_unique<node::DocumentFragment>();
    auto doc_type  = std::make_unique<node::DocumentType>( U"", U"", U"" );
    auto element   = std::make_unique<node::Element>( blogator::parser::specs::html5::Element::HTML5_DIV );
    auto text      = std::make_unique<node::Text>( U"" );
    auto undefined = std::make_unique<node::Node>();

    struct TestCase {
        std::string   description;
        unsigned long input_mask;
        node::Node *  input_node;
        bool          expected;
    };

    auto tests = std::vector<TestCase>( {
        { "SHOW_ALL/Attr", NodeFilter::SHOW_ALL, attr.get(), true },
        { "SHOW_ALL/CDATASection", NodeFilter::SHOW_ALL, cdata.get(), true },
        { "SHOW_ALL/Comment", NodeFilter::SHOW_ALL, comment.get(), true },
        { "SHOW_ALL/Document", NodeFilter::SHOW_ALL, doc.get(), true },
        { "SHOW_ALL/DocumentFragment", NodeFilter::SHOW_ALL, doc_frag.get(), true },
        { "SHOW_ALL/DocumentType", NodeFilter::SHOW_ALL, doc_type.get(), true },
        { "SHOW_ALL/Element", NodeFilter::SHOW_ALL, element.get(), true },
        { "SHOW_ALL/Text", NodeFilter::SHOW_ALL, text.get(), true },
        { "SHOW_ALL/Node", NodeFilter::SHOW_ALL, undefined.get(), false },

        { "SHOW_ALL/Attr", NodeFilter::SHOW_ATTRIBUTE, attr.get(), true },
        { "SHOW_ALL/CDATASection", NodeFilter::SHOW_CDATA_SECTION, cdata.get(), true },
        { "SHOW_ALL/Comment", NodeFilter::SHOW_COMMENT, comment.get(), true },
        { "SHOW_ALL/Document", NodeFilter::SHOW_DOCUMENT, doc.get(), true },
        { "SHOW_ALL/DocumentFragment", NodeFilter::SHOW_DOCUMENT_FRAGMENT, doc_frag.get(), true },
        { "SHOW_ALL/DocumentType", NodeFilter::SHOW_DOCUMENT_TYPE, doc_type.get(), true },
        { "SHOW_ALL/Element", NodeFilter::SHOW_ELEMENT, element.get(), true },
        { "SHOW_ALL/Text", NodeFilter::SHOW_TEXT, text.get(), true },

        { "SHOW_ALL/Attr", NodeFilter::SHOW_TEXT, attr.get(), false },
        { "SHOW_ALL/CDATASection", NodeFilter::SHOW_ELEMENT, cdata.get(), false },
        { "SHOW_ALL/Comment", NodeFilter::SHOW_DOCUMENT_TYPE, comment.get(), false },
        { "SHOW_ALL/Document", NodeFilter::SHOW_DOCUMENT_FRAGMENT, doc.get(), false },
        { "SHOW_ALL/DocumentFragment", NodeFilter::SHOW_DOCUMENT, doc_frag.get(), false },
        { "SHOW_ALL/DocumentType", NodeFilter::SHOW_COMMENT, doc_type.get(), false },
        { "SHOW_ALL/Element", NodeFilter::SHOW_CDATA_SECTION, element.get(), false },
        { "SHOW_ALL/Text", NodeFilter::SHOW_ATTRIBUTE, text.get(), false },
        { "SHOW_ALL/Node", NodeFilter::SHOW_TEXT, undefined.get(), false },
    } );

    for( const auto & test : tests ) {
        auto filter = NodeFilter( test.input_mask );
        ASSERT_EQ( filter.validate( *test.input_node ), test.expected ) << "Failed on '" << test.description << "'.";
    }
}

TEST( parser_dom_NodeFilter_Tests, acceptNode_0 ) { //no callback
    auto text    = std::make_unique<node::Text>( U"text" );
    auto comment = std::make_unique<node::Comment>( U"text" );

    auto filter = NodeFilter( NodeFilter::SHOW_COMMENT );

    ASSERT_EQ( filter.acceptNode( *text ), NodeFilter::Result::FILTER_SKIP );
    ASSERT_EQ( filter.acceptNode( *comment ), NodeFilter::Result::FILTER_ACCEPT );
}

TEST( parser_dom_NodeFilter_Tests, acceptNode_1 ) { //with callback
    auto text     = std::make_unique<node::Text>( U"" );
    auto comment1 = std::make_unique<node::Comment>( U"text" );
    auto comment2 = std::make_unique<node::Comment>( U"some-text" );

    auto filter = NodeFilter( NodeFilter::SHOW_COMMENT,
                              []( const node::Node & node ) {
                                                    return node.textContent() == U"text"
                                                           ? NodeFilter::Result::FILTER_ACCEPT
                                                           : NodeFilter::Result::FILTER_REJECT;
    } );

    ASSERT_EQ( filter.acceptNode( *text ), NodeFilter::Result::FILTER_SKIP );
    ASSERT_EQ( filter.acceptNode( *comment1 ), NodeFilter::Result::FILTER_ACCEPT );
    ASSERT_EQ( filter.acceptNode( *comment2 ), NodeFilter::Result::FILTER_REJECT );
}