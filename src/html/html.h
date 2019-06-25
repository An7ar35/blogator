#ifndef BLOGATOR_HTML_H
#define BLOGATOR_HTML_H

#include "reader/reader.h"
#include "writer/writer.h"

namespace blogator::html {
    std::string createStylesheetLink( const std::filesystem::path &css_loc, const std::filesystem::path &out_dir );
    std::string createHyperlink( const std::filesystem::path &href, const std::string &link_text, const std::string &css_class = "" );
    std::string createDateTime( const dto::DateStamp &date_stamp, const dto::Template::MonthStringLookup_t &months, const std::string &css_class = "" );
}

#endif //BLOGATOR_HTML_H
