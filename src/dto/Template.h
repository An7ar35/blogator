#ifndef BLOGATOR_DTO_TEMPLATE_H
#define BLOGATOR_DTO_TEMPLATE_H

#include <memory>
#include <filesystem>
#include <map>

#include "HTML.h"
#include "InsertPosition.h"

namespace blogator::dto {
    struct Template {
        enum class Type {
            LANDING, POST, INDEX, INDEX_ENTRY, INDEX_LIST
        };

        explicit Template( Type template_type );

        typedef std::map<std::string, bool> BlockInsertClasses_t;

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