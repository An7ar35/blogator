#ifndef BLOGATOR_CONFIGURATION_DTO_VALUESTORE_H
#define BLOGATOR_CONFIGURATION_DTO_VALUESTORE_H

#include <vector>
#include <memory>

#include "Value.h"

namespace blogator::configuration {
    /**
     * ValueStore wrapper
     */
    class ValueStore {
      public:
        typedef std::vector<std::unique_ptr<Value>> ValueArray_t;

        ValueStore() = default;
        ValueStore( ValueStore && vs ) noexcept;
        ValueStore( const ValueStore & vs );

        ValueStore & operator =( ValueStore && rhs ) noexcept ;
        ValueStore & operator =( const ValueStore & rhs );

        ValueArray_t & operator()();
        const ValueArray_t & operator()() const;

        friend std::ostream & operator <<( std::ostream & os, const ValueStore & vs );

        bool operator ==( const ValueStore & rhs ) const;
        bool operator !=( const ValueStore & rhs ) const;

        ValueArray_t & values();
        void emplace_back( std::unique_ptr<Value> && ptr );
        void setArrayFlag( bool flag = true );
        void clear();

        [[nodiscard]] bool isArray() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

      private:
        bool         _is_array { false };
        ValueArray_t _store;
    };

    std::ostream & operator <<( std::ostream & os, const ValueStore & vs );
}

#endif //BLOGATOR_CONFIGURATION_DTO_VALUESTORE_H