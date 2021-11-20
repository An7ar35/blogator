#ifndef BLOGATOR_TESTS_CMAKE_H
#define BLOGATOR_TESTS_CMAKE_H

#include <string>

namespace blogator::tests {
    inline static const std::string HTML5LIB_TOKENIZER_TEST_PATH = "@SUBMODULE_PATH@/html5lib-tests/tokenizer";
    inline static const std::string W3_TEST_FILE_DIRECTORY_PATH  = "@CMAKE_RESOURCE_PATH@/tests/W3";
}

#endif //BLOGATOR_TESTS_CMAKE_H
