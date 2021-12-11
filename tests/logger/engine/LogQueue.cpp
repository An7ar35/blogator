#include "gtest/gtest.h"
#include "../../../src/logger/engine/LogQueue.h"

using blogator::logger::engine::LogQueue;

TEST( logger_LogQueue_Tests, enqueue_dequeue_1 ) {
    auto       q = LogQueue<std::string>();
    const auto expected = "my data";

    q.enqueue( expected );
    ASSERT_EQ( q.size(), 1 );

    auto returned = q.dequeue();
    ASSERT_EQ( expected, *returned );
    ASSERT_EQ( q.size(), 0 );
}

TEST( logger_LogQueue_Tests, enqueue_dequeue_2 ) {
    auto       q = LogQueue<std::string>();
    const auto expected1 = "my data 1";
    const auto expected2 = "my data 2";

    q.enqueue( expected1 );
    q.enqueue( expected2 );
    ASSERT_EQ( q.size(), 2 );

    auto returned1 = q.dequeue();
    auto returned2 = q.dequeue();
    ASSERT_EQ( expected1, *returned1 );
    ASSERT_EQ( expected2, *returned2 );
    ASSERT_EQ( q.size(), 0 );
}

TEST( logger_LogQueue_Tests, ready_callback ) {
    const int  enqueue_count  = 3;
    int        returned_count = 0;
    auto       q              = LogQueue<std::string>();
    const auto callback       = std::function<void()>( [&returned_count]() { returned_count++; } );

    q.setReadyCallback( callback );

    for( auto i = 0; i < enqueue_count; ++i ) {
        q.enqueue( std::to_string( i ) );
    }

    ASSERT_EQ( returned_count, enqueue_count );
}