#ifndef BLOGATOR_CONFIGURATION_VALUE_H
#define BLOGATOR_CONFIGURATION_VALUE_H

#include <ostream>
#include <variant>
#include <vector>
#include <set>
#include <memory>

#include "../enum/ValueType.h"

namespace blogator::configuration {
    class Value {
      public:
        typedef std::vector<Value>                     Array_t;
        typedef uint8_t                                Flags_t;
        typedef ValueType                              Type_e;
        typedef std::variant<bool,
                             int64_t,
                             double,
                             std::u32string,
                             std::unique_ptr<Array_t>> Variant_t;


        static constexpr uint8_t IS_ARRAY_VALUE = { 0b01 };
        static constexpr uint8_t IS_NAMED_VALUE = { 0b10 };

        explicit Value( Flags_t flags = 0b00 );
        explicit Value( bool value, Flags_t flags = 0b00 );
        explicit Value( int64_t value, Flags_t flags = 0b00 );
        explicit Value( double value, Flags_t flags = 0b00 );
        explicit Value( std::u32string value, Flags_t flags = 0b00 );
        explicit Value( const char32_t * value, Flags_t flags = 0b00 );
        Value( Value && value ) noexcept ;
        Value( const Value & value );
        ~Value();

        Value & operator =( Value && rhs ) noexcept;
        Value & operator =( const Value & rhs );

        [[nodiscard]] Value clone() const;

        bool operator ==( const Value & rhs ) const;
        bool operator !=( const Value & rhs ) const;

        [[nodiscard]] Type_e type() const;

        [[nodiscard]] bool getBool() const;
        [[nodiscard]] int64_t getInt() const;
        [[nodiscard]] double getDouble() const;
        [[nodiscard]] const std::u32string & getString() const;
        [[nodiscard]] const Array_t & getArray() const;
        Array_t & getArray();

        static std::ostream & print( std::ostream & os, const Value & value, const std::set<Type_e>& quote = {} );
        static void convertToArray( Value & value );

      private:
        Type_e    _vtype;
        Variant_t _value;
    };

    std::ostream & operator <<( std::ostream & os, const Value & value );
    std::ostream & operator <<( std::ostream & os, const Value::Array_t & array );
}

#endif //BLOGATOR_CONFIGURATION_VALUE_H