#include "gtest/gtest.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/DocumentType.h"
#include "../../../src/parser/dom/node/Element.h"

#include "../../../TestHelpers/TestDOMs.h"
#include "../../../src/string/helpers.h"

using namespace blogator::parser::dom;
using           blogator::parser::dom::NodeType;
using           blogator::parser::dom::DOMString_t;
using           blogator::parser::specs::infra::Namespace;
using           blogator::parser::specs::infra::ContentType;

TEST( parser_dom_node_Document_Tests, constructor ) { //default
    //since a fair amount of tests relies on a default being HTML
    auto doc = node::Document();
    ASSERT_EQ( doc.type(), node::Document::Type::HTML );
    ASSERT_EQ( doc.quirksMode(), QuirksMode::NO_QUIRKS );
}

TEST( parser_dom_node_Document_Tests, copy_constructor_0 ) {
    auto charset   = U"UTF-8";
    auto url       = DOMString_t( U"https://www.my.site.com" );
    auto path      = std::filesystem::path( "/file/path/" );
    auto name      = U"test-name";
    auto public_id = U"test-public-id";
    auto system_id = U"test-system-id";
    auto doc       = node::Document( charset, ContentType::APPLICATION_RSS_XML, url, path, QuirksMode::LIMITED_QUIRKS );
    doc.appendChild( std::make_unique<node::DocumentType>( name, public_id, system_id ) );
    doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );

    auto copy = node::Document( doc );

    ASSERT_EQ( copy.characterSet(), doc.characterSet() );
    ASSERT_EQ( copy.contentType(), doc.contentType() );
    ASSERT_EQ( copy.URL(), doc.URL() );
    ASSERT_EQ( copy.filepath(), doc.filepath() );
    ASSERT_EQ( copy.type(), doc.type() );
    ASSERT_EQ( copy.quirksMode(), doc.quirksMode() );
    ASSERT_NE( copy.doctype(), nullptr );
    ASSERT_EQ( copy.doctype()->name(), name );
    ASSERT_EQ( copy.doctype()->publicId(), public_id );
    ASSERT_EQ( copy.doctype()->systemId(), system_id );
    ASSERT_NE( copy.documentElement(), nullptr );
    ASSERT_EQ( copy.documentElement()->elementType(), blogator::parser::specs::infra::Element::HTML5_HTML );
}

TEST( parser_dom_node_Document_Tests, move_constructor_0 ) {
    auto charset   = U"UTF-8";
    auto url       = DOMString_t( U"https://www.my.site.com" );
    auto path      = std::filesystem::path( "/file/path/" );
    auto name      = U"test-name";
    auto public_id = U"test-public-id";
    auto system_id = U"test-system-id";
    auto doc       = node::Document( charset, ContentType::APPLICATION_RSS_XML, url, path, QuirksMode::LIMITED_QUIRKS );
    doc.appendChild( std::make_unique<node::DocumentType>( name, public_id, system_id ) );
    doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );

    auto moved = node::Document( std::move( doc ) );

    ASSERT_EQ( moved.characterSet(), charset );
    ASSERT_EQ( moved.contentType(), U"application/rss+xml" );
    ASSERT_EQ( moved.URL(), url );
    ASSERT_EQ( moved.filepath(), path );
    ASSERT_EQ( moved.type(), node::Document::Type::XML );
    ASSERT_EQ( moved.quirksMode(), QuirksMode::LIMITED_QUIRKS );
    ASSERT_NE( moved.doctype(), nullptr );
    ASSERT_EQ( moved.doctype()->name(), name );
    ASSERT_EQ( moved.doctype()->publicId(), public_id );
    ASSERT_EQ( moved.doctype()->systemId(), system_id );
    ASSERT_NE( moved.documentElement(), nullptr );
    ASSERT_EQ( moved.documentElement()->elementType(), blogator::parser::specs::infra::Element::HTML5_HTML );
}

