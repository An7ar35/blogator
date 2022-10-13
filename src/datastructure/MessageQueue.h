#ifndef BLOGATOR_DATASTRUCTURE_MESSAGEQUEUE_H
#define BLOGATOR_DATASTRUCTURE_MESSAGEQUEUE_H

#include <memory>
#include <optional>
#include <condition_variable>
#include <functional>
#include <array>

#include "../logger/Logger.h"

namespace blogator {
    /**
     * ADT Message Queue
     * @tparam SIZE Queue size
     * @tparam T Message object type
     * @tparam Deleter Message object type deleter to use (optional)
     */
    template<size_t SIZE, typename T, class Deleter = std::default_delete<T>> class MessageQueue {
      public:
        typedef std::function<void( void )> SignalFunc_t;

        MessageQueue();
        explicit MessageQueue( SignalFunc_t content_sig_fn );
        MessageQueue( const MessageQueue & ) = delete;
        ~MessageQueue();

        MessageQueue & operator =( const MessageQueue & ) = delete;

        void setContentCallback( const SignalFunc_t &cb );

        bool enqueue( std::unique_ptr<T> && data_ptr );
        bool enqueue( std::unique_ptr<T> & data_ptr );
        std::optional<std::unique_ptr<T>> dequeue();

        [[nodiscard]] consteval size_t size() const;
        [[nodiscard]] bool empty() const;

      private:
        /**
         * Node data container
         */
        struct Node {
            ~Node();

            [[nodiscard]] bool hasData() const;

            std::atomic<T *> data;
        };

        // ╔════════════════════════════════╗
        // ║  Nodes cache (circular buffer) ║
        // ╚════════════════════════════════╝
        //
        // Empty:                                Full:
        //      0  1  2  3  4  5  6     n              0  1  2  3  4  5  6     n
        // ╭─>─[ ][ ][ ][ ][ ][ ][ ]...[ ]─>─╮    ╭─>─[x][x][x][x][x][x][x]...[x]─>─╮
        // │    ↑                            │    │       ↑                         │
        // │    front, back                  │    │       back, front               │
        // ╰─<─────────────<──────────────<──╯    ╰─<─────────────<──────────────<──╯
        //
        // Some elements:                        Some elements, wrapped around:
        //      0  1  2  3  4  5  6     n              0  1  2  3  4  5  6     n
        // ╭─>─[ ][x][x][x][x][ ][ ]...[ ]─>─╮    ╭─>─[x][x][ ][ ][ ][ ][x]...[x]─>─╮
        // │       ↑           ↑             │    │          ↑           ↑          │
        // │       front       back          │    │          back        front      │
        // ╰─<─────────────<──────────────<──╯    ╰─<─────────────<──────────────<──╯
        //
        // Enqueue/Dequeue Operations:
        //      0  1  2  3  4  5  6     n
        // ╭─>─[ ][ ][x][x][x][ ][ ]...[ ]─>─╮
        // │ dequeue →|        |→ enqueue    │
        // │          front    back          │
        // ╰─<─────────────<──────────────<──╯
        std::unique_ptr<std::array<Node, SIZE>> _nodes;

        std::atomic<size_t>                     _front_i;
        std::atomic<size_t>                     _pending_back_i;
        std::atomic<size_t>                     _committed_back_i;

        std::function<void( void )>             _consumer_sig_fn;

        std::atomic<size_t>                     _bottleneck_count;
        std::atomic<size_t>                     _bottleneck_max_count;


        [[nodiscard]] bool reachedEnd( size_t index ) const;
        [[nodiscard]] bool emptied( size_t front_i ) const;

        void recordBottleneckEvent();
        void resetBottleneckState();

        static size_t normalize( size_t index, int offset );
    };
}

//==================================[MessageQueue<T>::Node]=========================================

/**
 * [PRIVATE] Destructor
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 */
template<size_t SIZE, typename T, class Deleter> blogator::MessageQueue<SIZE, T, Deleter>::Node::~Node() {
    Deleter()( data.load() );
}

/**
 * [PRIVATE] Checks if node has data
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 */
template<size_t SIZE, typename T, class Deleter> bool blogator::MessageQueue<SIZE, T, Deleter>::Node::hasData() const {
    return ( data.load() != nullptr );
}

//=====================================[MessageQueue<T>]============================================

