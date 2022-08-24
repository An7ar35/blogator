#include "Value.h"

#include "../../unicode/utf8.h"
#include "../exception/value_type_mismatch.h"

using namespace blogator::configuration;

/**
 * Constructor
 * @param value Boolean value
 */
Value::Value( bool value ) :
    _vtype( Type_e::BOOLEAN )
{
    _value.emplace<bool>( value );
}

/**
 * Constructor
 * @param value Integer value
 */
Value::Value( int64_t value ) :
    _vtype( Type_e::INTEGER )
{
    _value.emplace<int64_t>( value );
}

/**
 * Constructor
 * @param value Float value
 */
Value::Value( double value ) :
    _vtype( Type_e::FLOAT )
{
    _value.emplace<double>( value );
}

/**
 * Constructor
 * @param value String value
 * @param is_name Flag to signal that the string is a named reference
 */
Value::Value( const char32_t * value, bool is_name ) :
    Value( std::u32string( value ), is_name )
{}

/**
 * Constructor
 * @param value String value
 * @param is_name Flag to signal that the string is a named reference
 */
Value::Value( std::u32string value, bool is_name ) :
    _vtype( ( is_name ? Type_e::NAME : Type_e::STRING ) )
{
    _value.emplace<std::u32string>( std::move( value ) );
}

/**
 * Equality operator
 * @param rhs Value to compare with
 * @return Equality state
 */
bool Value::operator ==( const Value &rhs ) const {
    return this->type() == rhs.type()
        && this->_value == rhs._value;
}

/**
 * Inequality operator
 * @param rhs Value to compare with
 * @return Inequality state
 */
bool Value::operator !=( const Value &rhs ) const {
    return !( *this == rhs );
}

/**
 * Gets the value type of the value
 * @return ValueType enum
 */
Value::Type_e Value::type() const {
    return _vtype;
}

/**
 * Gets the boolean value
 * @return Boolean value
 * @throws blogator::configuration::exception::value_type_mismatch when value and type do not match
 */
bool Value::getBool() const {
    if( _vtype != ValueType::BOOLEAN ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getBool()] Wrong value type: " + blogator::to_string( _vtype )
        );
    }

    try {
        return std::get<bool>( _value );
    } catch( const std::bad_variant_access &e ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getBool()] Value variant type mismatch"
        );
    }
}

/**
 * Gets the integer value
 * @return Integer value
 * @throws blogator::configuration::exception::value_type_mismatch when value and type do not match
 */
int64_t Value::getInt() const {
    if( _vtype != ValueType::INTEGER ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getInt()] Wrong value type: " + blogator::to_string( _vtype )
        );
    }

    try {
        return std::get<int64_t>( _value );
    } catch( const std::bad_variant_access &e ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getInt()] Value variant type mismatch"
        );
    }
}

/**
 * Gets the double float value
 * @return Double value
 * @throws blogator::configuration::exception::value_type_mismatch when value and type do not match
 */
double Value::getDouble() const {
    if( _vtype != ValueType::FLOAT ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getDouble()] Wrong value type: " + blogator::to_string( _vtype )
        );
    }

    try {
        return std::get<double>( _value );
    } catch( const std::bad_variant_access &e ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getDouble()] Value variant type mismatch"
        );
    }
}

/**
 * Gets the string value
 * @return UTF-32 string value
 * @throws blogator::configuration::exception::value_type_mismatch when value and type do not match
 */
const std::u32string & Value::getString() const {
    if( _vtype != ValueType::NAME && _vtype != ValueType::STRING ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getString()] Wrong value type: " + blogator::to_string( _vtype )
        );
    }

    try {
        return std::get<std::u32string>( _value );
    } catch( const std::bad_variant_access &e ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getString()] Value variant type mismatch"
        );
    }
}

/**
 * Output stream operator
 * @param os Output stream
 * @param value configuration:Value to print
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const Value &value ) {
    switch( value.type() ) {
        case ValueType::NAME   : { os << unicode::utf8::convert( value.getString() );                 } break;
        case ValueType::BOOLEAN: { os << ( value.getBool() ? "true" : "false" ) << "\"";              } break;
        case ValueType::INTEGER: { os << std::to_string( value.getInt() ) << "\"";                    } break;
        case ValueType::FLOAT  : { os << std::to_string( value.getDouble() ) << "\"";                 } break;
        case ValueType::STRING : { os << "\"" << unicode::utf8::convert( value.getString() ) << "\""; } break;
    }

    return os;
}
