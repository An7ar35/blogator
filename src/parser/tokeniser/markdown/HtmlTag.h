#ifndef BLOGATOR_PARSER_TOKENISER_MARKDOWN_HTMLTAG_H
#define BLOGATOR_PARSER_TOKENISER_MARKDOWN_HTMLTAG_H

#include <string>

namespace blogator::parser::tokeniser::markdown {
    /**
     * HTML tag cache
     * @param type Tag type
     * @param name Tag name
     */
    struct HtmlTag {
        enum class Type {
            START,
            END,
            SELF_CLOSE,
        };

        HtmlTag();

        Type           type;
        std::u32string name;
    };
}

#endif //BLOGATOR_PARSER_TOKENISER_MARKDOWN_HTMLTAG_H
