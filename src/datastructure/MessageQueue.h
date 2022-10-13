#ifndef BLOGATOR_DATASTRUCTURE_MESSAGEQUEUE_H
#define BLOGATOR_DATASTRUCTURE_MESSAGEQUEUE_H

#include <memory>
#include <optional>
#include <condition_variable>
#include <functional>

namespace blogator {
    /**
     * ADT Message Queue
     * @tparam T Message object type
     */
    template<typename T> class MessageQueue {
      public:
        MessageQueue();
        explicit MessageQueue( std::function<void( void )> signaling_fn );
        MessageQueue( const MessageQueue & ) = delete;
        ~MessageQueue();

        MessageQueue & operator =( const MessageQueue & ) = delete;

        void enqueue( std::unique_ptr<T> data_ptr );
        std::optional<std::unique_ptr<T>> dequeue();

        [[nodiscard]] bool empty() const;

      private:
        struct Node {
            Node() :
                next( nullptr )
            {};

            explicit Node( std::unique_ptr<T> &&data_ptr ) :
                data( std::move( data_ptr ) ),
                next( nullptr )
            {}

            std::unique_ptr<T>  data;
            std::atomic<Node *> next;
        };

        // enqueue => []<-....<-[]<-[] <= dequeue
        //            |       |      |
        //            _back   .next  _front

        std::atomic<Node *>         _back;
        std::atomic<Node *>         _front;
        std::function<void( void )> _signal_fn;

        Node * createNode( std::unique_ptr<T> && data_ptr );
        void deleteNode( Node * node );
    };
}

/**
 * Constructor
 * @tparam T Message object type
 */
template<typename T> blogator::MessageQueue<T>::MessageQueue() :
    _back( nullptr ),
    _front( nullptr )
{}

/**
 * Constructor
 * @tparam T Message object type
 * @param signaling_fn Function to use for signaling content in queue
 */
template<typename T> blogator::MessageQueue<T>::MessageQueue( std::function<void( void )> signaling_fn ) :
    _back( nullptr ),
    _front( nullptr ),
    _signal_fn( std::move( signaling_fn ) )
{}

/**
 * Destructor
 * @tparam T Message object type
 */
template<typename T> blogator::MessageQueue<T>::~MessageQueue() {
    if( !empty() ) {
        auto dequeued = this->dequeue();

        while( dequeued.has_value() ) {
            dequeued = this->dequeue();
        }
    }
}

/**
 * [THREAD-SAFE] Enqueue message object
 * @tparam T Message object type
 * @param data_ptr Message object pointer
 */
template<typename T> void blogator::MessageQueue<T>::enqueue( std::unique_ptr<T> data_ptr ) {
    std::atomic<Node *> new_node  = createNode( std::move( data_ptr ) );
    Node *              null_node = nullptr;

    if( _back.compare_exchange_strong( null_node, new_node ) ) {
        _front.compare_exchange_strong( ( null_node = nullptr ), new_node ); //stops dequeue-ing until front is set

    } else { //queue has element(s)
        Node * old_back = _back.load();

        while( !old_back->next.compare_exchange_weak( ( null_node = nullptr ), new_node ) ) {
            old_back = _back.load();
        }

        _back.compare_exchange_strong( old_back, new_node );
    }

    if( _signal_fn ) {
        _signal_fn();
    }
}

/**
 * [THREAD-SAFE] Dequeue message object
 * @tparam T Message object type
 * @return Dequeued message object wrapped in an optional
 */
template<typename T> std::optional<std::unique_ptr<T>> blogator::MessageQueue<T>::dequeue() {
    Node * old_front = _front.load();

    while( old_front && !_front.compare_exchange_weak( old_front, old_front->next ) ) { //disconnect target node
        old_front = _front.load();
    }

    if( old_front ) {
        Node * null_node     = nullptr;
        Node * old_front_tmp = old_front;
        _back.compare_exchange_strong( old_front_tmp, old_front->next );
        _front.compare_exchange_strong( null_node, old_front->next ); //edge-case: last node in queue

        auto data = std::move( old_front->data );
        deleteNode( old_front );
        return std::move( data );

    } else {
        return {};
    }
}

/**
 * [THREAD-SAFE] Gets empty state of the queue
 * @tparam T Message object type
 * @return Empty state of queue
 */
template<typename T> bool blogator::MessageQueue<T>::empty() const {
    return ( _front.load() == nullptr );
}

/**
 * [PRIVATE] Creates a node
 * @tparam T Message object type
 * @param data_ptr Data pointer to pack inside node
 * @return Pointer to packed node
 */
template<typename T> typename blogator::MessageQueue<T>::Node * blogator::MessageQueue<T>::createNode( std::unique_ptr<T> &&data_ptr ) {
    return new Node( std::move( data_ptr ) );
}

/**
 * [PRIVATE] Frees up a Node
 * @tparam T Message object type
 * @param node Pointer to node
 */
template<typename T> void blogator::MessageQueue<T>::deleteNode( Node * node ) {
    delete node;
}

#endif //BLOGATOR_MESSAGEQUEUE_H