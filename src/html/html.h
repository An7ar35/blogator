#ifndef BLOGATOR_READER_H
#define BLOGATOR_READER_H

#include "reader/reader.h"
#include "writer/writer.h"

namespace blogator::html {
    std::string createStylesheetLink( const std::filesystem::path &css_loc, const std::filesystem::path &out_dir );
}

#endif //BLOGATOR_READER_H