/**
 * Constructor
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 */
template<size_t SIZE, typename T, class Deleter> blogator::MessageQueue<SIZE, T, Deleter>::MessageQueue() :
    _nodes( std::make_unique<std::array<Node, SIZE>>() ),
    _front_i( 0 ),
    _pending_back_i( 0 ),
    _committed_back_i( 0 ),
    _bottleneck_count( 0 ),
    _bottleneck_max_count( 0 )
{
    static_assert( SIZE > 0, "MessageQueue<SIZE, T, Deleter> size must be > 0!" );

    if( !std::atomic<size_t>::is_always_lock_free ) {
        LOG_WARNING(
            "[MessageQueue<SIZE, T, Deleter>::MessageQueue()] "
            "Atomic operation on MessageQueue size_t not available."
        );
    }
}

/**
 * Constructor
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param content_sig_fn Function to use for signaling content is in the queue
 */
template<size_t SIZE, typename T, class Deleter> blogator::MessageQueue<SIZE, T, Deleter>::MessageQueue( SignalFunc_t content_sig_fn ) :
    _nodes( std::make_unique<std::array<Node, SIZE>>() ),
    _front_i( 0 ),
    _pending_back_i( 0 ),
    _committed_back_i( 0 ),
    _consumer_sig_fn( std::move( content_sig_fn ) ),
    _bottleneck_count( 0 ),
    _bottleneck_max_count( 0 )
{
    static_assert( SIZE > 0, "MessageQueue<SIZE, T, Deleter> size must be > 0!" );

    if( !std::atomic<size_t>::is_always_lock_free ) {
        LOG_WARNING(
            "[MessageQueue<SIZE, T, Deleter>::MessageQueue(..)] "
            "Atomic operation on MessageQueue size_t not available."
        );
    }
}

/**
 * Destructor
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 */
template<size_t SIZE, typename T, class Deleter> blogator::MessageQueue<SIZE, T, Deleter>::~MessageQueue() {
    const auto bottlenecks = _bottleneck_max_count.load();

    if( bottlenecks > 0 ) {
        LOG_WARNING(
            "[MessageQueue<SIZE, T, Deleter>::MessageQueue(..)] ",
            "Bottleneck event(s) observed in queue (size=", SIZE, ", max sequential failed enqueues=", bottlenecks,") - consider increasing its size."
        );
    }
}

/**
 * Sets the content signalling callback function
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param cb Function to use for signaling content is in the queue
 */
template<size_t SIZE, typename T, class Deleter> void blogator::MessageQueue<SIZE, T, Deleter>::setContentCallback( const SignalFunc_t &cb ) {
    _consumer_sig_fn = cb;
}

/**
 * [THREAD-SAFE] Enqueue message object
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param data_ptr Message object pointer (untouched on failure)
 * @return Success
 */
template<size_t SIZE, typename T, class Deleter> bool blogator::MessageQueue<SIZE, T, Deleter>::enqueue( std::unique_ptr<T> && data_ptr ) {
    return enqueue( data_ptr );
}

/**
 * [THREAD-SAFE] Enqueue message object
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param data_ptr Message object pointer (untouched on failure)
 * @return Success
 */
template<size_t SIZE, typename T, class Deleter> bool blogator::MessageQueue<SIZE, T, Deleter>::enqueue( std::unique_ptr<T> & data_ptr ) {
    auto back_i        = _pending_back_i.load();
    bool queue_is_full = false;

    //as long as there are available nodes, keep trying to grab a free one from the 'back'
    while( !( queue_is_full = reachedEnd( back_i ) )
        && !_pending_back_i.compare_exchange_weak( back_i, MessageQueue::normalize( back_i, 1 ) ) )
    {};

    if( !queue_is_full ) {
        auto & node     = _nodes->at( back_i );
        T    * r_ptr    = data_ptr.release();
        T    * null_ptr = nullptr;

        //edge-case: spin lock in the event that a dequeue is currently finishing up on the node
        //           only when the following is true:
        //           a) the queue is full and `_pending_back_i` == `_front_i`,
        //           a) dequeue operation started on _front_i and thus incremented its value,
        //           b) enqueue operation started right before dequeue's data pointer exchange step
        while( !node.data.compare_exchange_weak( ( null_ptr = nullptr ), r_ptr ) );

        //wait for the previous pending node to be committed in order to preserve the queue order
        auto original_back_i = back_i;
        while( !_committed_back_i.compare_exchange_weak( ( original_back_i = back_i ), MessageQueue::normalize( back_i, 1 ) ) );

        if( _consumer_sig_fn ) {
            _consumer_sig_fn();
        }

    } else {
        recordBottleneckEvent();
    }

    return !queue_is_full;
}

