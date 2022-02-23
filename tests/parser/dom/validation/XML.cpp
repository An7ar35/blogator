#include "gtest/gtest.h"
#include "../../../src/parser/dom/validation/XML.h"

#include "../../../src/parser/dom/exception/DOMException.h"

using namespace blogator::parser::dom::validation;
using           blogator::parser::dom::exception::DOMException;
using           blogator::parser::dom::exception::DOMExceptionType;

TEST( parser_dom_validation_XML_Tests, validateNS_0 ) { //name only
    auto v = XML::validateNS( U"", U"name" );
    ASSERT_EQ( v.size(), 1 );
    ASSERT_EQ( v[0], U"name" );
}

TEST( parser_dom_validation_XML_Tests, validateNS_1 ) { //namespace + name
    auto v = XML::validateNS( U"validation_XML_Tests_NS", U"name" );
    ASSERT_EQ( v.size(), 1 );
    ASSERT_EQ( v[0], U"name" );
}

TEST( parser_dom_validation_XML_Tests, validateNS_2 ) { //namespace + prefixed name
    auto v = XML::validateNS( U"validation_XML_Tests_NS", U"prefix:name" );
    ASSERT_EQ( v.size(), 2 );
    ASSERT_EQ( v[0], U"prefix" );
    ASSERT_EQ( v[1], U"name" );
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_0 ) { //no name
    try {
        std::ignore = XML::validateNS( U"validation_XML_Tests_NS", U"" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_1 ) { //no namespace with a prefixed name
    try {
        std::ignore = XML::validateNS( U"", U"prefix:name" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_2 ) { //invalid name
    try {
        std::ignore = XML::validateNS( U"validation_XML_Tests_NS", U"n@me" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_3 ) { //invalid name with valid prefix
    try {
        std::ignore = XML::validateNS( U"validation_XML_Tests_NS", U"XMLTestNS:n@me" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_4 ) { //invalid prefix
    try {
        std::ignore = XML::validateNS( U"validation_XML_Tests_NS", U"Inv@lidNS:name" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::InvalidCharacterError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_5 ) { //xml prefix without correct namespace
    try {
        std::ignore = XML::validateNS( U"InvalidNS", U"xml:name" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_6 ) { //xmlns as name without correct namespace
    try {
        std::ignore = XML::validateNS( U"InvalidNS", U"xmlns" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_7 ) { //xmlns as prefix without correct namespace
    try {
        std::ignore = XML::validateNS( U"InvalidNS", U"xmlns:name" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_8 ) { //xmlns namespace without 'xmlns' as prefix
    try {
        std::ignore = XML::validateNS( U"http://www.w3.org/2000/xmlns/", U"prefix:name" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}

TEST( parser_dom_validation_XML_Tests, validateNS_fail_9 ) { //xmlns namespace without 'xmlns' as name
    try {
        std::ignore = XML::validateNS( U"http://www.w3.org/2000/xmlns/", U"name" );
        FAIL() << "Nothing thrown.";
    } catch( DOMException &e ) {
        ASSERT_EQ( e.type(), DOMExceptionType::NamespaceError ) << "DOMException thrown with wrong DOMExceptionType: " << e.type();
    } catch( std::exception &other ) {
        FAIL() << "Wrong exception type thrown.";
    }
}