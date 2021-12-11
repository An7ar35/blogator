#ifndef BLOGATOR_LOGGER_ENGINE_LOGQUEUE_H
#define BLOGATOR_LOGGER_ENGINE_LOGQUEUE_H

#include <mutex>
#include <deque>
#include <memory>
#include <functional>

namespace blogator::logger::engine {
    template<typename T> class LogQueue {
      public:
        LogQueue() = default;
        ~LogQueue() = default;
        LogQueue( const LogQueue & ) = delete;

        LogQueue & operator =( const LogQueue & ) = delete;

        void setReadyCallback( const std::function<void()> &cb );

        void enqueue( T data );
        void enqueue( std::unique_ptr<T> data );
        [[nodiscard]] std::unique_ptr<T> dequeue();
        [[nodiscard]] size_t size();
        [[nodiscard]] bool empty();

      private:
        std::function<void()>          _signal_readiness_cb {};
        std::mutex                     _mutex;
        std::deque<std::unique_ptr<T>> _queue;
    };
}

/**
 * Set ready-state signalling callback
 * @tparam T Typename for queue's data
 * @param cb Callback for signaling ready-state (i.e.: has content)
 */
template<typename T> void blogator::logger::engine::LogQueue<T>::setReadyCallback( const std::function<void()> &cb ) {
    std::lock_guard<std::mutex> guard( _mutex );
    _signal_readiness_cb = cb;
}

/**
 * Enqueues data
 * @tparam T Typename for queue's data
 * @param data Data to enqueue
 */
template<typename T> void blogator::logger::engine::LogQueue<T>::enqueue( T data ) {
    {
        std::lock_guard<std::mutex> guard( _mutex );
        _queue.template emplace_back( std::make_unique<T>( std::move( data ) ) );
    }

    if( _signal_readiness_cb ) {
        _signal_readiness_cb();
    }
}

/**
 * Enqueues data
 * @tparam T Typename for queue's data
 * @param data Unique pointer to data to enqueue
 */
template<typename T> void blogator::logger::engine::LogQueue<T>::enqueue( std::unique_ptr<T> data ) {
    {
        std::lock_guard<std::mutex> guard( _mutex );
        _queue.template emplace_back( std::move( data ) );
    }

    if( _signal_readiness_cb ) {
        _signal_readiness_cb();
    }
}

/**
 * Dequeues a message
 * @tparam T Typename for queue's data
 * @return Pointer to dequeued data
 */
template<typename T> std::unique_ptr<T> blogator::logger::engine::LogQueue<T>::dequeue() {
    std::lock_guard<std::mutex> guard( _mutex );

    if( !_queue.empty() ) {
        auto data = std::move( _queue.front() );
        _queue.pop_front();
        return std::move( data );

    } else {
        return std::unique_ptr<T>();
    }
}

/**
 * Gets the number of data packets in queue
 * @tparam T Typename for queue's data
 * @return Queue size
 */
template<typename T> size_t blogator::logger::engine::LogQueue<T>::size() {
    std::lock_guard<std::mutex> guard( _mutex );
    return _queue.size();
}

/**
 * Gets the empty state of the queue
 * @tparam T Typename for queue's data
 * @return Empty state
 */
template<typename T> bool blogator::logger::engine::LogQueue<T>::empty() {
    std::lock_guard<std::mutex> guard( _mutex );
    return false;
}

#endif //BLOGATOR_LOGGER_ENGINE_LOGQUEUE_H