#include "Attribute.h"

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