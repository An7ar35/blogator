#ifndef BLOGATOR_DATASTRUCTURE_OBJECTPOOL_H
#define BLOGATOR_DATASTRUCTURE_OBJECTPOOL_H

#include <memory>
#include <stack>
#include <set>

#include "../logger/Logger.h"
#include "../exception/resource_unavailable.h"

namespace blogator {
    template<typename T> class ObjectPoolDeleter;

    /**
     * ADT Object Pool
     * @tparam T Object type
     */
    template<typename T> class ObjectPool {
      public:
        typedef ObjectPoolDeleter<T>          Deleter_t;
        typedef std::unique_ptr<T, Deleter_t> ObjectPtr_t;

        friend Deleter_t;

      public:
        explicit ObjectPool( size_t init_size = 1 );
        ObjectPool( const ObjectPool & ) = delete;
        ObjectPool & operator=( const ObjectPool & ) = delete;
        ~ObjectPool();

        ObjectPtr_t acquire();
        void release( ObjectPtr_t object_ptr );

        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t used() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] size_t available() const;

        static bool active( ObjectPool<T> * pool_ptr );

      private:
        static std::mutex                _global_mutex;
        static std::set<ObjectPool<T> *> _global_pools;

        mutable std::mutex               _mutex;
        const size_t                     _size;
        size_t                           _used;
        std::stack<std::unique_ptr<T>>   _pool;

        void release( std::unique_ptr<T> object_ptr );
    };

    template<typename T> std::mutex ObjectPool<T>::_global_mutex;
    template<typename T> std::set<ObjectPool<T> *> ObjectPool<T>::_global_pools;

    /**
     * ObjectPool Deleter
     * @tparam T Object Type
     */
    template<typename T> class ObjectPoolDeleter {
      public:
        explicit ObjectPoolDeleter( ObjectPool<T> & pool );
        void operator ()( T * ptr );

      private:
        std::reference_wrapper<ObjectPool<T>> _pool;
    };

    template<typename U> std::ostream & operator <<( std::ostream &os, const ObjectPool<U> &pool );
}

/**
 * Constructor
 * @tparam T Object Type
 * @param init_size Size of pool (default: 1)
 */
template<typename T> blogator::ObjectPool<T>::ObjectPool( size_t init_size ) :
    _size( init_size ),
    _used( 0 )
{
    for( size_t i = 0; i < init_size; ++i ) {
        _pool.emplace( std::make_unique<T>() );
    }

    std::lock_guard<std::mutex> guard( _global_mutex );
    _global_pools.insert( this );
}

/**
 * Destructor
 * @tparam T Object Type
 */
template<typename T> blogator::ObjectPool<T>::~ObjectPool() {
    std::lock_guard<std::mutex> guard( _global_mutex );
    _global_pools.erase( this );
}

/**
 * Gets an object from the pool
 * @return Pointer to pool object
 * @throws blogator::exception::resource_unavailable when pool is empty
 */
template<class T> typename blogator::ObjectPool<T>::ObjectPtr_t blogator::ObjectPool<T>::acquire() {
    std::lock_guard<std::mutex> guard( _mutex );

    if(_pool.empty()) {
        throw blogator::exception::resource_unavailable(
            "[ObjectPool<T>::acquire()] ObjectPool is currently empty."
        );
    }

    ObjectPtr_t tmp(
        _pool.top().release(),
        ObjectPoolDeleter{ *this }
    );

    _pool.pop();
    _used++;
    return std::move( tmp );
}

/**
 * [PRIVATE] Returns an object to the pool
 * @param object_ptr Pointer to object
 */
template<class T> void blogator::ObjectPool<T>::release( std::unique_ptr<T> object_ptr ) {
    std::lock_guard<std::mutex> guard( _mutex );
    _pool.push( std::move( object_ptr ) );
    _used--;
}

/**
 * Returns an object to the pool
 * @param object_ptr Pointer to object
 */
template<class T> void blogator::ObjectPool<T>::release( ObjectPtr_t object_ptr ) {
    std::lock_guard<std::mutex> guard( _mutex );
    _pool.emplace( std::move( object_ptr ) );
    _used--;
}

/**
 * Gets the empty state of pool
 * @return Empty state
 */
template<class T> bool blogator::ObjectPool<T>::empty() const {
    std::lock_guard<std::mutex> guard( _mutex );
    return _pool.empty();
}

/**
 * Gets the object count being used
 * @return Used object count
 */
template<class T> size_t blogator::ObjectPool<T>::used() const {
    std::lock_guard<std::mutex> guard( _mutex );
    return _used;
}

/**
 * Gets the capacity of the pool
 * @return Upper bound count of objects
 */
template<class T> size_t blogator::ObjectPool<T>::size() const {
    std::lock_guard<std::mutex> guard( _mutex );
    return _size;
}

/**
 * Gets the current count of pool objects
 * @return Count of objects in pool
 */
template<class T> size_t blogator::ObjectPool<T>::available() const {
    std::lock_guard<std::mutex> guard( _mutex );
    return _pool.size();
}

/**
 * Checks ObjectPool is still active
 * @tparam T ObjectPool Type
 * @param pool_ptr Pointer to ObjectPool (only pointer value will be used)
 * @return Active state
 */
template<typename T> bool blogator::ObjectPool<T>::active( blogator::ObjectPool<T> * pool_ptr ) {
    std::lock_guard<std::mutex> guard( ObjectPool<T>::_global_mutex );
    return ObjectPool<T>::_global_pools.contains( pool_ptr );
}

/**
 * Constructor
 * @tparam T ObjectPool Type
 * @param pool Reference to the source ObjectPool
 */
template<typename T> blogator::ObjectPoolDeleter<T>::ObjectPoolDeleter( blogator::ObjectPool<T> & pool ) :
    _pool( pool )
{}

/**
 * Operation operator (sends back the object to its pool)
 * @tparam T ObjectPool Type
 * @param ptr Object pointer
 */
template<typename T> void blogator::ObjectPoolDeleter<T>::ObjectPoolDeleter::operator ()( T * ptr ) {
    std::lock_guard<std::mutex> guard( ObjectPool<T>::_global_mutex );

    if( ObjectPool<T>::_global_pools.contains( &_pool.get() ) ) {
        _pool.get().release( std::unique_ptr<T> { ptr } );
    } else {
        LOG_WARNING( "[ObjectPoolDeleter<T>::operator()( T * )] Source ObjectPool<T> is gone - using default deleter." );
        std::default_delete<T>{}( ptr );
    }
}

/**
 * Output stream operator
 * @param out Output stream
 * @param pool ObjectPool
 * @return Output stream
 */
template<typename T> std::ostream & blogator::operator <<( std::ostream &os, const blogator::ObjectPool<T> &pool ) {
    os << "ObjectPool<T>: " << pool.available() << "/" << pool.size() << " free";
    return os;
}

#endif //BLOGATOR_DATASTRUCTURE_OBJECTPOOL_H