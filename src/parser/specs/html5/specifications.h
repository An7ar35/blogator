#ifndef BLOGATOR_PARSER_HTML5_SPECS_H
#define BLOGATOR_PARSER_HTML5_SPECS_H

#include "ContentModel.h"
#include "Element.h"
#include "ErrorCode.h"
#include "InsertionMode.h"
#include "Namespace.h"

//Namespace = { [Element...] }
//Element = { PAIRED/UNPAIRED, VERBATIM, ContentModel }
//ContentModel = { [{attribute, checkFunction}...] }

namespace blogator::parser::specs::html5::specifications {
    static html5::ContentModel getContentModel( html5::Element element );
    static Namespace getNamespace( html5::Element element );

    //TODO create getContentModel lookup for elements
}

#endif //BLOGATOR_PARSER_HTML5_SPECS_H
