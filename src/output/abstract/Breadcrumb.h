#ifndef BLOGATOR_OUTPUT_ABSTRACT_BREADCRUMB_H
#define BLOGATOR_OUTPUT_ABSTRACT_BREADCRUMB_H

#include <list>

#include "../../dto/Page.h"

namespace blogator::output::abstract {
    class Breadcrumb {
      public:
        typedef std::list<std::pair<std::string, std::filesystem::path>> BreadCrumb_t;

      protected:
        void writeBreadcrumb( dto::Page   &page,
                              const std::string &indent,
                              const BreadCrumb_t &parents,
                              const std::string &child ) const;
    };
}

#endif //BLOGATOR_OUTPUT_ABSTRACT_BREADCRUMB_H