/**
 * CMAKE template for project version-ing
 * --------------------------------------
 * This file is used to generate a header file with the project's version and details found in the
 * CMakeList.txt file. The version is injected from there into the 'generated/version.h' file made
 * from this template
 */
#ifndef BLOGATOR_CMAKE_VARS_H
#define BLOGATOR_CMAKE_VARS_H

#include <string>

namespace blogator::cmake {
    inline static const std::string AUTHOR        = "@PROJECT_AUTHOR@";
    inline static const std::string URL           = "@PROJECT_HOMEPAGE_URL@";
    static const unsigned           VERSION_MAJOR = @PROJECT_VERSION_MAJOR@;
    static const unsigned           VERSION_MINOR = @PROJECT_VERSION_MINOR@;
    static const unsigned           VERSION_PATCH = @PROJECT_VERSION_PATCH@;
    inline static const std::string VERSION       = "@PROJECT_VERSION@";
}

#endif //BLOGATOR_CMAKE_VARS_H
