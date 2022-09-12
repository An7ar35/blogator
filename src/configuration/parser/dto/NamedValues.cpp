#include "NamedValues.h"

using namespace blogator::configuration::parser;

/**
 * Equality operator
 * @param rhs CfgValue to compare against
 * @return Equality state
 */
bool NamedValues::ValueRef::operator ==( const NamedValues::ValueRef &rhs ) const {
    return pos == rhs.pos
           && &ref.get() == &rhs.ref.get();
}