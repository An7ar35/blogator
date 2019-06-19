#ifndef BLOGATOR_DTO_INDEX_H
#define BLOGATOR_DTO_INDEX_H

#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include "Article.h"

namespace blogator::dto {
    struct Index {
        friend std::ostream &operator <<( std::ostream &s, const Index &index ) {
            s << "Header....: " << index._paths.header.string() << std::endl;
            s << "Footer....: " << index._paths.footer.string() << std::endl;
            s << "Blog CSS..: " << index._paths.blog_css.string() << std::endl;
            s << "Index CSS.: " << index._paths.index_css.string() << std::endl;
            s << "Articles..: " << index._file_index.size() << " found." << std::endl;
            s << "Tags......: " << std::endl;
            for( const auto &t : index._global_tags )
                s << "\t" << t.first << std::endl;
            s << std::endl;
            for( const auto &a : index._file_index )
                s << a << std::endl;
            return s;
        }

        struct Paths {
            std::filesystem::path header;
            std::filesystem::path footer;
            std::filesystem::path index_css;
            std::filesystem::path blog_css;
        } _paths;

        std::vector<Article>                         _file_index;
        std::map<std::string, std::filesystem::path> _global_tags;
    };
}

#endif //BLOGATOR_DTO_INDEX_H