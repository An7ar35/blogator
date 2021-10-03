#ifndef BLOGATOR_DATASTRUCTURE_TRIE_H
#define BLOGATOR_DATASTRUCTURE_TRIE_H

#include <map>
#include <deque>

#include "TrieTracker.h"

namespace blogator {
    /**
     * Trie data-structure
     * @tparam T Element type
     */
    template<TrieType T> class Trie {
      public:
        typedef std::vector<std::vector<T>> Collection_t;

        friend std::ostream & operator<<( std::ostream &os, const Trie<T> &trie ) {
            return trie.print( os );
        }

        bool add( T e );
        template<class InputIt> bool add( InputIt first, InputIt last );

        bool match( TrieTracker<T> & tracker, T next_element );
        template<class InputIt> bool has( InputIt first, InputIt last ) const;

        void clear();
        [[nodiscard]] size_t rootCount() const;
        Collection_t collect() const;
        std::ostream & print( std::ostream & os ) const;

      private:
        template<class InputIt> bool has_( InputIt first, InputIt last, const TrieNode<T> * node ) const;
        void collect_( Collection_t &collection, std::deque<T> & stem, const TrieNode<T> &node ) const;
        std::ostream & print_( std::ostream & os, std::deque<T> & stem, const TrieNode<T> &node ) const;

        template<class InputIt> void add_( InputIt first, InputIt last, TrieNode<T> &node );

        std::map<T, size_t>      _grove;
        std::vector<TrieNode<T>> _roots;
    };

    //================================== IMPLEMENTATION ============================================

    /**
     * Adds a single element
     * @tparam T Element type
     * @param e Element
     * @return Success (will rollback on failure)
     */
    template<TrieType T> bool Trie<T>::add( T e ) {
        auto [root_it, added] = _grove.try_emplace( e, 0 );
        auto & root_index     = root_it->second;

        if( !added ) {
            _roots.at( root_index ).setEnd( true );
            _roots.at( root_index ).incrementUsage();
            return true; //EARLY RETURN
        }

        try {
            auto node = _roots.emplace_back( TrieNode<T>( e, true ) );
            root_index = ( _roots.size() - 1 );
            return true;

        } catch( std::exception & e ) {
            _grove.erase( root_it ); //rollback (added == true)
            return false;
        }
    }

    /**
     * Adds a range of elements in order
     * @tparam T Element type
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end + 1 of the range
     * @return Success
     */
    template<TrieType T> template<class InputIt> bool Trie<T>::add( InputIt first, InputIt last ) {
        if( first == last ) {
            return false;
        }

        auto  [root_it, added] = _grove.try_emplace( *first, 0 );
        auto & root_index      = root_it->second;

        if( added ) {
            auto & node = _roots.emplace_back( *first, ( std::next( first, 1 ) == last ) );
            root_index = ( _roots.size() - 1 );
            add_( std::next( first ), last, node );

        } else {
            if( ( std::next( first, 1 ) == last ) )
                _roots.at( root_index ).setEnd( true );

            _roots.at( root_index ).incrementUsage();
            add_( std::next( first ), last, _roots.at( root_index ) );
        }

        return true;
    }

    /**
     * Recursively adds a range of element in order starting at a given node
     * @tparam T Element type
     * @tparam InputIt Iterator type
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end + 1 of the range
     * @param node Node to add children element from
     */
    template<TrieType T> template<class InputIt> void Trie<T>::add_( InputIt first, InputIt last, TrieNode<T> &node ) {
        if( first == last ) {
            node.setEnd( true );
            return; //EARLY RETURN
        }

        auto [it, added] = node.addChild( *first );

        return add_( std::next( first ), last, it->second );
    }

    /**
     * Try to match with an element in an iterative manner using a tracker to keep progress state
     * @tparam T Element type
     * @param tracker Tracker object for keeping state
     * @param next_element Element to try to match next
     * @return Match state
     */
    template<TrieType T> bool Trie<T>::match( TrieTracker<T> &tracker, T next_element ) {
        if( tracker._last_match_node == nullptr ) { //starting at root
            if( _grove.contains( next_element ) ) {
                const auto root_index = _grove.at( next_element );
                auto     & root_node  = _roots.at( root_index );

                tracker._match_state     = true;
                tracker._complete_match  = root_node.end();
                tracker._last_match_node = &root_node;
                tracker._last_match_buffer.emplace_back( next_element );

            }

        } else {
            auto child_it = tracker._last_match_node->_children.find( next_element );

            if( child_it != tracker._last_match_node->_children.cend() ) {
                tracker._complete_match  = child_it->second.end();
                tracker._last_match_node = &child_it->second;
                tracker._last_match_buffer.emplace_back( next_element );

            } else {
                tracker._match_state = false;
            }
        }

        return tracker._match_state;
    }

