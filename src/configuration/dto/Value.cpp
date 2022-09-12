#include "Value.h"

#include "../../unicode/utf8.h"
#include "../exception/value_type_mismatch.h"

using namespace blogator::configuration;

/**
 * Constructor (default)
 * @param flags Value flags indicating property of the value added (default: 0x00)
 */
Value::Value( Value::Flags_t flags ) :
    _vtype( Type_e::VOID )
{
    if( flags & IS_ARRAY_VALUE ) {
        _vtype = Type_e::ARRAY;
        _value.emplace<std::unique_ptr<Array_t>>( std::make_unique<Array_t>() );

    } else {
        _value.emplace<bool>( false );
    }
}

/**
 * Constructor
 * @param value Boolean value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 */
Value::Value( bool value, Value::Flags_t flags ) :
    _vtype( Type_e::BOOLEAN )
{
    if( flags & IS_ARRAY_VALUE ) {
        _vtype = Type_e::ARRAY;
        _value.emplace<std::unique_ptr<Array_t>>( std::make_unique<Array_t>() );
        auto & array = std::get<std::unique_ptr<Array_t>>( _value );
        array->emplace_back( Value( value ) );

    } else {
        _value.emplace<bool>( value );
    }
}

/**
 * Constructor
 * @param value Integer value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 */
Value::Value( int64_t value, Value::Flags_t flags ) :
    _vtype( Type_e::INTEGER )
{
    if( flags & IS_ARRAY_VALUE ) {
        _vtype = Type_e::ARRAY;
        _value.emplace<std::unique_ptr<Array_t>>( std::make_unique<Array_t>() );
        auto & array = std::get<std::unique_ptr<Array_t>>( _value );
        array->emplace_back( Value( value ) );

    } else {
        _value.emplace<int64_t>( value );
    }
}

/**
 * Constructor
 * @param value Float value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 */
Value::Value( double value, Value::Flags_t flags ) :
    _vtype( Type_e::FLOAT )
{
    if( flags & IS_ARRAY_VALUE ) {
        _vtype = Type_e::ARRAY;
        _value.emplace<std::unique_ptr<Array_t>>( std::make_unique<Array_t>() );
        auto & array = std::get<std::unique_ptr<Array_t>>( _value );
        array->emplace_back( Value( value ) );

    } else {
        _value.emplace<double>( value );
    }
}

/**
 * Constructor
 * @param value String value
 * @param is_name Flag to signal that the string is a named reference
 * @param flags Value flags indicating property of the value added (default: 0x00)
 */
Value::Value( const char32_t * value, Value::Flags_t flags ) :
    Value( std::u32string( value ), flags )
{}

/**
 * Constructor
 * @param value String value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 */
Value::Value( std::u32string value, Value::Flags_t flags ) :
    _vtype( ( flags & IS_NAMED_VALUE ? Type_e::NAME : Type_e::STRING ) )
{
    if( flags & IS_ARRAY_VALUE ) {
        _vtype = Type_e::ARRAY;
        _value.emplace<std::unique_ptr<Array_t>>( std::make_unique<Array_t>() );
        auto & array = std::get<std::unique_ptr<Array_t>>( _value );
        array->emplace_back( Value( value, ( IS_NAMED_VALUE & flags ) ) );

    } else {
        _value.emplace<std::u32string>( std::move( value ) );
    }
}

/**
 * Move-constructor
 * @param value Value to move over
 */
Value::Value( Value &&value ) noexcept :
    _vtype( value._vtype ),
    _value( std::move( value._value ) )
{}

/**
 * Copy-constructor
 * @param value Value to copy over
 */
Value::Value( const Value &value ) :
    _vtype( value._vtype )
{
    switch( value.type()  ) {
        case ValueType::VOID:    { _value = Variant_t();                                                                      } break;
        case ValueType::NAME:    { _value.emplace<std::u32string>( value.getString() );                                       } break;
        case ValueType::BOOLEAN: { _value.emplace<bool>( value.getBool() );                                                   } break;
        case ValueType::INTEGER: { _value.emplace<int64_t>( value.getInt() );                                                 } break;
        case ValueType::FLOAT:   { _value.emplace<double>( value.getDouble() );                                               } break;
        case ValueType::STRING:  { _value.emplace<std::u32string>( value.getString() );                                       } break;
        case ValueType::ARRAY:   { _value.emplace<std::unique_ptr<Array_t>>( std::make_unique<Array_t>( value.getArray() ) ); } break;
    }
}

/**
 * Destructor
 */
Value::~Value() = default;

/**
 * Move-operator
 * @param rhs Value to move
 * @return Moved value
 */