TEST( parser_dom_node_Document_Tests, copy_operator_0 ) {
    auto charset   = U"UTF-8";
    auto url       = DOMString_t( U"https://www.my.site.com" );
    auto path      = std::filesystem::path( "/file/path/" );
    auto name      = U"test-name";
    auto public_id = U"test-public-id";
    auto system_id = U"test-system-id";
    auto doc       = node::Document( charset, ContentType::APPLICATION_RSS_XML, url, path, QuirksMode::LIMITED_QUIRKS );
    doc.appendChild( std::make_unique<node::DocumentType>( name, public_id, system_id ) );
    doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );

    auto copy = node::Document();
    copy = doc;

    ASSERT_EQ( copy.characterSet(), doc.characterSet() );
    ASSERT_EQ( copy.contentType(), doc.contentType() );
    ASSERT_EQ( copy.URL(), doc.URL() );
    ASSERT_EQ( copy.filepath(), doc.filepath() );
    ASSERT_EQ( copy.type(), doc.type() );
    ASSERT_EQ( copy.quirksMode(), doc.quirksMode() );
    ASSERT_NE( copy.doctype(), nullptr );
    ASSERT_EQ( copy.doctype()->name(), name );
    ASSERT_EQ( copy.doctype()->publicId(), public_id );
    ASSERT_EQ( copy.doctype()->systemId(), system_id );
    ASSERT_NE( copy.documentElement(), nullptr );
    ASSERT_EQ( copy.documentElement()->elementType(), blogator::parser::specs::infra::Element::HTML5_HTML );
}

TEST( parser_dom_node_Document_Tests, move_operator_0 ) {
    auto charset   = U"UTF-8";
    auto url       = DOMString_t( U"https://www.my.site.com" );
    auto path      = std::filesystem::path( "/file/path/" );
    auto name      = U"test-name";
    auto public_id = U"test-public-id";
    auto system_id = U"test-system-id";
    auto doc       = node::Document( charset, ContentType::APPLICATION_RSS_XML, url, path, QuirksMode::LIMITED_QUIRKS );
    doc.appendChild( std::make_unique<node::DocumentType>( name, public_id, system_id ) );
    doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );

    auto moved = node::Document();
    moved = std::move( doc );

    ASSERT_EQ( moved.characterSet(), charset );
    ASSERT_EQ( moved.contentType(), U"application/rss+xml" );
    ASSERT_EQ( moved.URL(), url );
    ASSERT_EQ( moved.filepath(), path );
    ASSERT_EQ( moved.type(), node::Document::Type::XML );
    ASSERT_EQ( moved.quirksMode(), QuirksMode::LIMITED_QUIRKS );
    ASSERT_NE( moved.doctype(), nullptr );
    ASSERT_EQ( moved.doctype()->name(), name );
    ASSERT_EQ( moved.doctype()->publicId(), public_id );
    ASSERT_EQ( moved.doctype()->systemId(), system_id );
    ASSERT_NE( moved.documentElement(), nullptr );
    ASSERT_EQ( moved.documentElement()->elementType(), blogator::parser::specs::infra::Element::HTML5_HTML );
}

TEST( parser_dom_node_Document_Tests, swap_0 ) { //as Node
    auto charset_1      = U"UTF-8";
    auto content_type_1 = ContentType::APPLICATION_RSS_XML;
    auto url_1          = DOMString_t( U"https://www.my.site1.com" );
    auto path_1         = std::filesystem::path( "/file/path/1/" );
    auto quirks_1       = QuirksMode::LIMITED_QUIRKS;
    auto doc_1          = node::Document( charset_1, content_type_1, url_1, path_1, quirks_1 );

    auto charset_2      = U"UTF-16";
    auto content_type_2 = ContentType::APPLICATION_XHTML_XML;
    auto url_2          = DOMString_t( U"https://www.my.site2.com" );
    auto path_2         = std::filesystem::path( "/file/path/2/" );
    auto quirks_2       = QuirksMode::NO_QUIRKS;
    auto doc_2          = node::Document( charset_2, content_type_2, url_2, path_2, quirks_2 );

    blogator::parser::dom::node::swap( dynamic_cast<node::Node &>( doc_1 ), dynamic_cast<node::Node &>( doc_2 ) );

    ASSERT_EQ( doc_1.characterSet(), charset_2 );
    ASSERT_EQ( doc_1.contentType(), U"application/xhtml+xml" );
    ASSERT_EQ( doc_1.URL(), url_2 );
    ASSERT_EQ( doc_1.filepath(), path_2 );
    ASSERT_EQ( doc_1.type(), node::Document::Type::HTML );
    ASSERT_EQ( doc_1.quirksMode(), quirks_2 );

    ASSERT_EQ( doc_2.characterSet(), charset_1 );
    ASSERT_EQ( doc_2.contentType(), U"application/rss+xml" );
    ASSERT_EQ( doc_2.URL(), url_1 );
    ASSERT_EQ( doc_2.filepath(), path_1 );
    ASSERT_EQ( doc_2.type(), node::Document::Type::XML );
    ASSERT_EQ( doc_2.quirksMode(), quirks_1 );
}

