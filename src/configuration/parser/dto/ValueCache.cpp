#include "ValueCache.h"

using namespace blogator::configuration::parser;

/**
 * Constructor
 * @param root Root value object
 * @param position Position of root value object in source text
 */
ValueCache::ValueCache( blogator::configuration::Value &&root, TextPos position ) :
    position( position ),
    root( std::make_unique<Value>( std::move( root ) ) ),
    hierarchy( { *this->root } )
{}

/**
 * Converts the value at the current context into an array and place original value within it
 * @return Success (current context value converted to ARRAY)
 */
bool ValueCache::convertCurrentContextToArray() {
    if( hierarchy.empty() ) {
        return false;
    }

    auto &     curr_ctx_value = hierarchy.back().get();
    const auto curr_is_void   = ( curr_ctx_value.type() == Value::Type_e::VOID );

    Value::convertToArray( curr_ctx_value );

    if( curr_is_void ) {
        auto & ctx_ref    = hierarchy.back();
        auto & arr_in_ctx = ctx_ref.get().getArray();

        arr_in_ctx.emplace_back( Value() );
    }


//    if( root->type() != Value::Type_e::ARRAY ) {
//        const auto is_void = ( root->type() == Value::Type_e::VOID );
//
//        Value::convertToArray( *root );
//
//        if( is_void ) {
//            auto & ctx_ref    = hierarchy.back();
//            auto & arr_in_ctx = ctx_ref.get().getArray();
//
//            arr_in_ctx.emplace_back( Value() );
//        }
//
//        return true; //EARLY RETURN
//    }

    return true;
}

/**
 * Creates an array inside the current array
 * @return Array creation success
 */
bool ValueCache::createArray() {
    if( root && root->type() == Value::Type_e::ARRAY ) {
        auto &curr_val = hierarchy.back().get();
        auto &curr_arr = curr_val.getArray();
        hierarchy.emplace_back( curr_arr.emplace_back( Value( Value::IS_ARRAY_VALUE ) ) );
        return true;
    }

    return false;
}

/**
 * Closes the current array in the hierarchy (effective up to but excluding root)
 * @return Close success
 */
bool ValueCache::closeArray() {
    if( hierarchy.size() > 1 ) {
        hierarchy.pop_back();
        return true;
    }

    return false;
}

/**
 * Adds a value to current array context (auto sets as root if not initialised and, for any further calls, auto converts root if not ARRAY)
 * @param value Value to add
 * @param pos Position of value in source text
 */
void ValueCache::addValueToCurrentContext( blogator::configuration::Value &&value, const TextPos &pos ) {
    if( root ) {
        if( root->type() != Value::Type_e::ARRAY ) {
            Value::convertToArray( *root );
        }

        auto & ctx_ref    = hierarchy.back();
        auto & arr_in_ctx = ctx_ref.get().getArray();
        auto & new_value  = arr_in_ctx.emplace_back( std::move( value ) );

        if( new_value.type() == Value::Type_e::ARRAY ) {
            hierarchy.emplace_back( new_value );
        }

    } else {
        root     = std::make_unique<Value>( std::move( value ) );
        position = pos;
        hierarchy.emplace_back( *root );
    }
}

/**
 * Gets the root value's type
 * @return Root Value::Type_e
 */
blogator::configuration::Value::Type_e ValueCache::rootType() const {
    return ( root ? root->type() : Value::Type_e::VOID );
}

/**
 * Gets the initialised state of the cache
 * @return Initialised state of cache
 */
bool ValueCache::initialised() const {
    return root.operator bool();
}