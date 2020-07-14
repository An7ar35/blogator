#ifndef BLOGATOR_DOM_HTML5_H
#define BLOGATOR_DOM_HTML5_H

#include "Tag.h"
#include "Attribute.h"
#include "Specs.h"
#include "rules.h"

/**
 * Header file to collate everything in the html5:: namespace so that only 1 #inlcude need be done.
 */
namespace blogator::dom::html5 {
    const auto strToTag = Specs::strToTag;
    const auto tagToStr = Specs::tagToStr;
    const auto isPaired = Specs::isPaired;
}

#endif //BLOGATOR_DOM_HTML5_H
