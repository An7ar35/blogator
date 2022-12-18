#include "gtest/gtest.h"
#include "../../src/datastructure/MessageQueue.h"

#include <thread>
#include <syncstream>

using namespace blogator;

namespace blogator_tests::datastructure::MessageQueue {
    using blogator::MessageQueue;

    using ProducerProperties_t = std::vector<std::tuple<std::string, size_t>>;
    using ConsumerProperties_t = std::vector<std::tuple<std::string>>;

    /**
     * Dummy object
     */
    struct Obj {
        explicit Obj( uint64_t num ) : i( num ) {}
        ~Obj() { ++destroyed_count; }

        uint64_t i;

        static size_t destroyed_count;
        static void initCounter() { destroyed_count = 0; }
    };

    size_t Obj::destroyed_count { 0 };

    /**
     * Message Producer
     * @tparam Q_SIZE   MessageQueue buffer size
     * @tparam Q_TYPE   MessageQueue data type
     * @param queue     Message queue
     * @param thread_id Thread ID
     * @param msg_count Number of data messages to enqueue
     */
    template<size_t Q_SIZE, typename Q_TYPE> void produce(
        MessageQueue<Q_SIZE, Q_TYPE> & queue,
        const std::string            & thread_id,
        size_t                         msg_count )
    {
        for( size_t i = 0; i < msg_count; ++i ) {
            std::stringstream ss;
            ss << "Thread " << thread_id << ": msg #" << std::to_string( i );
            auto ptr = std::make_unique<std::string>( ss.str() );
            while( !queue.enqueue( ptr ) );
        }
    };

    /**
     * Message Consumer
     * @tparam Q_SIZE   MessageQueue buffer size
     * @tparam Q_TYPE   MessageQueue data type
     * @param queue     Message queue
     * @param thread_id Thread ID
     * @param counter   Dequeued message counter
     * @param run_flag  Run flag
     */
    template<size_t Q_SIZE, typename Q_TYPE> void consume(
        MessageQueue<Q_SIZE, Q_TYPE> & queue,
        const std::string            & thread_id [[maybe_unused]],
        std::atomic<size_t>          & counter,
        std::atomic<bool>            & run_flag  )
    {
        size_t local_counter = 0;
        auto   ret           = queue.dequeue();

        do {
            if( ret.has_value() ) {
                ++local_counter;
                ASSERT_NE( ret.value().get(), nullptr );
            }

        } while( ( ret = queue.dequeue() ).has_value() || run_flag.load( std::memory_order_acq_rel ) );

        counter.fetch_add( local_counter );
    };
}

TEST( MessageQueue_Tests, size_1 ) {
    using namespace blogator_tests::datastructure::MessageQueue;

    auto queue = MessageQueue<1, int>();
    ASSERT_EQ( queue.size(), 1 );
}

TEST( MessageQueue_Tests, size_2 ) {
    using namespace blogator_tests::datastructure::MessageQueue;

    auto queue = MessageQueue<10, int>();
    ASSERT_EQ( queue.size(), 10 );
}

TEST( MessageQueue_Tests, default_deleter ) {
    using namespace blogator_tests::datastructure::MessageQueue;

    const size_t msg_count = 10;

    Obj::initCounter();

    {
        auto queue = MessageQueue<50, Obj>();
        ASSERT_TRUE( queue.empty() );

        for( size_t i = 0; i < msg_count; ++i ) {
            queue.enqueue( std::make_unique<Obj>( i ) );
        }

        ASSERT_FALSE( queue.empty() );
    }

    ASSERT_EQ( Obj::destroyed_count, msg_count );
}

TEST( MessageQueue_Tests, custom_deleter ) {
    using namespace blogator_tests::datastructure::MessageQueue;

    struct CustomDeleter {
        void operator()( Obj * ptr ) {
            delete ptr;
        }
    };

    const size_t msg_count = 10;

    Obj::initCounter();

    {
        auto queue = MessageQueue<50, Obj, CustomDeleter>();
        ASSERT_TRUE( queue.empty() );

        for( size_t i = 0; i < msg_count; ++i ) {
            queue.enqueue( std::make_unique<Obj>( i ) );
        }

        ASSERT_FALSE( queue.empty() );
    }

    ASSERT_EQ( Obj::destroyed_count, msg_count );

}

