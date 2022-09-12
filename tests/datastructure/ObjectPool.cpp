#include "gtest/gtest.h"
#include "../../src/datastructure/ObjectPool.h"

using namespace blogator;

TEST( ObjectPool_Tests, constructor_scope_1 ) {
    struct Object { int val { 0 }; };

    auto pool = ObjectPool<Object>();

    ASSERT_EQ( pool.available(), 1 );
    ASSERT_TRUE( ObjectPool<Object>::active( &pool ) );

    {
        auto obj = pool.acquire();
        ASSERT_EQ( pool.available(), 0 );
        ASSERT_THROW( pool.acquire(), blogator::exception::resource_unavailable );
    }

    ASSERT_EQ( pool.available(), 1 );
}

TEST( ObjectPool_Tests, constructor_scope_2 ) {
    struct Object { int val { 0 }; };

    ObjectPool<Object> * ptr = nullptr;

    const auto create = []( ObjectPool<Object> ** pool_ptr ) {
        auto pool = ObjectPool<Object>();
        *pool_ptr = &pool;
        return pool.acquire();
    };

    auto obj = create( &ptr );

    ASSERT_TRUE( obj );
    ASSERT_NE( ptr, nullptr ); //check it was assigned
    ASSERT_FALSE( ObjectPool<Object>::active( ptr ) );

}