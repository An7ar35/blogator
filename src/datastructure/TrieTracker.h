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
        const T & lastMatchedElement() const;
        [[nodiscard]] bool matched() const ;
        [[nodiscard]] bool matching() const;
        [[nodiscard]] bool partial() const;
        [[nodiscard]] bool complete() const;
        template<class InputIt> bool match( InputIt first, InputIt last ) const;

      private:
        bool           _match_state;
        bool           _complete_match;
        std::vector<T> _last_match_buffer;
        TrieNode<T> *  _last_match_node;
    };

    //================================== IMPLEMENTATION ============================================
    /**
     * Constructor
     * @tparam T Element type
     */
    template<TrieType T> TrieTracker<T>::TrieTracker() :
        _match_state( false ),
        _complete_match( false ),
        _last_match_buffer( {} ),
        _last_match_node( nullptr )
    {}

    /**
     * Gets the buffer containing the last matched of the tracker
     * @tparam T Element type
     * @return Last matched buffer
     */
    template<TrieType T> const std::vector<T> &TrieTracker<T>::lastMatch() const {
        return _last_match_buffer;
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
        return _match_state;
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
     * Checks if the tracker is currently matching a complete match (non-partial)
     * @tparam T Element type
     * @return Complete match state
     */
    template<TrieType T> bool TrieTracker<T>::complete() const {
        return _complete_match;
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