TEST( MessageQueue_Tests, synchronised_single_message_0 ) {
    auto queue = MessageQueue<50, size_t>();
    ASSERT_TRUE( queue.empty() );

    { //enqueue
        queue.enqueue( std::make_unique<size_t>( 2022 ) );
    }

    ASSERT_FALSE( queue.empty() );

    { //dequeue
        auto ret = queue.dequeue();
        ASSERT_TRUE( ret.has_value() );
        ASSERT_EQ( *ret.value(), 2022 );
    }

    ASSERT_TRUE( queue.empty() );
}

TEST( MessageQueue_Tests, synchronised_multiple_messages_0 ) { // single thread - single enqueue then dequeue
    auto queue = MessageQueue<50, size_t>();

    ASSERT_TRUE( queue.empty() );

    queue.enqueue( std::make_unique<size_t>( 2022 ) );

    ASSERT_FALSE( queue.empty() );

    auto ret1 = queue.dequeue();
    ASSERT_TRUE( ret1.has_value() );
    ASSERT_EQ( *ret1.value(), 2022 );

    ASSERT_TRUE( queue.empty() );
    auto ret2 = queue.dequeue();
    ASSERT_FALSE( ret2.has_value() );
}

TEST( MessageQueue_Tests, synchronised_multiple_messages_1 ) { // single thread - full queue enqueue then dequeue
    auto queue = MessageQueue<50, size_t>();

    ASSERT_TRUE( queue.empty() );

    //Filling up the queue
    for( size_t i = 0; i < 50; ++i ) {
        queue.enqueue( std::make_unique<size_t>( i ) );
    }

    ASSERT_FALSE( queue.empty() );

    for( size_t i = 0; i < 50; ++i ) {
        auto ret = queue.dequeue();
        ASSERT_TRUE( ret.has_value() );
        ASSERT_EQ( *ret.value(), i );
    }

    ASSERT_TRUE( queue.empty() );
    auto ret = queue.dequeue();
    ASSERT_FALSE( ret.has_value() );
}

TEST( MessageQueue_Tests, synchronised_multiple_messages_2 ) { // single thread - +full, -half, +half, -full
    auto queue = MessageQueue<50, size_t>();

    ASSERT_TRUE( queue.empty() );

    { //Filling up the queue
        for( size_t i = 0; i < 50; ++i ) {
            queue.enqueue( std::make_unique<size_t>( i ) );
        }
    }

    { //Emptying up the queue half way
        for( size_t i = 0; i < 25; ++i ) {
            auto ret = queue.dequeue();
            ASSERT_TRUE( ret.has_value() );
            ASSERT_EQ( *ret.value(), i );
        }
    }

    { //Filling up the queue again
        for( size_t i = 50; i < 75; ++i ) {
            queue.enqueue( std::make_unique<size_t>( i ) );
        }
    }

    { //Emptying up the queue completely
        for( size_t i = 25; i < 75; ++i ) {
            auto ret = queue.dequeue();
            ASSERT_TRUE( ret.has_value() );
            ASSERT_EQ( *ret.value(), i );
        }
    }

    ASSERT_TRUE( queue.empty() );
    auto ret = queue.dequeue();
    ASSERT_FALSE( ret.has_value() );
}

TEST( MessageQueue_Tests, low_concurrent_thrashing_0 ) { //1 producer and 1 consumer, msg < queue size
    auto queue = MessageQueue<100, std::string>();

    size_t total_msgs      = 50;
    auto   dequeue_counter = std::atomic<size_t>( 0 );
    auto   run_flag        = std::atomic_bool( true );

    auto consumer = std::thread( [&queue, &dequeue_counter, &run_flag]() {
        auto ret = queue.dequeue();

        do {
            if( ret.has_value() ) {
                ++dequeue_counter;
                ASSERT_NE( ret.value().get(), nullptr );
            }

        } while( ( ret = queue.dequeue() ).has_value() || run_flag.load( std::memory_order_acq_rel ) );
    } );

    auto producer = std::thread( [&queue, &total_msgs]() {
        for( size_t i = 0; i < total_msgs; ++i ) {
            std::stringstream ss;
            ss << "Thread " << "msg #" << std::to_string( i );
            while( !queue.enqueue( std::make_unique<std::string>( ss.str() ) ) );
        }
    } );

    producer.join();
    run_flag.store( false, std::memory_order_release );
    consumer.join();

    ASSERT_EQ( dequeue_counter.load(), total_msgs );
}

