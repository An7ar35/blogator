#include "validator.h"

#include <cwctype>

#include "../special_char.h"
#include "../../../exception/DOMException.h"
#include "../../../encoding/encoding.h"
#include "../../../string/helpers.h"

/**
 * Checks the character matches the end of tag character ('>')
 * @param c Character to check
 * @return Matching state for the '>' character
 */
bool blogator::dom::html5::validator::isTagEnd( char32_t c ) {
    return c == special_char::GREATER_THAN;
}

/**
 * Checks for validity of attribute name character
 * @param c        UTF-32 character
 * @param err_flag Error flag switch to true when an illegal character is found (ctrl, ", ' or non-character)
 * @return Validity state i.e.: !(ctrl OR '"' OR '\'' OR non-char OR space OR '>' OR '/' OR '=' )
 */
bool blogator::dom::html5::validator::isValidAttributeNameChar( char32_t c, bool &err_flag ) {
    /* "Attribute names must consist of one or more characters other than controls,
     *  U+0020 SPACE, U+0022 ("), U+0027 ('), U+003E (>), U+002F (/), U+003D (=),
     *  and non-characters." */
    err_flag = iswcntrl( c )
            || c == html5::special_char::QUOTATION_MARK
            || c == html5::special_char::APOSTROPHE
            || encoding::isNonCharacter( c );

    return !( err_flag
              || c == html5::special_char::SPACE
              || c == html5::special_char::GREATER_THAN
              || c == html5::special_char::SOLIDUS
              || c == html5::special_char::EQUALS_SIGN
    );
}


/**
 * Checks for validity of character from an unquoted attribute value
 * @param c        UTF-32 character
 * @param err_flag Error flag switch to true when an illegal character is found (ctrl/non-character)
 * @return  Validity state i.e.: !(ctrl OR non-char OR space OR '/' )
 */
bool blogator::dom::html5::validator::isValidAttributeValueChar( char32_t c, bool &err_flag ) {
    err_flag = iswcntrl( c )
            || encoding::isNonCharacter( c )
            || c == html5::special_char::QUOTATION_MARK
            || c == html5::special_char::APOSTROPHE
            || c == html5::special_char::LESS_THAN
            || c == html5::special_char::EQUALS_SIGN
            || c == html5::special_char::GRAVE_ACCENT;

    return !( err_flag
              || c == html5::special_char::SPACE
              || c == html5::special_char::GREATER_THAN
    );
}

/**
 * Checks for validity of character from a bound attribute value
 * @param c             UTF-32 character
 * @param boundary_char UTF-32 character used as a boundary char for the attribute value (e.g.: single or double quotations)
 * @param err_flag      Error flag switch to true when an illegal character is found (ctrl/non-character)
 * @return  Validity state i.e.: !(ctrl OR non-char OR boundary character)
 */
bool blogator::dom::html5::validator::isValidAttributeValueChar( char32_t c, char32_t boundary_char, bool &err_flag ) {
    err_flag = iswcntrl( c ) || encoding::isNonCharacter( c );
    return !( err_flag || c == boundary_char );
}

/**
 * Escapes an HTML5 reserved character
 * @param c Character
 * @return Character reference string ("&..;")
 */
std::u32string blogator::dom::html5::validator::escapeReservedChar( char32_t c ) {
    static const std::u32string QUOTATION_MARK = { U"&quot;" };
    static const std::u32string APOSTROPHE     = { U"&#39;"  };
    static const std::u32string AMPERSAND      = { U"&amp;"  };
    static const std::u32string LESS_THAN      = { U"&lt;"   };
    static const std::u32string GREATER_THAN   = { U"&gt;"   };

    switch( c ) {
        case html5::special_char::QUOTATION_MARK:
            return QUOTATION_MARK;
        case html5::special_char::APOSTROPHE:
            return APOSTROPHE;
        case html5::special_char::AMPERSAND:
            return AMPERSAND;
        case html5::special_char::LESS_THAN:
            return LESS_THAN;
        case html5::special_char::GREATER_THAN:
            return GREATER_THAN;
        default:
            return U"";
    }
}

/**
 * Validates character reference structure (does _not_ check the value itself)
 * @param str String that may be a character reference
 * @return Validity state
 */
bool blogator::dom::html5::validator::isValidCharReference( const std::u32string& str ) {
    if( str.size() < 3 )
        return false;

    auto       it        = str.cbegin();
    auto       val_begin = it;
    const auto last      = std::next( it, str.size() - 1 );

    if( *(it++) != special_char::AMPERSAND || *last != special_char::SEMICOLON ) // "&...;"
        return false;

    if( *it == U'#' ) { //Decimal || Hexadecimal
        if( std::towlower( *(++it) ) == U'x' ) {//Hexadecimal ("&#x...;")
            val_begin = std::next( it );
            while( ( ++it ) != last && iswxdigit( *it ) );

        } else { //Decimal  ("&#...;")
            val_begin = it;
            while( it != last && iswdigit( *( it++ ) ) );
        }

    } else { //Named (ASCII alphanumeric)
        while( it != last && iswalnum( *(it++) ) );
    }

    return it == last && ( std::distance( val_begin, last ) > 0 );
}

