#ifndef BLOGATOR_DTO_SEEKRANGE_H
#define BLOGATOR_DTO_SEEKRANGE_H

#include "InsertPosition.h"

namespace blogator::dto {
    struct SeekRange { //Hold the seek range for a section of a file's content
        SeekRange( const InsertPosition &from, const InsertPosition &to ) :
            _from( from ), _to( to )
        {};

        friend std::ostream &operator <<( std::ostream &s, const SeekRange &seek_range ) {
            s << "(" << seek_range._from << " -> " << seek_range._to << ")";
            return s;
        }

        bool operator==( const SeekRange &rhs ) const {
            return _from == rhs._from && _to == rhs._to;
        }

        bool operator<( const SeekRange &rhs ) const {
            return _from == rhs._from ? _to < rhs._to : _from < rhs._from;
        }

        InsertPosition _from;
        InsertPosition _to;
    };
}

#endif //BLOGATOR_DTO_SEEKRANGE_H