    /**
     * Try to match a range of element
     * @tparam T Element type
     * @tparam InputIt Iterator type
     * @param first Beginning of range
     * @param last End of range
     * @return Match state
     */
    template<TrieType T> template<class InputIt> bool Trie<T>::has( InputIt first, InputIt last ) const {
        if( first == last )
            return false; //EARLY RETURN

        if( !_grove.contains( *first ) )
            return false; //EARLY RETURN

        const auto   root_index = _grove.at( *first );
        const auto & root_node  = _roots.at( root_index );
        auto         curr_it    = std::next( first );

        return has_( curr_it, last, &root_node );
    }

    /**
     * Clears the trie
     * @tparam T Element type
     */
    template<TrieType T> void Trie<T>::clear() {
        _grove.clear();
        _roots.clear();
    }

    /**
     * Gets the number of roots in the grove
     * @tparam T Element type
     * @return Root count in the grove
     */
    template<TrieType T> size_t Trie<T>::rootCount() const {
        return _roots.size();
    }

    /**
     * [FOR TESTING PURPOSES] Collects all element variations in the Trie
     * @tparam T Element type
     * @return Collection of element variations
     */
    template<TrieType T> typename Trie<T>::Collection_t Trie<T>::collect() const {
        Collection_t collection;

        for( auto root_index : _grove ) {
            auto root_node = _roots.at( root_index.second );
            auto stem      = std::deque<T>();

            collect_( collection, stem, root_node );
        }

        return collection;
    }

    /**
     * Prints out all element variations found in the Trie
     * @tparam T Element type
     * @param os Output stream
     * @return Output stream
     */
    template<TrieType T> std::ostream & Trie<T>::print( std::ostream &os ) const {
        for( auto root_index : _grove ) {
            auto root_node = _roots.at( root_index.second );
            auto stem      = std::deque<T>();

            print_( os, stem, root_node );
        }

        return os;
    }

    /**
     * Checks if range matches elements starting at a node
     * @tparam T Element type
     * @tparam InputIt Iterator type
     * @param first Begin iterator of range
     * @param last End iterator of range
     * @param node Pointer to starting node
     * @return Match state
     */
    template<TrieType T> template<class InputIt> bool Trie<T>::has_( InputIt first, const InputIt last, const TrieNode<T> * node ) const {
        auto child_it = node->_children.find( *first );

        if( child_it != node->_children.cend() ) {
            auto next_it = std::next( first );

            if( next_it == last ) {
                return true;
            } else {
                return has_( next_it, last, &child_it->second );
            }
        }

        return false;
    }

    /**
     * Collects all the element variations from a node
     * @tparam T Element type
     * @param collection Collection
     * @param stem Stem of element(s)
     * @param node Starting node
     */
    template<TrieType T> void Trie<T>::collect_( Trie<T>::Collection_t &collection, std::deque<T> &stem, const TrieNode<T> &node ) const {
        stem.push_back( node.element() );

        if( node.end() || !node.hasChildren() ) {
            collection.emplace_back( std::vector<T>( stem.begin(), stem.end() ) );
        }

        for( const auto & n : node.children() ) {
            collect_( collection, stem, n.second );
        }

        stem.pop_back();
    }

    /**
     * Prints out all the element variations from a node
     * @tparam T Element type
     * @param os Output stream
     * @param stem Stem of element(s)
     * @param node Starting node
     * @return Output stream
     */
    template<TrieType T> std::ostream & Trie<T>::print_( std::ostream &os, std::deque<T> &stem, const TrieNode<T> &node ) const { //TODO
        stem.push_back( node.element() );

        if( node.end() || !node.hasChildren() ) {
            std::for_each( stem.begin(), stem.end(), [&os]( const auto &e ) { os << e << " "; } );
            os << std::endl;
        }

        for( const auto & n : node.children() ) {
            print_( os, stem, n.second );
        }

        stem.pop_back();
        return os;
    }
}

#endif //BLOGATOR_DATASTRUCTURE_TRIE_H
