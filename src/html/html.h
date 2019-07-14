#ifndef BLOGATOR_HTML_H
#define BLOGATOR_HTML_H

#include <list>

#include "../dto/Options.h"
#include "reader/reader.h"
#include "writer/writer.h"

namespace blogator::html {
    typedef std::list<std::pair<std::string, std::filesystem::path>> BreadCrumb_t;

    std::string createStylesheetLink( const std::filesystem::path &css_loc, const std::filesystem::path &out_dir );
    std::string createHyperlink( const std::filesystem::path &href, const std::string &link_text, const std::string &css_class = "" );
    std::string createHyperlink( const std::filesystem::path &href, const std::string &link_text, const std::string &css_class, const std::string &rel );
    std::string createDateTime( const dto::DateStamp &date_stamp, const dto::Options::MonthStringLookup_t &months, const std::string &css_class = "" );
    dto::HTML   createBreadcrumb( const BreadCrumb_t &breadcrumb );
    std::unique_ptr<dto::IndexDateTree> generateIndexDateTreeHTML( const dto::Index &master_index, const dto::Options &options );
    std::unique_ptr<dto::IndexTagTree> generateIndexTagTreeHTML(  const dto::Index &master_index, const dto::Options &options );
}

#endif //BLOGATOR_HTML_H