TEST( MessageQueue_Tests, low_concurrent_thrashing_1 ) { //1 producer and 1 consumer, msg > queue size
    auto queue = MessageQueue<50, uint64_t>();

    size_t total_msgs      = 200;
    auto   dequeue_counter = std::atomic<size_t>( 0 );
    auto   run_flag        = std::atomic_bool( true );

    auto producer = std::thread( [&queue, &total_msgs]() {
        for( size_t i = 0; i < total_msgs; ++i ) {
            while( !queue.enqueue( std::make_unique<uint64_t>( i ) ) );
        }
    } );

    auto consumer = std::thread( [&queue, &dequeue_counter, &run_flag]() {
        uint64_t msg_num = 0;
        auto     ret     = queue.dequeue();

        do {
            if( ret.has_value() ) {
                ++dequeue_counter;
                ASSERT_NE( ret.value().get(), nullptr );
                ASSERT_EQ( *ret.value(), msg_num++ );
            }

        } while( ( ret = queue.dequeue() ).has_value() || run_flag.load( std::memory_order_acq_rel ) );
    } );

    producer.join();
    run_flag.store( false, std::memory_order_release );
    consumer.join();

    ASSERT_EQ( dequeue_counter.load(), total_msgs );
}

TEST( MessageQueue_Tests, low_concurrent_thrashing_2 ) { //2 consumer on full queue
    auto queue = MessageQueue<100, uint64_t>();

    size_t total_msgs          = 100;
    auto   dequeue_counter     = std::atomic<size_t>( 0 );
    auto   run_flag            = std::atomic_bool( true );
    auto   consumer_properties = std::vector<std::string>( { "C1", "C2" } );
    auto   consumer_threads    = std::vector<std::thread>();

    const auto consume = [&queue, &dequeue_counter, &run_flag]( const std::string & thread_id ) {
        auto ret = queue.dequeue();

        do {
            if( ret.has_value() ) {
                ++dequeue_counter;
                ASSERT_NE( ret.value().get(), nullptr );
            }

        } while( ( ret = queue.dequeue() ).has_value() || run_flag.load( std::memory_order_acq_rel ) );
    };

    for( size_t i = 0; i < total_msgs; ++i ) {
        while( !queue.enqueue( std::make_unique<uint64_t>( i ) ) );
    }

    for( auto & property : consumer_properties ) {
        consumer_threads.emplace_back( std::thread( consume, property ) );
    }

    run_flag.store( false, std::memory_order_release );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_msgs );
}

TEST( MessageQueue_Tests, low_concurrent_thrashing_3 ) { //2 producers and 1 consumer, msg > queue size
    auto         queue               = MessageQueue<100, std::string>();
    auto         run_flag            = std::atomic_bool( true );
    auto         dequeue_counter     = std::atomic<size_t>( 0 );
    size_t       total_produced_msgs = 0;
    auto         consumer_threads    = std::vector<std::thread>();
    auto         producer_threads    = std::vector<std::thread>();

    auto producer_properties = std::vector<std::tuple<std::string, size_t>>( { { "P1", 100 }, { "P2",  100 } } );
    auto consumer_properties = std::vector<std::string>( { "C1" } );

    const auto produce = [&queue, &dequeue_counter]( const std::string& thread_id, size_t enq_n ) {
        size_t local_dequeue_counter = 0;

        for( size_t i = 0; i < enq_n; ++i ) {
            std::stringstream ss;
            ss << "Thread " << thread_id << ": msg #" << std::to_string( i );
            auto ptr = std::make_unique<std::string>( ss.str() );
            while( !queue.enqueue( ptr ) );
        }

        dequeue_counter += local_dequeue_counter;
    };

    const auto consume = [&queue, &dequeue_counter, &run_flag]( [[maybe_unused]] const std::string &thread_name ) {
        auto ret = queue.dequeue();

        do {
            if( ret.has_value() ) {
                ++dequeue_counter;
                ASSERT_NE( ret.value().get(), nullptr );
            }

        } while( ( ret = queue.dequeue() ).has_value() || run_flag.load( std::memory_order_acq_rel ) );
    };

    for( auto & property : producer_properties ) {
        producer_threads.emplace_back( std::thread( produce, std::get<0>( property ), std::get<1>( property ) ) );
        total_produced_msgs += std::get<1>( property );
    }

    for( auto & thread_name : consumer_properties ) {
        consumer_threads.emplace_back( std::thread( consume, thread_name ) );
    }

    for( auto & th : producer_threads ) {
        th.join();
    }

    run_flag.store( false );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_produced_msgs );
}

