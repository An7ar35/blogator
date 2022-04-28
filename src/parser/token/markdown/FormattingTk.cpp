#include "FormattingTk.h"

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
 * Sets the format type for the token
 * @param type Format type
 */
void FormattingTk::setFormatType( FormattingTk::Type type ) {
    _format_type = type;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void FormattingTk::toStr( std::ostream &os ) const {
    os << "MarkdownTk={ type: " << this->type() << ", formatting: " << this->formatType() << ", text: \"";
    blogator::unicode::utf8::convert( os, this->text() );
    os << "\", position: " << this->position() << " }";
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
        default:                                { os << "NONE";          } break;;
    }

    return os;
}