Value & Value::operator =( Value &&rhs ) noexcept {
    _vtype = rhs._vtype;
    _value = std::move( rhs._value );
    return *this;
}

/**
 * Copy-operator
 * @param rhs Value to copy
 * @return Copied value
 */
Value & Value::operator =( const Value &rhs ) {
    *this = rhs.clone();
    return *this;
}

/**
 * Clone the value
 * @return Cloned value
 */
Value Value::clone() const {
    return { *this };
}

/**
 * Equality operator
 * @param rhs Value to compare with
 * @return Equality state
 */
bool Value::operator ==( const Value &rhs ) const {
    if( this->type() == rhs.type() ) {
        if( _vtype == Type_e::ARRAY ) {
            const auto & arr1 = this->getArray();
            const auto & arr2 = rhs.getArray();

            for( auto it1 = arr1.cbegin(), it2 = arr2.cbegin(); it1 < arr1.cend() && it2 < arr2.cend(); ++it1, ++it2 ) {
                if( *it1 != *it2 ) {
                    return false;
                }
            }

            return true;

        } else {
            return this->_value == rhs._value;
        }
    }

    return false;
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
 * Gets the value array
 * @return Reference to the Value array
 */
const Value::Array_t & Value::getArray() const {
    return const_cast<Value *>( this )->getArray();
}

/**
 * Gets the value array
 * @return Reference to the Value array
 */
Value::Array_t & Value::getArray() {
    if( _vtype != ValueType::ARRAY ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getArray()] Wrong value type: " + blogator::to_string( _vtype )
        );
    }

    try {
        auto & ptr = std::get<std::unique_ptr<Array_t>>( _value );

        if( !ptr ) {
            ptr = std::make_unique<Array_t>();
        }

        return *ptr;

    } catch( const std::bad_variant_access &e ) {
        throw exception::value_type_mismatch(
            "[configuration::Value::getArray()] Value variant type mismatch"
        );
    }
}

/**
 * Converts a value into an array with its old self as an item within
 * @param value Value to convert (a VOID value will just be converted into an empty array)
 */
void Value::convertToArray( Value & value ) { //TODO unit test
    auto old_value = std::move( value );
    value = Value( Value::IS_ARRAY_VALUE );

    if( old_value.type() != Type_e::VOID ) {
        auto &arr = value.getArray();
        arr.emplace_back( std::move( old_value ) );
    }
}

/**
 * Print to a stream (UTF-8)
 * @param os Output stream
 * @param value Value to print
 * @param quote Value types to double quote (Type_e::ARRAY has no effect)
 * @return Output stream
 */
std::ostream & Value::print( std::ostream & os, const Value & value, const std::set<Type_e> & quote ) {
    const auto quote_value = ( value.type() != Type_e::ARRAY && quote.contains( value.type() ) );

    if( quote_value ) {
        os << R"(")";
    }

    switch( value.type() ) {
        case ValueType::VOID   : { os << "null";                                                            } break;
        case ValueType::NAME   : { os << unicode::utf8::convert( value.getString() );                       } break;
        case ValueType::BOOLEAN: { os << ( value.getBool() ? "true" : "false" );                            } break;
        case ValueType::INTEGER: { os << std::to_string( value.getInt() );                                  } break;
        case ValueType::FLOAT  : { os << std::to_string( value.getDouble() );                               } break;

        case ValueType::STRING : {
            if( quote_value ) {
                os << R"(\")" << unicode::utf8::convert( value.getString() ) << R"(\")";
            } else {
                os << R"(")" << unicode::utf8::convert( value.getString() ) << R"(")";
            }
        } break;

        case ValueType::ARRAY  : {
            const auto & array = value.getArray();

            if( array.empty() ) {
                os << "[]";
            } else {
                os << "[ ";

                for( size_t i = 0; i < array.size(); ++i ) {
                    Value::print( os, array.at( i ), quote );

                    if( i < ( array.size() - 1 ) ) {
                        os << ", ";
                    }
                }

                os << " ]";
            }
        } break;
    }

    if( quote_value ) {
        os << R"(")";
    }

    return os;
}

/**
 * Output stream operator
 * @param os Output stream
 * @param value configuration:Value to print
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const Value &value ) {
    Value::print( os, value );
    return os;
}

/**
 * Output stream operator
 * @param os Output stream
 * @param value configuration:Value::Array_t to print
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const Value::Array_t &array ) {
    if( array.empty() ) {
        os << "[]";

    } else {
        os << "[ ";

        for( size_t i = 0; i < array.size(); ++i ) {
            os << array.at( i );

            if( i < ( array.size() - 1 ) ) {
                os << ", ";
            }
        }

        os << " ]";
    }

    return os;
}