TEST( parser_dom_node_Document_Tests, swap_1 ) { //as Document
    auto charset_1      = U"UTF-8";
    auto content_type_1 = ContentType::APPLICATION_RSS_XML;
    auto url_1          = DOMString_t( U"https://www.my.site1.com" );
    auto path_1         = std::filesystem::path( "/file/path/1/" );
    auto quirks_1       = QuirksMode::LIMITED_QUIRKS;
    auto doc_1          = node::Document( charset_1, content_type_1, url_1, path_1, quirks_1 );

    auto charset_2      = U"UTF-16";
    auto content_type_2 = ContentType::APPLICATION_XHTML_XML;
    auto url_2          = DOMString_t( U"https://www.my.site2.com" );
    auto path_2         = std::filesystem::path( "/file/path/2/" );
    auto quirks_2       = QuirksMode::NO_QUIRKS;
    auto doc_2          = node::Document( charset_2, content_type_2, url_2, path_2, quirks_2 );

    blogator::parser::dom::node::swap( doc_1, doc_2 );

    ASSERT_EQ( doc_1.characterSet(), charset_2 );
    ASSERT_EQ( doc_1.contentType(), U"application/xhtml+xml" );
    ASSERT_EQ( doc_1.URL(), url_2 );
    ASSERT_EQ( doc_1.filepath(), path_2 );
    ASSERT_EQ( doc_1.type(), node::Document::Type::HTML );
    ASSERT_EQ( doc_1.quirksMode(), quirks_2 );

    ASSERT_EQ( doc_2.characterSet(), charset_1 );
    ASSERT_EQ( doc_2.contentType(), U"application/rss+xml" );
    ASSERT_EQ( doc_2.URL(), url_1 );
    ASSERT_EQ( doc_2.filepath(), path_1 );
    ASSERT_EQ( doc_2.type(), node::Document::Type::XML );
    ASSERT_EQ( doc_2.quirksMode(), quirks_1 );
}

TEST( parser_dom_node_Document_Tests, type ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.type(), node::Document::Type::HTML );
}

TEST( parser_dom_node_Document_Tests, filepath ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.filepath(), "/file/path/" );
}

TEST( parser_dom_node_Document_Tests, quirksMode ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.quirksMode(), QuirksMode::NO_QUIRKS );
}

TEST( parser_dom_node_Document_Tests, setQuirks ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    doc.setQuirks( QuirksMode::LIMITED_QUIRKS );
    ASSERT_EQ( doc.quirksMode(), QuirksMode::LIMITED_QUIRKS );
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_0 ) { //HTML doc, by name
    auto   doc     = node::Document();
    auto * element = doc.createDocumentElement( U"Div" );

    ASSERT_EQ( doc.childNodes().size(), 1 );
    ASSERT_EQ( doc.childNodes()[0].get(), element );
    ASSERT_EQ( doc.documentElement(), element );
    ASSERT_EQ( element->localName(), U"div" );
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_1 ) { //XML doc, by name
    auto   doc     = node::Document( U"", ContentType::APPLICATION_XML, U"", "", node::Document::Type::XML, QuirksMode::NO_QUIRKS );
    auto * element = doc.createDocumentElement( U"Div" );

    ASSERT_EQ( doc.childNodes().size(), 1 );
    ASSERT_EQ( doc.childNodes()[0].get(), element );
    ASSERT_EQ( doc.documentElement(), element );
    ASSERT_EQ( element->localName(), U"Div" );
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_2 ) { //HTML doc, by Element type enum
    auto   doc     = node::Document();
    auto * element = doc.createDocumentElement( blogator::parser::specs::infra::Element::HTML5_DIV );

    ASSERT_EQ( doc.childNodes().size(), 1 );
    ASSERT_EQ( doc.childNodes()[0].get(), element );
    ASSERT_EQ( doc.documentElement(), element );
    ASSERT_EQ( element->localName(), U"div" );
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_3 ) { //XML doc, by Element type enum
    auto   doc     = node::Document( U"", ContentType::APPLICATION_XML, U"", "", node::Document::Type::XML, QuirksMode::NO_QUIRKS );
    auto * element = doc.createDocumentElement( blogator::parser::specs::infra::Element::HTML5_DIV );

    ASSERT_EQ( doc.childNodes().size(), 1 );
    ASSERT_EQ( doc.childNodes()[0].get(), element );
    ASSERT_EQ( doc.documentElement(), element );
    ASSERT_EQ( element->localName(), U"div" );
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_fail_0 ) { //by name 2x
    auto   doc     = node::Document();
    doc.createDocumentElement( U"Div" );

    try {
        doc.createDocumentElement( U"span" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_fail_1 ) { //by Element type enum x2
    auto   doc     = node::Document();
    doc.createDocumentElement( blogator::parser::specs::infra::Element::HTML5_DIV );

    try {
        doc.createDocumentElement( blogator::parser::specs::infra::Element::HTML5_DIV );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::HierarchyRequestError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createDocumentElement_fail_3 ) { //invalid name
    auto doc = node::Document();

    try {
        doc.createDocumentElement( U"sp@n" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, url ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.URL(), U"https://www.my.site.com" );
}

TEST( parser_dom_node_Document_Tests, documentURI ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.documentURI(), U"https://www.my.site.com" );
}

TEST( parser_dom_node_Document_Tests, compatMode ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.compatMode(), U"CSS1Compat" );
}

TEST( parser_dom_node_Document_Tests, characterSet ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.characterSet(), U"UTF-8" );
}

TEST( parser_dom_node_Document_Tests, contentType ) {
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XHTML_XML,
                               U"https://www.my.site.com",
                               "/file/path/",
                               node::Document::Type::HTML,
                               QuirksMode::NO_QUIRKS );

    ASSERT_EQ( doc.contentType(), U"application/xhtml+xml" );
}

