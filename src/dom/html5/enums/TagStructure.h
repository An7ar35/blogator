#ifndef BLOGATOR_DOM_HTML5_TAGSTRUCTURE_H
#define BLOGATOR_DOM_HTML5_TAGSTRUCTURE_H

namespace blogator::dom::html5 {
    /**
     * HTML5 tag structure
     */
    enum class TagStructure {
        PAIRED,   //open and closing tag pair
        UNPAIRED, //single self-closing tag
    };
}

#endif //BLOGATOR_DOM_HTML5_TAGSTRUCTURE_H
