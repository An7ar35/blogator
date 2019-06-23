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
            s << "Post page template..: " << index._paths.post_template.string() << std::endl;
            s << "Index page template.: " << index._paths.index_template.string() << std::endl;
            s << "Start page template.: " << index._paths.start_template.string() << std::endl;
            s << "Blog stylesheet ....: " << index._paths.blog_css.string() << std::endl;
            s << "Index stylesheet ...: " << index._paths.index_css.string() << std::endl;
            s << "Article count.......: " << index._articles.size() << " found." << std::endl;
            s << "Tags found..........: ";
            for( const auto &t : index._global_tags )
                s << "\"" << t.first << "\" ";
            s << std::endl << std::endl;
            for( const auto &a : index._articles )
                s << a << std::endl;
            return s;
        }

        struct Paths {
            std::filesystem::path index_css;
            std::filesystem::path blog_css;
            std::filesystem::path post_template;
            std::filesystem::path index_template;
            std::filesystem::path start_template;
        } _paths;

        std::vector<Article>                         _articles;
        std::map<std::string, std::filesystem::path> _global_tags; //where in the "index" folder each tag index folder lives in
    };
}

#endif //BLOGATOR_DTO_INDEX_H