TEST( parser_dom_node_Document_Tests, doctype ) {
    auto   doc     = node::Document();
    auto * doctype = doc.appendChild( std::make_unique<node::DocumentType>( U"name", U"public-id", U"system-id" ) );

    ASSERT_EQ( doc.doctype(), doctype );
}

TEST( parser_dom_node_Document_Tests, documentElement ) {
    auto   doc     = node::Document();
    auto * element = doc.appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_HTML ) );

    ASSERT_EQ( doc.documentElement(), element );
}

TEST( parser_dom_node_Document_Tests, getElementById ) {
    std::unique_ptr<node::Node> doc;
    auto v = blogator::tests::createTestDOM_2( doc );

    auto tests = std::vector<DOMString_t>( { U"id-a", U"id-b", U"p3" } );

    for( auto & test : tests ) {
        auto el = dynamic_cast<node::Document *>( doc.get() )->getElementById( test );
        ASSERT_NE( el, nullptr );

        auto attr =  el->attributes().getNamedItem( U"id" );
        ASSERT_NE( attr, nullptr );
        ASSERT_NE( attr->value(), nullptr );
        ASSERT_EQ( *attr->value(), test );
    }
}

TEST( parser_dom_node_Document_Tests, getElementsByTagName_0 ) { //html doc
    std::unique_ptr<node::Node> doc;
    auto v = blogator::tests::createTestDOM_2( doc );

    struct TestCase {
        DOMString_t tag_name;
        uint        expected_found;
    };

    auto tests = std::vector<TestCase>( {
        { U"HEAD", 1 },
        { U"BODY", 1 },
        { U"DIV", 3 },
        { U"H1", 1 },
        { U"H2", 1 },
        { U"H3", 2 },
        { U"P", 3 },
    } );

    auto count = 0;
    for( auto & test : tests ) {
        auto elements = dynamic_cast<node::Document *>( doc.get() )->getElementsByTagName( test.tag_name );
        ASSERT_EQ( elements.size(), test.expected_found ) << "Failed on test " << count;

        for( auto i = 0; i < elements.size(); ++i ) {
            ASSERT_EQ( elements[i]->tagName(), test.tag_name ) << "Failed on test " << count << ", i=" << i;
        }
    }
}