/**
 * [THREAD-SAFE] Dequeue message object
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @return Dequeued message object wrapped in an optional
 */
template<size_t SIZE, typename T, class Deleter> std::optional<std::unique_ptr<T>> blogator::MessageQueue<SIZE, T, Deleter>::dequeue() {
    auto front_i     = _front_i.load();
    bool queue_empty = false;

    //as long as there are still available committed nodes in the queue, keep trying to grab one from the 'front'
    while( !( queue_empty = emptied( front_i ) )
        && !_front_i.compare_exchange_weak( front_i, MessageQueue::normalize( front_i, 1 ) ) );

    if( queue_empty ) {
        return {}; //EARLY RETURN
    }

    resetBottleneckState();

    auto & node  = _nodes->at( front_i );
    T    * r_ptr = node.data.exchange( nullptr );

    return std::unique_ptr<T>( r_ptr );
}

/**
 * [THREAD-SAFE] Gets the actual size of the queue
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @return Base 2 size of the queue
 */
template<size_t SIZE, typename T, class Deleter> consteval size_t blogator::MessageQueue<SIZE, T, Deleter>::size() const {
    return SIZE;
}

/**
 * [THREAD-SAFE] Gets empty state of the queue
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @return Empty state of queue
 */
template<size_t SIZE, typename T, class Deleter> bool blogator::MessageQueue<SIZE, T, Deleter>::empty() const {
    const auto front_i = _front_i.load();
    const auto back_i  = _committed_back_i.load();
    return ( front_i == back_i && !_nodes->at( front_i ).hasData() );
}

/**
 * [PRIVATE] Check if a given index is at the end of the available buffer space
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param index Index
 * @return Reached front of the queue in a filled buffer
 */
template<size_t SIZE, typename T, class Deleter> bool blogator::MessageQueue<SIZE, T, Deleter>::reachedEnd( size_t index ) const {
    return ( index == _front_i.load() && _nodes->at( index ).hasData() );
}

/**
 * [PRIVATE] Check if queue is empty from the perspective of the given index
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param front_i Current front index
 * @return No more committed data to consume
 */
template<size_t SIZE, typename T, class Deleter> bool blogator::MessageQueue<SIZE, T, Deleter>::emptied( size_t front_i ) const {
    const auto committed_back_i = _committed_back_i.load();

    if( front_i == committed_back_i ) {
        return !_nodes->at( committed_back_i ).hasData()
            || committed_back_i != _pending_back_i.load();
    }

    return false;
}

/**
 * [PRIVATE] Record a bottleneck event
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 */
template<size_t SIZE, typename T, class Deleter> void blogator::MessageQueue<SIZE, T, Deleter>::recordBottleneckEvent() {
    ++_bottleneck_count;
}

/**
 * [PRIVATE] Reset the bottleneck state
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 */
template<size_t SIZE, typename T, class Deleter> void blogator::MessageQueue<SIZE, T, Deleter>::resetBottleneckState() {
    size_t curr_bottleneck = _bottleneck_count.load();
    size_t max_bottleneck  = _bottleneck_max_count.load();

    if( curr_bottleneck > max_bottleneck ) {
        _bottleneck_max_count.compare_exchange_strong( max_bottleneck, curr_bottleneck );
        _bottleneck_count.compare_exchange_strong( curr_bottleneck, 0 );
    }
}

/**
 * [PRIVATE] Normalises an index
 * @tparam SIZE Queue size
 * @tparam T Message object type
 * @tparam Deleter Message object type deleter to use (optional)
 * @param index Index
 * @param offset Offset (+/-1 or 0)
 * @return Normalised index with offset
 */
template<size_t SIZE, typename T, class Deleter> size_t blogator::MessageQueue<SIZE, T, Deleter>::normalize( size_t index, int offset ) {
    constexpr size_t UPPER_BOUND { SIZE - 1 };

    if( index == 0 && offset < 0 ) {
        return UPPER_BOUND;

    } else if( index == UPPER_BOUND && offset > 0 ) {
        return 0;

    } else {
        return ( ( index + offset ) % SIZE );
    }
}

#endif //BLOGATOR_MESSAGEQUEUE_H