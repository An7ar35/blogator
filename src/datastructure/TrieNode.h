#ifndef BLOGATOR_DATASTRUCTURE_TRIENODE_H
#define BLOGATOR_DATASTRUCTURE_TRIENODE_H

#include <type_traits>
#include <limits>
#include <vector>

namespace blogator {
    template<typename T> concept TrieType = ( std::numeric_limits<T>::is_integer &&
                                              std::negation<std::bool_constant<std::is_same<T, bool>::value>>::value );

    template<TrieType T> class TrieNode {
      public:
        typedef typename std::map<T, TrieNode<T>>::iterator ChildIterator_t;

        explicit TrieNode( T e );
        TrieNode( T e, bool end_flag );

        bool operator ==( const TrieNode<T> &rhs ) const;
        bool operator !=( const TrieNode<T> &rhs ) const;
        bool operator <( const TrieNode<T> &rhs ) const;
        bool operator >( const TrieNode<T> &rhs ) const;
        bool operator <=( const TrieNode<T> &rhs ) const;
        bool operator >=( const TrieNode<T> &rhs ) const;

        std::pair<ChildIterator_t, bool> addChild( T e, bool end = false );
        void incrementUsage();
        void decrementUsage();
        void setEnd( bool val = true );

        [[nodiscard]] bool hasChildren() const;
        std::pair<ChildIterator_t, bool> next( T e );
        T element() const;
        [[nodiscard]] size_t usage() const;
        [[nodiscard]] bool end() const;

      private:
        T                        _element;
        std::map<T, TrieNode<T>> _children;
        size_t                   _usage_count;
        bool                     _end;
    };

    //================================== IMPLEMENTATION ============================================

    /**
     * Constructor
     * @tparam T Element type
     * @param e Element
     */
    template<TrieType T> TrieNode<T>::TrieNode( T e ) :
        _element( e ),
        _usage_count( 0 ),
        _end( false )
    {}

    /**
     * Constructor
     * @tparam T Element type
     * @param e Element
     * @param end_flag End flag value
     */
    template<TrieType T> TrieNode<T>::TrieNode( T e, bool end_flag ):
        _element( e ),
        _usage_count( 0 ),
        _end( end_flag )
    {}

    /**
     * Equal operator
     * @tparam Element type
     * @param rhs TrieNode to compare to
     * @return Equal state
     */
    template<TrieType T> bool TrieNode<T>::operator ==( const TrieNode<T> &rhs ) const {
        return ( _element == rhs._element );
    }

    /**
     * Not equal operator
     * @tparam Element type
     * @param rhs TrieNode to compare to
     * @return Not equal state
     */
    template<TrieType T> bool TrieNode<T>::operator !=( const TrieNode<T> &rhs ) const {
        return ( _element != rhs._element );
    }

    /**
     * Smaller-than operator
     * @tparam Element type
     * @param rhs TrieNode to compare to
     * @return Smaller-than state
     */
    template<TrieType T> bool TrieNode<T>::operator <( const TrieNode<T> &rhs ) const {
        return ( _element < rhs._element );
    }

    /**
     * Greater-than operator
     * @tparam Element type
     * @param rhs TrieNode to compare to
     * @return Greater-than state
     */
    template<TrieType T> bool TrieNode<T>::operator >( const TrieNode<T> &rhs ) const {
        return ( _element > rhs._element );
    }

    /**
     * Smaller-equal operator
     * @tparam Element type
     * @param rhs TrieNode to compare to
     * @return Smaller-equal state
     */
    template<TrieType T> bool TrieNode<T>::operator <=( const TrieNode<T> &rhs ) const {
        return ( _element <= rhs._element );
    }

    /**
     * Greater-equal operator
     * @tparam Element type
     * @param rhs TrieNode to compare to
     * @return Greater-equal state
     */
    template<TrieType T> bool TrieNode<T>::operator >=( const TrieNode<T> &rhs ) const {
        return ( _element >= rhs._element );
    }

    /**
     * Adds a child node and increments the node's usage count by 1
     * @tparam T Element type
     * @param e Element of the child node
     * @param end End flag to set (default=false)
     * @return { iterator, insertion_flag }
     */
    template<TrieType T> std::pair<typename TrieNode<T>::ChildIterator_t, bool> TrieNode<T>::addChild( T e, bool end ) {
        auto pair = _children.try_emplace( e, TrieNode<T>{ e, end } );

        if( pair.first->second.end() != end ) {
            pair.first->second.setEnd( end );
        }

        ++_usage_count;
        return pair;
    }

    /**
     * Increments usage count by 1
     * @tparam T Element type
     */
    template<TrieType T> void TrieNode<T>::incrementUsage() {
        ++_usage_count;
    }

    /**
     * Decrement usage count by 1
     * @tparam T Element type
     */
    template<TrieType T> void TrieNode<T>::decrementUsage() {
        if( _usage_count > 0 )
            --_usage_count;
    }

    /**
     * Sets the end flag
     * @tparam T Element type
     * @param val Flag value (default=true)
     */
    template<TrieType T> void TrieNode<T>::setEnd( bool val ) {
        _end = val;
    }

    /**
     * Checks if node has children
     * @tparam T Element type
     * @return Has children state
     */
    template<TrieType T> bool TrieNode<T>::hasChildren() const {
        return !( _children.empty() );
    }

    /**
     * Gets the child node matching a given element
     * @tparam T Element type
     * @param e Element to find
     * @return { iterator, found_flag }
     */
    template<TrieType T> std::pair<typename TrieNode<T>::ChildIterator_t, bool> TrieNode<T>::next( T e ) {
        auto it = _children.find( e );
        return { it, ( it != _children.cend() ) };
    }

    /**
     * Gets the node's element
     * @tparam T Element type
     * @return Element
     */
    template<TrieType T> T TrieNode<T>::element() const {
        return _element;
    }

    /**
     * Gets the use count of the node
     * @tparam T Element type
     * @return Use counter
     */
    template<TrieType T> size_t TrieNode<T>::usage() const {
        return _usage_count;
    }

    /**
     * Gets the end flag of the node
     * @tparam T Element type
     * @return End flag state
     */
    template<TrieType T> bool TrieNode<T>::end() const {
        return _end;
    }
}

#endif //BLOGATOR_DATASTRUCTURE_TRIENODE_H
