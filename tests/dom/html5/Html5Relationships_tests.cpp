#include "gtest/gtest.h"
#include "../../src/dom/html5/Html5Relationships.h"

TEST( Html5Relationships, validateNode_consistency_check ) { //Checks that all Tag types are covered in the `validateNode(..)` method
    using blogator::dom::html5::Html5Relationships;
    using blogator::dom::html5::Tag;
    using blogator::dom::DOTNode;

    for( auto i = 0; i < static_cast<int>( Tag::ENUM_END ); ++i ) {
        auto node = DOTNode( static_cast<Tag>( i ) );
        ASSERT_NO_THROW( Html5Relationships::validateNode( node ) );
    }

    auto node = DOTNode( static_cast<Tag>( Tag::ENUM_END ) );
    ASSERT_THROW( Html5Relationships::validateNode( node ), std::invalid_argument );
}

//#include "../../src/dom/html5/Html5Properties.h"
//#include "../../src/encoding/encoding.h"

//TEST( XXX, print ) {
//    using blogator::dom::html5::Html5Properties;
//    using blogator::dom::html5::Tag;
//
//    for( size_t i = 0; i < static_cast<size_t>( Tag::ENUM_END ); ++ i ) {
//        std::stringstream ss;
//        auto tag = blogator::encoding::encodeToUTF8( Html5Properties::tagToStr( static_cast<Tag>( i ) ) );
//        std::transform( tag.begin(), tag.end(), tag.begin(), []( auto c ) { return std::toupper( c ); } );
//        ss << "template<> bool blogator::dom::html5::rules::context::isValidContext<blogator::dom::html5::Tag::"
//           <<  tag << ">( const DOTNode &node ) {\n"
//              "    return false;\n"
//              "}\n\n";
//        std::cout << ss.str();
//    }
//    std::cout << std::endl;
//}