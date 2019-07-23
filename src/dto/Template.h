#ifndef BLOGATOR_DTO_TEMPLATE_H
#define BLOGATOR_DTO_TEMPLATE_H

#include <memory>
#include <filesystem>
#include <map>

#include "HTML.h"

namespace blogator::dto {
    struct Template {
        enum class Type {
            LANDING, LANDING_ENTRY, POST, INDEX, INDEX_ENTRY, INDEX_LIST
        };

        struct InsertPosition {
            InsertPosition( HTML::LineIndex_t l, HTML::CharIndex_t c ) :
                line( l ), col( c )
            {};

            HTML::LineIndex_t line;
            HTML::CharIndex_t col;

            friend std::ostream &operator <<( std::ostream &s, const InsertPosition &insert_pos ) {
                s << insert_pos.line << ":" << insert_pos.col;
                return s;
            }

            bool operator==( const InsertPosition &rhs ) const {
                return line == rhs.line  && col == rhs.col;
            }

            bool operator<( const InsertPosition &rhs ) const {
                return line == rhs.line ? col < rhs.col : line < rhs.line;
            }
        };

        explicit Template( Type template_type );

        typedef std::map<std::string, bool>                     BlockInsertClasses_t;
        typedef std::map<InsertPosition, std::string>           ConsecutiveWritePositions_t;
        typedef std::map<InsertPosition, std::filesystem::path> ConsecutivePathPositions_t;

        struct WritePosIterators {
            ConsecutiveWritePositions_t::const_iterator block;
            ConsecutivePathPositions_t::const_iterator  path;
        };

        Type                        type;
        std::filesystem::path       src;
        std::unique_ptr<HTML>       html;
        ConsecutivePathPositions_t  path_write_pos;
        ConsecutiveWritePositions_t block_write_pos;
        BlockInsertClasses_t        block_classes;
    };
}

#endif //BLOGATOR_TEMPLATE_H