TEST( parser_dom_node_Document_Tests, getElementsByTagName_1 ) { //xml doc
    std::unique_ptr<node::Node> doc;
    auto v = blogator::tests::createTestDOM_3( doc );

    struct TestCase {
        DOMString_t tag_name;
        uint        expected_found;
    };

    auto tests = std::vector<TestCase>( {
        { U"root", 1 },
        { U"tr", 2 },
        { U"td", 6 },
        { U"ug:student", 1 },
        { U"ug:name", 1 },
        { U"ug:surname", 1 },
        { U"ug:score", 1 },
        { U"pg:student", 1 },
        { U"pg:name", 1 },
        { U"pg:surname", 1 },
        { U"pg:score", 1 },
    } );

    auto count = 0;
    for( auto & test : tests ) {
        auto elements = dynamic_cast<node::Document *>( doc.get() )->getElementsByTagName( test.tag_name );
        ASSERT_EQ( elements.size(), test.expected_found ) << "Failed on test " << count;

        for( auto i = 0; i < elements.size(); ++i ) {
            ASSERT_EQ( elements[i]->qualifiedName(), test.tag_name ) << "Failed on test " << count << ", i=" << i;
        }
    }
}

TEST( parser_dom_node_Document_Tests, getElementsByTagName_2 ) { //'*'
    std::unique_ptr<node::Node> doc;
    auto   v             = blogator::tests::createTestDOM_2( doc );
    size_t element_count = 0;

    for( auto * ptr : v ) {
        if( ptr->nodeType() == NodeType::ELEMENT_NODE ) {
            ++element_count;
        }
    }

    auto returned = dynamic_cast<node::Document *>( doc.get() )->getElementsByTagName( U"*" );
    ASSERT_EQ( element_count, returned.size() );
}

TEST( parser_dom_node_Document_Tests, getElementsByTagNameNS_0 ) { //all elements
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );

    auto elements = doc->getElementsByTagNameNS( U"*", U"*" );

    ASSERT_EQ( elements.size(), 17 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"root" );
    ASSERT_EQ( elements[16]->qualifiedName(), U"pg:score" );
}

TEST( parser_dom_node_Document_Tests, getElementsByTagNameNS_1 ) { //all elements in a namespace
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );

    auto elements = doc->getElementsByTagNameNS( U"http://www.site.com/xml/undergrad", U"*" );

    ASSERT_EQ( elements.size(), 4 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"ug:student" );
    ASSERT_EQ( elements[1]->qualifiedName(), U"ug:name" );
    ASSERT_EQ( elements[2]->qualifiedName(), U"ug:surname" );
    ASSERT_EQ( elements[3]->qualifiedName(), U"ug:score" );
}

TEST( parser_dom_node_Document_Tests, getElementsByTagNameNS_2 ) { //one element in any namespace
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );

    auto elements = doc->getElementsByTagNameNS( U"*", U"name" );

    ASSERT_EQ( elements.size(), 2 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"ug:name" );
    ASSERT_EQ( elements[1]->qualifiedName(), U"pg:name" );
}

TEST( parser_dom_node_Document_Tests, getElementsByTagNameNS_3 ) { //particular ns and local name
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_3( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );

    auto elements = doc->getElementsByTagNameNS( U"http://www.site.com/xml/undergrad", U"name" );

    ASSERT_EQ( elements.size(), 1 );
    ASSERT_EQ( elements[0]->qualifiedName(), U"ug:name" );
}

TEST( parser_dom_node_Document_Tests, getElementsByClassName_0 ) { //empty set
    std::unique_ptr<node::Node> doc;
    auto v = blogator::tests::createTestDOM_2( doc );

    auto elements = dynamic_cast<node::Document *>( doc.get() )->getElementsByClassName( {} );
    ASSERT_TRUE( elements.empty() );
}

TEST( parser_dom_node_Document_Tests, getElementsByClassName_1 ) {
    std::unique_ptr<node::Node> doc;
    auto v = blogator::tests::createTestDOM_2( doc );

    struct TestCase {
        DOMString_t class_name;
        uint        expected_found;
    };

    auto tests = std::vector<TestCase>( {
        { U"non-existent", 0 },
        { U"header", 1 },
        { U"title", 3 },
        { U"paragraph-heading", 2 },
        { U"p-section", 3 },
    } );

    auto count = 0;
    for( auto & test : tests ) {
        auto elements = dynamic_cast<node::Document *>( doc.get() )->getElementsByClassName( { test.class_name } );
        ASSERT_EQ( elements.size(), test.expected_found ) << "Failed on test " << count;;

        for( auto i = 0; i < elements.size(); ++i ) {
            auto attr =  elements[i]->attributes().getNamedItem( U"class" );
            ASSERT_NE( attr, nullptr ) << "Failed on test " << count;;
            ASSERT_NE( attr->value(), nullptr ) << "Failed on test " << count;;

            auto values = blogator::string::split<char32_t>( *attr->value(), U' ' );
            ASSERT_NE( std::find_if( values.begin(), values.end(), [&test]( const auto &val ) { return val == test.class_name; } ),
                       values.end() )
                       << "Class name not found in element in test " << count;
        }

        count++;
    }
}

