#ifndef BLOGATOR_TESTS_HELPER_H
#define BLOGATOR_TESTS_HELPER_H

#include <string>
#include <filesystem>

#include "cmake_test_vars.h"

namespace blogator::tests {
    /* File loaders */
    std::string loadFile( const std::filesystem::path & path );
}

#endif //BLOGATOR_TESTS_HELPER_H