/**
 * Validates a tag as an opening tag
 * @param str        Complete tag string "<...>"
 * @param attributes Place to insert the validated attributes found (v, k=v, k='v', k="v")
 * @return Opening tag name type
 * @throws exception::DOMException when validation fails
 */
blogator::dom::html5::Tag blogator::dom::html5::validator::validateOpeningTag(
    const std::u32string &str,
    std::vector<std::pair<std::u32string, std::u32string>> &attributes )
{
    auto it            = str.cbegin();
    auto substr_length { 0 };
    bool end_reached   { false };

    if( it == str.cend() ) { //empty
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateOpeningTag( \"" << encoding::encodeToUTF8( str ) << "\", .. )";
        throw exception::DOMException( ss.str(),
            "Opening tag string is empty.",
            exception::DOMErrorType::SyntaxError
        );
    }

    if( *(it++) != html5::special_char::LESS_THAN ) { //i=0 '<'
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateOpeningTag( \"" << encoding::encodeToUTF8( str ) << "\", .. )";
        throw exception::DOMException( ss.str(),
            "1st character of an opening tag must be U+003C ('<').",
            exception::DOMErrorType::SyntaxError
        );
    }

    if( it == str.cend() || !iswalpha( *(it++) ) ) { //i=2 beginning of tag name
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateOpeningTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "Tag name must be made of ASCII alphanumeric characters only.",
            exception::DOMErrorType::SyntaxError
        );
    }

    ++substr_length;

    while( it != str.cend() && !iswspace( *it ) && !isTagEnd( *it ) && *it != html5::special_char::SOLIDUS ) { //reach end of tag name
        if( !iswalpha( *(it++) ) ) {
            std::stringstream ss;
            ss << "blogator::dom::html5::validator::validateOpeningTag( \"" << encoding::encodeToUTF8( str ) << "\", .. )";
            throw exception::DOMException( ss.str(),
                "Tag name must be made of ASCII alphanumeric characters only.",
                exception::DOMErrorType::SyntaxError
            );
        }

        ++substr_length;
    }

    const auto tag_name = str.substr( 1, substr_length );

    string::skipWhitespace<char32_t>( str, it );

    { //Parse and validate attributes if any
        auto attribute_begin_it = it;

        while( it != str.cend() && *it != html5::special_char::SOLIDUS && !isTagEnd( *it ) )
            ++it;

        const auto attribute_length  = it - attribute_begin_it;
        const auto attribute_start_i = attribute_begin_it - str.cbegin();

        if( attribute_length ) {
            try {
                validateAttributes( str.substr( attribute_start_i, attribute_length ), attributes );

            } catch( std::invalid_argument &e ) {
                std::stringstream loc, msg;
                loc << "blogator::dom::html5::validator::validateOpeningTag( \"" << encoding::encodeToUTF8( str ) << "\", .. )";
                msg << "Invalid attribute(s) format: " << e.what();
                throw exception::DOMException( loc.str(), msg.str(), exception::DOMErrorType::SyntaxError );
            }
        }
    }

    if( it == str.cend() || *it == html5::special_char::SOLIDUS ) { //i.e. self closing tag "<.../>"
        if( (++it) == str.cend() || !isTagEnd( *it ) ) { //'>'
            std::stringstream ss;
            ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
            throw exception::DOMException( ss.str(),
                "The last character on a self closing tag (\"<.../>\") must U+003E ('>').",
                exception::DOMErrorType::SyntaxError
            );
        }

        end_reached = true;
    }

    if( !end_reached && isTagEnd( *(it++) ) )
        end_reached = true;

    if( end_reached && it != str.cend() ) {
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
           "String has character(s) after the U+003E character ('>').",
           exception::DOMErrorType::SyntaxError
        );
    }

    try {
        return html5::Html5Properties::strToTag( tag_name );

    } catch( std::invalid_argument &e ) {
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateOpeningTag( \"" << encoding::encodeToUTF8( str ) << "\", .. )";
        throw exception::DOMException( ss.str(),
            "Tag name is not valid.",
            exception::DOMErrorType::SyntaxError
        );
    }
}

/**
 * Validates an attribute string
 * @param str        Attribute string in the format of N, N=V, N='V' or N="V"
 * @param attributes Attribute store
 * @throws std::invalid_argument when attribute string has formatting errors
 */
void blogator::dom::html5::validator::validateAttributes(
    const std::u32string &str,
    std::vector<std::pair<std::u32string, std::u32string>> &attributes )
{
    auto it = str.begin();

    string::skipWhitespace<char32_t>( str, it );

    if( it == str.cend() ) //no more attributes/end of recursion
        return;

    u32stringstream_t name, value;

    { //attribute name
        bool illegal_char { false };

        while( it != str.cend() && validator::isValidAttributeNameChar( *it, illegal_char ) )
            name << *( it++ );

        if( illegal_char ) {
            std::stringstream ss;
            ss << "Invalid character in named attribute \"" << encoding::encodeToUTF8( str ) << "\".";
            throw std::invalid_argument( ss.str() );
        }
    }

    string::skipWhitespace<char32_t>( str, it );

    if( it != str.cend() && *(it++) == special_char::EQUALS_SIGN ) { //attribute name has a value

        string::skipWhitespace<char32_t>( str, it );

        if( it == str.cend() ) {
            std::stringstream ss;
            ss << "Expected value for named attribute \"" << encoding::encodeToUTF8( name.str() ) << "\" missing.";
            throw std::invalid_argument( ss.str() );
        }

        bool illegal_char { false };
        auto boundary     { AttrBoundaryChar::NONE };

        if( *it == special_char::APOSTROPHE ) { //single-quoted attribute value syntax
            boundary = AttrBoundaryChar::APOSTROPHE;

            while( (++it) != str.cend() && isValidAttributeValueChar( *it, special_char::APOSTROPHE, illegal_char ) )
                value << *it;

        } else if( *it == special_char::QUOTATION_MARK ) { //double-quoted attribute value syntax
            boundary = AttrBoundaryChar::QUOTATION_MARK;

            while( (++it) != str.cend() && isValidAttributeValueChar( *it, special_char::QUOTATION_MARK, illegal_char ) )
                value << *it;

        } else { //unquoted attribute value syntax
            while( it != str.cend() && isValidAttributeValueChar( *it, illegal_char ) )
                value << *(it++);
        }

        if( illegal_char ) {
            std::stringstream ss;
            ss << "Invalid character in value for named attribute \"" << encoding::encodeToUTF8( name.str() ) << "\".";
            throw std::invalid_argument( ss.str() );
        }

        if( it == str.cend() && boundary != AttrBoundaryChar::NONE ) {
            std::stringstream ss;
            ss << "Missing closing character ("
               << ( boundary == AttrBoundaryChar::APOSTROPHE ? "'" : "\"")
               << ") from value for named attribute \"" << encoding::encodeToUTF8( name.str() ) << "\".";
            throw std::invalid_argument( ss.str() );
        }

        if( boundary != AttrBoundaryChar::NONE )
            ++it;
    }

    attributes.emplace_back( name.str(), value.str() );

    if( it != str.cend() )
        validateAttributes( str.substr( it - str.cbegin() ), attributes );
}

/**
 * Validates a tag as a closing tag
 * @param str Complete tag string "<...>"
 * @return Closing tag name type
 * @throws exception::DOMException when validation fails
 */
blogator::dom::html5::Tag blogator::dom::html5::validator::validateClosingTag( const std::u32string &str ) {
    auto it     = str.cbegin();
    auto length = 0;

    if( it == str.cend() ) { //empty
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "Closing tag string is empty.",
            exception::DOMErrorType::SyntaxError
        );
    }

    if( *(it++) != html5::special_char::LESS_THAN ) { //i=0 '<'
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "1st character of an end tag must be U+003C ('<').",
            exception::DOMErrorType::SyntaxError
        );
    }

    if( it == str.cend() || *(it++) != html5::special_char::SOLIDUS ) { //i=1 '/'
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "2nd character of an end tag must be U+002F ('/').",
            exception::DOMErrorType::SyntaxError
        );
    }

    if( it == str.cend() || isTagEnd( *it ) ) { //i=2 beginning of tag name
        std::stringstream loc, msg;
        loc << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        msg << "No name found in closing tag \"" << encoding::encodeToUTF8( str ) << "\".";
        throw exception::DOMException( loc.str(), msg.str(), exception::DOMErrorType::SyntaxError );
    }

    while( it != str.cend() && !iswspace( *it ) && !isTagEnd( *it ) ) { //reach end of tag name
        if( !iswalpha( *(it++) ) ) {
            std::stringstream ss;
            ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
            throw exception::DOMException( ss.str(),
                "Tag name must be made of ASCII alphanumeric characters only.",
                exception::DOMErrorType::SyntaxError
            );
        }

        ++length;
    }

    if( length == 0 ) {
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "No name detected after \"</\" in closing tag.",
            exception::DOMErrorType::SyntaxError
        );
    }

    while( it != str.cend() && !isTagEnd( *it ) ) {
        if( !iswspace( *(it++) ) ) {
            std::stringstream ss;
            ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
            throw exception::DOMException( ss.str(),
                "After the tag name, there may only be ASCII whitespace before the closing character.",
                exception::DOMErrorType::SyntaxError
            );
        }
    }

    if( it == str.cend() || !isTagEnd( *it ) ) { //'>'
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "The last character on an end tag must U+003E ('>').",
            exception::DOMErrorType::SyntaxError
        );
    }

    try {
        return html5::Html5Properties::strToTag( str.substr( 2, length ) );

    } catch( std::invalid_argument &e ) {
        std::stringstream ss;
        ss << "blogator::dom::html5::validator::validateClosingTag( \"" << encoding::encodeToUTF8( str ) << "\" )";
        throw exception::DOMException( ss.str(),
            "Tag name is not valid.",
            exception::DOMErrorType::SyntaxError
        );
    }
}