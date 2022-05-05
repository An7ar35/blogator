#ifndef BLOGATOR_SOURCES_H
#define BLOGATOR_SOURCES_H

#include <string>

namespace test_harness {
    inline static const std::string HTML5LIB_TOKENIZER_TEST_PATH        = "@SUBMODULE_PATH@/html5lib-tests/tokenizer";
    inline static const std::string HTML5LIB_TREECONSTRUCTION_TEST_PATH = "@SUBMODULE_PATH@/html5lib-tests/tree-construction";
    inline static const std::string COMMONMARK_SPEC_TEST_PATH           = "@SUBMODULE_PATH@/commonmark-spec";
    inline static const std::string MARKDOWN_TOKENIZER_TEST_PATH        = "@CMAKE_TEST_HARNESS_DIRECTORY@/markdown/tests";
}

#endif //BLOGATOR_SOURCES_H