TEST( parser_dom_node_Document_Tests, createElement_0 ) { //by string, html doc
    auto doc = node::Document();
    auto el  = doc.createElement( U"TAGNAME" );

    ASSERT_EQ( el->qualifiedName(), U"tagname" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
    ASSERT_EQ( el->namespaceURI(), U"http://www.w3.org/1999/xhtml" );
}

TEST( parser_dom_node_Document_Tests, createElement_1 ) { //by string, non-html doc
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    auto el  = doc.createElement( U"TAGNAME" );

    ASSERT_EQ( el->qualifiedName(), U"TAGNAME" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
    ASSERT_EQ( el->namespaceURI(), U"http://www.w3.org/XML/1998/namespace" );
}

TEST( parser_dom_node_Document_Tests, createElement_2 ) { //by enum, html doc
    auto doc = node::Document();
    auto el  = doc.createElement( blogator::parser::specs::infra::Element::HTML5_DIV );

    ASSERT_EQ( el->qualifiedName(), U"div" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
    ASSERT_EQ( el->namespaceURI(), U"http://www.w3.org/1999/xhtml" );
}

TEST( parser_dom_node_Document_Tests, createElement_3 ) { //by enum, html doc
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );
    auto el  = doc.createElement( blogator::parser::specs::infra::Element::HTML5_DIV );

    ASSERT_EQ( el->qualifiedName(), U"div" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
    ASSERT_EQ( el->namespaceURI(), U"http://www.w3.org/XML/1998/namespace" );
}

TEST( parser_dom_node_Document_Tests, createElement_fail_0 ) { //by string, invalid
    auto doc = node::Document();

    try {
        std::ignore = doc.createElement( U"įvǎlid°" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_0 ) {
    auto doc     = node::Document();
    auto ns      = DOMString_t( U"parser_dom_node_Document_Tests_createElementNS_0" );
    auto element = doc.createElementNS( ns, U"ns3:name" );

    ASSERT_EQ( element->qualifiedName(), U"ns3:name" );
    ASSERT_EQ( element->ownerDocument(), &doc );
    ASSERT_EQ( element->parentNode(), nullptr );
    ASSERT_EQ( element->namespaceURI(), ns );
}

TEST( parser_dom_node_Document_Tests, createElementNS_1 ) {
    auto doc     = node::Document();
    auto ns      = DOMString_t( U"parser_dom_node_Document_Tests_createElementNS_1" );
    auto element = doc.createElementNS( ns, U"name" );

    ASSERT_EQ( element->namespaceURI(), ns );
    ASSERT_EQ( element->qualifiedName(), U"name" );
    ASSERT_EQ( element->ownerDocument(), &doc );
    ASSERT_EQ( element->parentNode(), nullptr );
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_0 ) { //no namespace
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"", U"ns4:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_1 ) { //invalid name with prefix
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"MyNS5", U"ns5:sp@n" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_2 ) { //invalid name without prefix
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"MyNS5", U"sp@n" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_3 ) { //xml as prefix without correct namespace
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"InvalidNS", U"xml:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_4 ) { //xmlns as name without correct namespace
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"InvalidNS", U"xmlns" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_5 ) { //xmlns as prefix without correct namespace
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"InvalidNS", U"xmlns:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createElementNS_fail_6 ) { //xmlns namespace without 'xmlns' qualifier
    auto doc = node::Document();

    try {
        std::ignore = doc.createElementNS( U"http://www.w3.org/2000/xmlns/", U"invalid" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createTextNode ) {
    auto doc = node::Document();
    auto el  = doc.createTextNode( U"text" );

    ASSERT_NE( el->nodeValue(), nullptr );
    ASSERT_EQ( *el->nodeValue(), U"text" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
}

TEST( parser_dom_node_Document_Tests, createCDATASection ) {
    auto doc = node::Document();
    auto el  = doc.createCDATASection( U"cdata" );

    ASSERT_NE( el->nodeValue(), nullptr );
    ASSERT_EQ( *el->nodeValue(), U"cdata" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
}

TEST( parser_dom_node_Document_Tests, createComment ) {
    auto doc = node::Document();
    auto el  = doc.createComment( U"comment" );

    ASSERT_NE( el->nodeValue(), nullptr );
    ASSERT_EQ( *el->nodeValue(), U"comment" );
    ASSERT_EQ( el->ownerDocument(), &doc );
    ASSERT_EQ( el->parentNode(), nullptr );
}

TEST( parser_dom_node_Document_Tests, createAttribute_0 ) {
    auto doc  = node::Document();
    auto attr = doc.createAttribute( U"test" );

    ASSERT_EQ( attr->localName(), U"test" );
    ASSERT_EQ( attr->ownerDocument(), &doc );
    ASSERT_EQ( attr->parentNode(), nullptr );
}

TEST( parser_dom_node_Document_Tests, createAttribute_fail_0 ) { //invalid local name
    auto doc  = node::Document();

    try {
        std::ignore = doc.createAttribute( U"t e s t" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_0 ) { //local name
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    auto attr = doc.createAttributeNS( U"myNS1", U"myNS1-val" );

    ASSERT_EQ( attr->ownerDocument(), &doc );
    ASSERT_EQ( attr->namespaceURI(), U"myNS1" );
    ASSERT_EQ( attr->prefix(), U"" );
    ASSERT_EQ( attr->localName(), U"myNS1-val" );
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_1 ) { //qualified name
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    auto attr = doc.createAttributeNS( U"myNS2", U"MNS2:myNS2-val" );

    ASSERT_EQ( attr->ownerDocument(), &doc );
    ASSERT_EQ( attr->namespaceURI(), U"myNS2" );
    ASSERT_EQ( attr->prefix(), U"MNS2" );
    ASSERT_EQ( attr->localName(), U"myNS2-val" );
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_fail_0 ) { //invalid name
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    try {
        std::ignore = doc.createAttributeNS( U"myNS", U"învàlîd-n@ame" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_fail_1 ) { //prefix but no namespace
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    try {
        std::ignore = doc.createAttributeNS( U"", U"p:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_fail_2 ) { //xml specified without its correct namespace
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    try {
        std::ignore = doc.createAttributeNS( U"not-XML-NS", U"xml:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_fail_3 ) { //xlmns prefix specified without its correct namespace
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    try {
        std::ignore = doc.createAttributeNS( U"not-XML-NS", U"xmlns:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_fail_4 ) { //xlmns name specified without its correct namespace
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    try {
        std::ignore = doc.createAttributeNS( U"not-XML-NS", U"xmlns" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createAttributeNS_fail_5 ) { //XLMNS namespace but prefix/local name not 'xlmns'
    auto doc = node::Document( U"UTF-8",
                               ContentType::APPLICATION_XML,
                               U"https://www.my.site.com",
                               "/file/path/" );

    try {
        std::ignore = doc.createAttributeNS( U"http://www.w3.org/2000/xmlns/", U"prefix:name" );
        FAIL() << "Nothing thrown.";
    } catch( exception::DOMException &e ) {
        ASSERT_EQ( e.type(), exception::DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_node_Document_Tests, createNodeIterator_0 ) { //with mask (only show Element nodes)
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_2( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );

    size_t actual_element_count   = 0;
    size_t expected_element_count = 0;

    for( auto * ptr : v ) {
        if( ptr->nodeType() == NodeType::ELEMENT_NODE ) {
            ++expected_element_count;
        }
    }

    auto it = doc->createNodeIterator( nullptr, NodeFilter::SHOW_ELEMENT );

    while( it != doc->end() ) {
        ASSERT_EQ( it->nodeType(), NodeType::ELEMENT_NODE );
        ++actual_element_count;
        ++it;
    }

    ASSERT_EQ( expected_element_count, actual_element_count );
}

TEST( parser_dom_node_Document_Tests, createNodeIterator_1 ) { //with filter (only show <p> elements)
    auto filter_fn = []( const node::Node & node ) {
        return ( node.nodeType() == blogator::parser::dom::NodeType::ELEMENT_NODE &&
                 dynamic_cast<const node::Element &>( node ).qualifiedName() == U"p" )
                 ? NodeFilter::Result::FILTER_ACCEPT
                 : NodeFilter::Result::FILTER_REJECT;
    };

    std::unique_ptr<node::Node> root;
    auto   v      = blogator::tests::createTestDOM_2( root );
    auto * doc    = dynamic_cast<node::Document *>( root.get() );
    auto   filter = NodeFilter( NodeFilter::SHOW_ALL, filter_fn );

    auto it = doc->createNodeIterator( nullptr, filter );

    size_t actual_count = 0;

    while( it != doc->end() ) {
        ASSERT_EQ( it->nodeType(), NodeType::ELEMENT_NODE );
        ASSERT_EQ( dynamic_cast<node::Element *>( it.node() )->tagName(), U"P" );
        ++actual_count;
        ++it;
    }

    ASSERT_EQ( actual_count, 3 );
}

TEST( parser_dom_node_Document_Tests, createTreeWalker_0 ) { //with mask (only show Element nodes)
    std::unique_ptr<node::Node> root;
    auto   v   = blogator::tests::createTestDOM_2( root );
    auto * doc = dynamic_cast<node::Document *>( root.get() );

    size_t actual_element_count   = 0;
    size_t expected_element_count = 0;

    for( auto * ptr : v ) {
        if( ptr->nodeType() == NodeType::ELEMENT_NODE ) {
            ++expected_element_count;
        }
    }

    auto walker = doc->createTreeWalker( nullptr, NodeFilter::SHOW_ELEMENT );

    while( walker.nextNode() ) {
        ASSERT_EQ( walker.currentNode()->nodeType(), NodeType::ELEMENT_NODE );
        ++actual_element_count;
    }

    ASSERT_EQ( expected_element_count, actual_element_count );
}

TEST( parser_dom_node_Document_Tests, createTreeWalker_1 ) { //with filter (only show <p> elements)
    auto filter_fn = []( const node::Node & node ) {
        return ( node.nodeType() == blogator::parser::dom::NodeType::ELEMENT_NODE &&
                 dynamic_cast<const node::Element &>( node ).qualifiedName() == U"p" )
               ? NodeFilter::Result::FILTER_ACCEPT
               : NodeFilter::Result::FILTER_SKIP;
    };


    std::unique_ptr<node::Node> root;
    auto   v      = blogator::tests::createTestDOM_2( root );
    auto * doc    = dynamic_cast<node::Document *>( root.get() );
    auto   filter = NodeFilter( NodeFilter::SHOW_ALL, filter_fn );

    auto walker = doc->createTreeWalker( nullptr, filter );

    size_t actual_count = 0;

    while( walker.nextNode() ) {
        ASSERT_EQ( walker.currentNode()->nodeType(), NodeType::ELEMENT_NODE );
        ASSERT_EQ( dynamic_cast<node::Element *>( walker.currentNode() )->tagName(), U"P" );
        ++actual_count;
    }

    ASSERT_EQ( actual_count, 3 );
}

TEST( parser_dom_node_Document_Tests, nodeName ) {
    auto doc = node::Document();
    ASSERT_EQ( doc.nodeName(), U"#document" );
}

TEST( parser_dom_node_Document_Tests, output_stream ) {
    std::unique_ptr<node::Node> doc;
    auto v = blogator::tests::createTestDOM_2( doc );

    std::stringstream expected_ss;
    expected_ss << "<!DOCTYPE html>"
                   "<!CDATA[cdata]]>"
                   "<html>"
                     "<head class=\"header\">"
                       "<div>"
                         "<h1>Heading 1</h1>"
                       "</div>"
                     "</head>"
                     "<!-- comment -->"
                     "<body>"
                       "<div id=\"id-a\">"
                         "<h2 class=\"title\">Heading 2</h2>"
                         "<h3 class=\"title paragraph-heading\">Sub-Title 1</h3>"
                         "<p class=\"p-section\">"
                           "Paragraph 1"
                         "</p>"
                       "</div>"
                       "<div id=\"id-b\">"
                         "<h3 class=\"title paragraph-heading\">Sub-Title 2</h3>"
                         "<p class=\"p-section\">"
                           "Paragraph 2"
                         "</p>"
                         "<p class=\"p-section\" id=\"p3\">"
                           "Paragraph 3"
                         "</p>"
                       "</div>"
                     "</body>"
                   "</html>";

    std::stringstream received_ss;
    received_ss << *doc;

    ASSERT_EQ( received_ss.str(), expected_ss.str() );
}