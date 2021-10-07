#ifndef BLOGATOR_PARSER_HTML5_SPECS_H
#define BLOGATOR_PARSER_HTML5_SPECS_H

#include "ContentModel.h"
#include "Element.h"
#include "ErrorCode.h"
#include "InsertionMode.h"
#include "Namespace.h"
#include "NamedCharRef.h"
#include "NumericCharRef.h"

#include "tokeniser/TokeniserState.h"
#include "tokeniser/TokenType.h"

//Namespace = { [Element...] }
//Element = { PAIRED/UNPAIRED, VERBATIM, ContentModel }
//ContentModel = { [{attribute, checkFunction}...] }

namespace blogator::parser::specs::html5 {
    struct Specifications {
        static bool matchesContentModel( html5::Element element, html5::ContentModel content_model );

        static void printElement( std::ostream &os, Element el );
        static void printContentModel( std::ostream &os, ContentModel model );
        static void printInsertionMode( std::ostream &os, InsertionMode mode );
        static void printNamespace( std::ostream &os, Namespace ns );
    };

    //TODO create getContentModel lookup for elements
}

#endif //BLOGATOR_PARSER_HTML5_SPECS_H
