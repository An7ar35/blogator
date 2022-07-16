#include "FormattingTk.h"

#include <unordered_map>

#include "../../../unicode/unicode.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param type Markdown token type
 * @param position Line:Col position of token in source text
 */
FormattingTk::FormattingTk( specs::markdown::TokenType type, TextPos position ) :
    MarkdownTk( type, position ),
    _format_type( Type::NONE )
{}

/**
 * Constructor
 * @param type Markdown token type
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
FormattingTk::FormattingTk( specs::markdown::TokenType type, std::u32string text, TextPos position ) :
    MarkdownTk( type, std::move( text ), position ),
    _format_type( Type::NONE )
{}

/**
 * Gets the token's format type
 * @return Format type
 */
FormattingTk::Type FormattingTk::formatType() const {
    return _format_type;
}

/**
 * Check for match with formatting character
 * @param fmt_char_1 Formatting character
 * @return Matching state
 */
bool FormattingTk::match( uint32_t fmt_char_1 ) const {
    return ( this->text().size() == 1 && this->text()[0] == fmt_char_1 );
}

/**
 * Check for match with formatting characters
 * @param fmt_char_1 Formatting character
 * @param fmt_char_2 Formatting character
 * @return Matching state
 */
bool FormattingTk::match( uint32_t fmt_char_1, uint32_t fmt_char_2 ) const {
    return ( this->text().size() == 2 && this->text()[0] == fmt_char_1 && this->text()[1] == fmt_char_2 );
}

/**
 * Sets the format type for the token
 * @param type Format type
 */
void FormattingTk::setFormatType( FormattingTk::Type type ) {
    _format_type = type;
}

/**
 * Resolves a formatting type based on a string
 * @param text Markdown formatting character(s)
 * @return Format type enum
 */
FormattingTk::Type FormattingTk::resolveFormatType( const std::u32string &text ) {
    static const auto MAP = std::unordered_map<std::u32string, Type>( {
        { U"",   Type::NONE },
        { U"**", Type::BOLD },
        { U"__", Type::BOLD },
        { U"*",  Type::ITALIC },
        { U"_",  Type::ITALIC },
        { U"~~", Type::STRIKETHROUGH },
        { U"==", Type::HIGHLIGHT },
        { U"~",  Type::SUBSCRIPT },
        { U"^",  Type::SUPERSCRIPT },
        { U"`",  Type::INLINE_CODE_1 },
        { U"``", Type::INLINE_CODE_2 },
    } );

    const auto it = MAP.find( text );

    return ( it == MAP.end() ? Type::NONE : it->second );
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void FormattingTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type() << R"(", "formatting": ")" << this->formatType() << R"(", "text": ")";
    unicode::normalize( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}

/**
 * Output stream operator
 * @param os Output stream
 * @param type FormattingTk::Type enum
 * @return Output stream
 */
std::ostream & blogator::parser::token::markdown::operator <<( std::ostream &os, FormattingTk::Type type ) {
    switch( type ) {
        case FormattingTk::Type::BOLD:          { os << "BOLD";          } break;
        case FormattingTk::Type::ITALIC:        { os << "ITALIC";        } break;
        case FormattingTk::Type::STRIKETHROUGH: { os << "STRIKETHROUGH"; } break;
        case FormattingTk::Type::HIGHLIGHT:     { os << "HIGHLIGHT";     } break;
        case FormattingTk::Type::SUBSCRIPT:     { os << "SUBSCRIPT";     } break;
        case FormattingTk::Type::SUPERSCRIPT:   { os << "SUPERSCRIPT";   } break;
        case FormattingTk::Type::INLINE_CODE_1: { os << "INLINE_CODE_1"; } break;
        case FormattingTk::Type::INLINE_CODE_2: { os << "INLINE_CODE_2"; } break;
        default:                                { os << "NONE";          } break;
    }

    return os;
}
