#ifndef BLOGATOR_PARSER_SPECS_HTML5_CONTENTTYPE_H
#define BLOGATOR_PARSER_SPECS_HTML5_CONTENTTYPE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::infra {
    enum class ContentType {
        APPLICATION_XHTML_XML,
        APPLICATION_RSS_XML,
        APPLICATION_XML,
        IMAGE_SVG_XML,
    };

    std::ostream & operator <<( std::ostream &os, ContentType content_type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::infra::ContentType content_type );
    std::u32string to_u32string( blogator::parser::specs::infra::ContentType content_type );
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_CONTENTTYPE_H