TEST( MessageQueue_Tests, low_concurrent_thrashing_4 ) { //1 producers and 2 consumer, msg > queue size
    using namespace blogator_tests::datastructure::MessageQueue;

    const size_t queue_size          = 100;
    auto         queue               = MessageQueue<queue_size, std::string>();
    auto         run_flag            = std::atomic_bool( true );
    auto         dequeue_counter     = std::atomic<size_t>( 0 );
    size_t       total_produced_msgs = 0;
    auto         consumer_threads    = std::vector<std::thread>();
    auto         producer_threads    = std::vector<std::thread>();

    auto producer_properties = ProducerProperties_t( {
        { "P1", 400 }
    } );
    auto consumer_properties = ConsumerProperties_t( {
        { "C1" },
        { "C2" }
    } );

    for( auto & property : producer_properties ) {
        producer_threads.emplace_back(
            std::thread(
                produce<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::get<1>( property ) )
        );
        total_produced_msgs += std::get<1>( property );
    }

    for( auto & property : consumer_properties ) {
        consumer_threads.emplace_back(
            std::thread(
                consume<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::ref( dequeue_counter ),
                std::ref( run_flag ) )
        );
    }

    for( auto & th : producer_threads ) {
        th.join();
    }

    run_flag.store( false );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_produced_msgs );
}

TEST( MessageQueue_Tests, low_concurrent_thrashing_5 ) { //many consumers/producers
    using namespace blogator_tests::datastructure::MessageQueue;

    const size_t queue_size          = 100;
    auto         queue               = MessageQueue<queue_size, std::string>();
    auto         run_flag            = std::atomic_bool( true );
    auto         dequeue_counter     = std::atomic<size_t>( 0 );
    size_t       total_produced_msgs = 0;
    auto         consumer_threads    = std::vector<std::thread>();
    auto         producer_threads    = std::vector<std::thread>();

    auto producer_properties = ProducerProperties_t( {
        { "P1", 100 },
        { "P2",  50 },
        { "P3",  80 }
    } );
    auto consumer_properties = ConsumerProperties_t( {
        { "C1" },
        { "C2" },
        { "C3" }
    } );

    for( auto & property : producer_properties ) {
        producer_threads.emplace_back(
            std::thread(
                produce<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::get<1>( property ) )
        );
        total_produced_msgs += std::get<1>( property );
    }

    for( auto & property : consumer_properties ) {
        consumer_threads.emplace_back(
            std::thread(
                consume<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::ref( dequeue_counter ),
                std::ref( run_flag ) )
        );
    }

    for( auto & th : producer_threads ) {
        th.join();
    }

    run_flag.store( false );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_produced_msgs );
}

