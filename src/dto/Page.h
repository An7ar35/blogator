#ifndef BLOGATOR_PAGE_H
#define BLOGATOR_PAGE_H

#include <fstream>
#include <filesystem>

namespace blogator::dto {
    struct Page {
        std::filesystem::path _abs_path;
        std::ofstream         _out;
    };
}

#endif //BLOGATOR_PAGE_H