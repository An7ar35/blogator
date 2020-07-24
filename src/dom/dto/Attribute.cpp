#include "Attribute.h"

#include <stdexcept>

#include "../html5/special_char.h"

/**
 * Equality operator
 * @param rhs Attribute to compare with
 * @return Equality state of the attributes
 */
bool blogator::dom::dto::Attribute::operator ==( const dto::Attribute &rhs ) const {
    return this->value == rhs.value && this->boundary == rhs.boundary;
}

/**
 * Not-Equal operator
 * @param rhs Attribute to compare with
 * @return !Equal state of the attributes
 */
bool blogator::dom::dto::Attribute::operator !=( const dto::Attribute &rhs ) const {
    return !( *this == rhs );
}

/**
 * Checks a string for any apostrophes or quotation marks in use
 * @param str String to check
 * @return Boundary character in use
 * @throws std::invalid_argument when both quotation mark(s) and apostrophe(s) are used in the string
 */
blogator::dom::html5::AttrBoundaryChar blogator::dom::dto::Attribute::checkNestedBoundaryChar( const std::u32string &str ) {
    auto bc = html5::AttrBoundaryChar::NONE;
    auto it = str.cbegin();

    while( it != str.cend() ) {
        switch( *(it++) ) {
            case html5::special_char::APOSTROPHE: {
                switch( bc ) {
                    case html5::AttrBoundaryChar::NONE:
                        bc = html5::AttrBoundaryChar::APOSTROPHE;
                        break;
                    case html5::AttrBoundaryChar::APOSTROPHE:
                        break;
                    case html5::AttrBoundaryChar::QUOTATION_MARK:
                        throw std::invalid_argument( "Using both quotation and apostrophes in string." );
                }
            }
            break;

            case html5::special_char::QUOTATION_MARK: {
                switch( bc ) {
                    case html5::AttrBoundaryChar::NONE:
                        bc = html5::AttrBoundaryChar::QUOTATION_MARK;
                        break;
                    case html5::AttrBoundaryChar::APOSTROPHE:
                        throw std::invalid_argument( "Using both quotation and apostrophes in string." );
                    case html5::AttrBoundaryChar::QUOTATION_MARK:
                        break;
                }
            }
            break;

            default:
                break;
        }
    }

    return bc;
}