#ifndef BLOGATOR_DOM_HTML5_NAMESPACE_H
#define BLOGATOR_DOM_HTML5_NAMESPACE_H

namespace blogator::dom::html5 {
    enum class NameSpace {
        BLOGATOR = 0,
        HTML5,
        MathML,
        SVG,

        /* insert any new namespaces here... */

        AGNOSTIC, //all namespaces
        ENUM_END  //used for iterating enums
    };
}

#endif //BLOGATOR_DOM_HTML5_NAMESPACE_H
