#ifndef BLOGATOR_TESTS_ASYNCNOTIFY_H
#define BLOGATOR_TESTS_ASYNCNOTIFY_H

#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>

namespace blogator::tests {
    /**
     * Helper to wait for an event happening asynchronously to the calling thread whilst blocking
     * @tparam T Type of Object to check
     */
    template<typename T> class AsyncNotify {
      public:
        typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint_t;
        typedef std::chrono::duration<uint64_t, std::milli>        Duration_ms_t;

        AsyncNotify();
        ~AsyncNotify() = default;

        void setCheckFn( std::function<bool(const T &)> fn );

        void notify();
        void notify( const T & obj );
        bool waitForNotification( uint64_t timeout_ms );
        bool waitForCheckSuccess( uint64_t timeout_ms );
        [[nodiscard]] bool result() const;

        void reset( std::function<bool(const T &)> fn = nullptr );

      private:
        static bool timedOut( TimePoint_t start, Duration_ms_t timeout );

        std::mutex                     _mutex;
        std::atomic<bool>              _notified;
        std::atomic<bool>              _check_result;
        std::function<bool(const T &)> _check_fn;
    };

    /**
     * Constructor
     * @tparam T Type of Object to check
     */
    template<typename T> AsyncNotify<T>::AsyncNotify() :
        _notified( false ),
        _check_result( false ),
        _check_fn( nullptr )
    {}

    /**
     * Sets the checking function
     * @tparam T Type of Object to check
     * @param fn Function to use for check on notify
     */
    template<typename T> void AsyncNotify<T>::setCheckFn( std::function<bool( const T & )> fn ) {
        _check_fn = fn;
    }

    /**
     * Callback for notifying of an expected event
     * @tparam T Type of object to check
     */
    template<typename T> void AsyncNotify<T>::notify() {
        std::lock_guard<std::mutex> guard( _mutex );
        _notified     = true;
    }

    /**
     * Callback for notifying of an expected event
     * @tparam T Type of Object to check
     * @param obj Object to pass to checking function
     */
    template<typename T> void AsyncNotify<T>::notify( const T &obj ) {
        std::lock_guard<std::mutex> guard( _mutex );
        _notified = true;

        if( _check_fn ) {
            _check_result = _check_fn( obj );
        }
    }

    /**
     * Wait for notification (blocking)
     * @tparam T Type of Object to check
     * @param timeout_ms Timeout of the wait
     * @return Notification state
     */
    template<typename T> bool AsyncNotify<T>::waitForNotification( uint64_t timeout_ms ) {
        auto start = std::chrono::steady_clock::now();
        auto timeout = Duration_ms_t( timeout_ms );
        while( !timedOut( start, timeout ) ) {
            {
                std::lock_guard<std::mutex> guard( _mutex );
                if( _notified ) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Wait for notification and a successful check result
     * @tparam T Type of Object to check
     * @param timeout_ms Timeout of the wait
     * @return Successful check state
     */
    template<typename T> bool AsyncNotify<T>::waitForCheckSuccess( uint64_t timeout_ms ) {
        auto start = std::chrono::steady_clock::now();
        auto timeout = Duration_ms_t( timeout_ms );
        while( !timedOut( start, timeout ) ) {
            {
                std::lock_guard<std::mutex> guard( _mutex );
                if( _notified && _check_result ) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Gets the results
     * @tparam T Type of Object to check
     * @return Checking result
     */
    template<typename T> bool AsyncNotify<T>::result() const {
        return _check_result;
    }

    /**
     * Resets the state
     * @tparam T Type of Object to check
     * @param fn Checking function to set (optional)
     */
    template<typename T> void AsyncNotify<T>::reset( std::function<bool(const T &)> fn ) {
        std::lock_guard<std::mutex> guard( _mutex );
        _check_fn     = fn;
        _notified     = false;
        _check_result = false;
    }

    /**
     * [PRIVATE] Checks a timeout has occurred
     * @tparam T Type of Object to check
     * @param start Start time point
     * @param timeout Timeout duration value
     * @return Timeout state
     */
    template<typename T> bool AsyncNotify<T>::timedOut( AsyncNotify::TimePoint_t start, AsyncNotify::Duration_ms_t timeout ) {
        return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - start ).count() > timeout.count();
    }
}

#endif //BLOGATOR_TESTS_ASYNCNOTIFY_H