TEST( MessageQueue_Tests, high_concurrent_thrashing_1 ) { //1 consumers, many producers
    using namespace blogator_tests::datastructure::MessageQueue;

    const size_t queue_size          = 2'000'000;
    auto         queue               = MessageQueue<queue_size, std::string>();
    auto         run_flag            = std::atomic_bool( true );
    auto         dequeue_counter     = std::atomic<size_t>( 0 );
    size_t       total_produced_msgs = 0;
    auto         consumer_threads    = std::vector<std::thread>();
    auto         producer_threads    = std::vector<std::thread>();

    auto producer_properties = ProducerProperties_t( {
        { "P1", 10'000'000 },
        { "P2",  5'000'000 },
        { "P3",  8'000'000 },
        { "P4", 10'000'000 },
        { "P5",  2'000'000 },
        { "P6",  5'000'000 }
    } );
    auto consumer_properties = ConsumerProperties_t( {
        { "C1" }
    } );

    for( auto & property : producer_properties ) {
        producer_threads.emplace_back(
            std::thread(
                produce<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::get<1>( property ) )
        );
        total_produced_msgs += std::get<1>( property );
    }

    for( auto & property : consumer_properties ) {
        consumer_threads.emplace_back(
            std::thread(
                consume<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::ref( dequeue_counter ),
                std::ref( run_flag ) )
        );
    }

    for( auto & th : producer_threads ) {
        th.join();
    }

    run_flag.store( false );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_produced_msgs );
}

TEST( MessageQueue_Tests, high_concurrent_thrashing_2 ) { //many consumers/producers
    using namespace blogator_tests::datastructure::MessageQueue;

    const size_t queue_size          = 1'000'000;
    auto         queue               = MessageQueue<queue_size, std::string>();
    auto         run_flag            = std::atomic_bool( true );
    auto         dequeue_counter     = std::atomic<size_t>( 0 );
    size_t       total_produced_msgs = 0;
    auto         consumer_threads    = std::vector<std::thread>();
    auto         producer_threads    = std::vector<std::thread>();

    auto producer_properties = ProducerProperties_t( {
        { "P1", 10'000'000 },
        { "P2",  5'000'000 },
        { "P3",  8'000'000 },
        { "P4", 10'000'000 }
    } );
    auto consumer_properties = ConsumerProperties_t( {
        { "C1" },
        { "C2" },
        { "C3" },
        { "C4" }
    } );

    for( auto & property : producer_properties ) {
        producer_threads.emplace_back(
            std::thread(
                produce<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::get<1>( property ) )
        );
        total_produced_msgs += std::get<1>( property );
    }

    for( auto & property : consumer_properties ) {
        consumer_threads.emplace_back(
            std::thread(
                consume<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::ref( dequeue_counter ),
                std::ref( run_flag ) )
        );
    }

    for( auto & th : producer_threads ) {
        th.join();
    }

    run_flag.store( false );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_produced_msgs );
}

TEST( MessageQueue_Tests, high_concurrent_thrashing_3 ) { //many consumers, 1 producer
    using namespace blogator_tests::datastructure::MessageQueue;

    const size_t queue_size          = 500'000;
    auto         queue               = MessageQueue<queue_size, std::string>();
    auto         run_flag            = std::atomic_bool( true );
    auto         dequeue_counter     = std::atomic<size_t>( 0 );
    size_t       total_produced_msgs = 0;
    auto         consumer_threads    = std::vector<std::thread>();
    auto         producer_threads    = std::vector<std::thread>();

    auto producer_properties = ProducerProperties_t( {
        { "P1", 10'000'000 }
    } );
    auto consumer_properties = ConsumerProperties_t( {
        { "C1" },
        { "C2" },
        { "C3" },
        { "C4" },
        { "C5" },
        { "C6" },
    } );

    for( auto & property : producer_properties ) {
        producer_threads.emplace_back(
            std::thread(
                produce<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::get<1>( property ) )
        );
        total_produced_msgs += std::get<1>( property );
    }

    for( auto & property : consumer_properties ) {
        consumer_threads.emplace_back(
            std::thread(
                consume<queue_size, std::string>,
                std::ref( queue ),
                std::get<0>( property ),
                std::ref( dequeue_counter ),
                std::ref( run_flag ) )
        );
    }

    for( auto & th : producer_threads ) {
        th.join();
    }

    run_flag.store( false );

    for( auto & th : consumer_threads ) {
        th.join();
    }

    ASSERT_EQ( dequeue_counter.load(), total_produced_msgs );
}