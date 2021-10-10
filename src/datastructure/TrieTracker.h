#ifndef BLOGATOR_DATASTRUCTURE_TRIETRACKER_H
#define BLOGATOR_DATASTRUCTURE_TRIETRACKER_H

#include "TrieNode.h"

namespace blogator {
    template<TrieType T> class Trie;

    /**
     * Tracker object for tracking matches made incrementally in the Trie data-structure
     * @tparam T Element type
     */
    template<TrieType T> class TrieTracker {
        friend class Trie<T>;
      public:
        TrieTracker();

        const std::vector<T> & lastMatch() const;
        std::vector<T> lastCompleteMatch() const;
        std::vector<T> remainder() const;

        const T & lastMatchedElement() const;
        const T & lastMatchedEndElement() const;

        [[nodiscard]] size_t lastCompleteMatchSize() const;
        [[nodiscard]] bool matched() const;
        [[nodiscard]] bool matching() const;
        [[nodiscard]] bool partial() const;
        [[nodiscard]] bool complete() const;
        [[nodiscard]] bool atEnd() const;
        template<class InputIt> bool match( InputIt first, InputIt last ) const;

      private:
        bool           _matching_state;
        bool           _complete_match;
        std::vector<T> _last_match_buffer;
        size_t         _last_end_index;
        TrieNode<T> *  _last_match_node;
    };

    //================================== IMPLEMENTATION ============================================
    /**
     * Constructor
     * @tparam T Element type
     */
    template<TrieType T> TrieTracker<T>::TrieTracker() :
        _matching_state( false ),
        _complete_match( false ),
        _last_match_buffer( {} ),
        _last_end_index( 0 ),
        _last_match_node( nullptr )
    {}

    /**
     * Gets the complete buffer containing every element contiguously matched with the tracker
     * @tparam T Element type
     * @return Last matched buffer
     */
    template<TrieType T> const std::vector<T> &TrieTracker<T>::lastMatch() const {
        return _last_match_buffer;
    }

    /**
     * Gets the last complete set of contiguously matched elements
     * @tparam T Element type
     * @return Collection of elements up to and including the last 'end' found
     */
    template<TrieType T> std::vector<T> TrieTracker<T>::lastCompleteMatch() const {
        return std::vector<T>( _last_match_buffer.begin(), std::next( _last_match_buffer.begin(), ( _last_end_index + 1 ) ) );
    }

    /**
     * Gets the remainder matched elements that were matched after an 'end'
     * @tparam T Element type
     * @return Collection of remaining elements
     */
    template<TrieType T> std::vector<T> TrieTracker<T>::remainder() const {
        return std::vector<T>( std::next( _last_match_buffer.begin(), ( _last_end_index + 1 ) ), _last_match_buffer.cend() );
    }

    /**
     * Gets the size of the last complete match found
     * @tparam T Element type
     * @return Size of match to an 'end'
     */
    template<TrieType T> size_t TrieTracker<T>::lastCompleteMatchSize() const {
        return ( _last_match_buffer.empty() ? 0 : _last_end_index + 1 );
    }

    /**
     * Gets the last element matched
     * @tparam T Element type
     * @return Last matched element
     * @throws std::out_of_range if element buffer is empty
     */
    template<TrieType T> const T &TrieTracker<T>::lastMatchedElement() const {
        if( !_last_match_buffer.empty() )
            return _last_match_buffer.back();

        throw std::out_of_range( "No elements in match buffer." );
    }

    /**
     * Gets the last 'end' element matched
     * @tparam T Element type
     * @return Last end matched element
     * @throws std::out_of_range if there was no 'end' element encountered
     */
    template<TrieType T> const T &TrieTracker<T>::lastMatchedEndElement() const {
        if( matched() ) {
            return _last_match_buffer.at( _last_end_index );
        }

        throw std::out_of_range( "No 'end' elements in match buffer." );
    }

    /**
     * Checks if there has been a match at some point during the lifetime of the tracker object
     * @tparam T Element type
     * @return Match state (past or present)
     */
    template<TrieType T> bool TrieTracker<T>::matched() const {
        return !_last_match_buffer.empty();
    }

    /**
     * Checks if the tracker is currently matching
     * @tparam T Element type
     * @return Matching state (present)
     */
    template<TrieType T> bool TrieTracker<T>::matching() const {
        return _matching_state;
    }

    /**
     * Checks if the tracker is currently matching partially (i.e. if the last element matched was not an end node)
     * @tparam T Element type
     * @return Partial match state
     */
    template<TrieType T> bool TrieTracker<T>::partial() const {
        return !_last_match_buffer.empty() && !_complete_match;
    }

    /**
     * Checks if the tracker has complete match (non-partial)
     * @tparam T Element type
     * @return Has encountered an 'end' element
     */
    template<TrieType T> bool TrieTracker<T>::complete() const {
        return _complete_match;
    }

    /**
     * Checks if the tracker is currently matching a complete match (non-partial)
     * @tparam T Element type
     * @return Last matching element is an 'end'
     */
    template<TrieType T> bool TrieTracker<T>::atEnd() const {
        return ( _last_match_node != nullptr && _last_match_node->end() );
    }

    /**
     * Compares the last matched in the buffer with a range
     * @tparam T Element type
     * @tparam InputIt Iterator type
     * @param first Beginning of the range
     * @param last End of the range
     * @return Match state of the comparison (false if first==last)
     */
    template<TrieType T> template<class InputIt> bool TrieTracker<T>::match( InputIt first, InputIt last ) const {
        if( matched() && first != last ) {
            return std::equal_to( _last_match_buffer, first, last );
        }

        return false;
    }
}

#endif //BLOGATOR_DATASTRUCTURE_TRIETRACKER_H
