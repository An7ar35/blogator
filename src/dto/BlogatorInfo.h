#ifndef BLOGATOR_DTO_BLOGATORINFO_H
#define BLOGATOR_DTO_BLOGATORINFO_H

#include <string>

namespace blogator::dto {
    struct BlogatorInfo {
        std::string _name;
        std::string _author;
        std::string _version;
        std::string _url;
    };
}

#endif //BLOGATOR_DTO_BLOGATORINFO_H
