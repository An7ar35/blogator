#ifndef BLOGATOR_CONFIGURATION_VALUE_H
#define BLOGATOR_CONFIGURATION_VALUE_H

#include <ostream>
#include <variant>

#include "../enum/ValueType.h"

namespace blogator::configuration {
    class Value {
      public:
        typedef ValueType                                           Type_e;
        typedef std::variant<bool, int64_t, double, std::u32string> Variant_t;

        bool operator ==( const Value & rhs ) const;
        bool operator !=( const Value & rhs ) const;

        explicit Value( bool value );
        explicit Value( int64_t value );
        explicit Value( double value );
        explicit Value( std::u32string value, bool is_name = false );
        explicit Value( const char32_t * value, bool is_name = false );

        [[nodiscard]] Type_e type() const;
        [[nodiscard]] bool getBool() const;
        [[nodiscard]] int64_t getInt() const;
        [[nodiscard]] double getDouble() const;
        [[nodiscard]] const std::u32string & getString() const;

      private:
        Type_e    _vtype;
        Variant_t _value;
    };

    std::ostream & operator <<( std::ostream & os, const Value & value );
}

#endif //BLOGATOR_CONFIGURATION_VALUE_H