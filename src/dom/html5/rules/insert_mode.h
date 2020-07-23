#ifndef BLOGATOR_DOM_HTML5_RULES_INSERT_MODE_H
#define BLOGATOR_DOM_HTML5_RULES_INSERT_MODE_H

#include "../enums/InsertMode.h"
#include "../../dot/DOTNode.h"

namespace blogator::dom::html5::rules::insert_mode {
    template<InsertMode> static bool validateContext( const DOTNode &node );
}

/* ======================================= IMPLEMENTATION ======================================= */

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::INITIAL>( const blogator::dom::DOTNode &node ) {
    return ( node.type() == Tag::DOCTYPE )
        || ( node.type() == Tag::COMMENT );
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::BEFORE_HTML>( const blogator::dom::DOTNode &node ) {
    if( node.type() == Tag::COMMENT )
        return true;
    return false;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::BEFORE_HEAD>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_HEAD>( const blogator::dom::DOTNode &node ) {
    switch( node.type() ) {
        case Tag::COMMENT:  [[fallthrough]];
        case Tag::BASE:     [[fallthrough]];
        case Tag::LINK:     [[fallthrough]];
        case Tag::META:     [[fallthrough]];
        case Tag::TITLE:    [[fallthrough]];
        case Tag::NOSCRIPT: [[fallthrough]];
        case Tag::STYLE:
            return true;
        case Tag::SCRIPT:
            //TODO check before tag
        case Tag::TEMPLATE:


        default:
            return false;
    }
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_HEAD_NOSCRIPT>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::AFTER_HEAD>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_BODY>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::TEXT>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_TABLE>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_TABLE_TEXT>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_CAPTION>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_COLUMN_GROUP>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_TABLE_BODY>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_ROW>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_CELL>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_SELECT>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_CELL>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_SELECT_IN_TABLE>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_TEMPLATE>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::AFTER_BODY>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::IN_FRAMESET>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::AFTER_FRAMESET>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::AFTER_AFTER_BODY>( const blogator::dom::DOTNode &node ) {
    return 0;
}

template<> bool blogator::dom::html5::rules::insert_mode::validateContext<blogator::dom::html5::InsertMode::AFTER_AFTER_FRAMESET>( const blogator::dom::DOTNode &node ) {
    return 0;
}

#endif //BLOGATOR_DOM_HTML5_RULES_INSERT_MODE_H
