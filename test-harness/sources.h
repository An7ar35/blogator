#ifndef BLOGATOR_SOURCES_H
#define BLOGATOR_SOURCES_H

#include <string>

namespace test_harness {
    inline static const std::string BLOGATOR_CONFIGURATION_TOKENIZER_TEST_PATH = "@TEST_FILES_PATH@/blogator/configuration/tokeniser";
    inline static const std::string BLOGATOR_CONFIGURATION_PARSER_TEST_PATH    = "@TEST_FILES_PATH@/blogator/configuration/tokeniser";
    inline static const std::string HTML5LIB_TOKENIZER_TEST_PATH               = "@SUBMODULE_PATH@/html5lib-tests/tokenizer";
    inline static const std::string HTML5LIB_TREECONSTRUCTION_TEST_PATH        = "@SUBMODULE_PATH@/html5lib-tests/tree-construction";
    inline static const std::string COMMONMARK_SPEC_TEST_PATH                  = "@SUBMODULE_PATH@/commonmark-spec";
    inline static const std::string MARKDOWN_TOKENIZER_TEST_PATH               = "@TEST_FILES_PATH@/markdown/tokeniser";
    inline static const std::string MARKDOWN_TREECONSTRUCTION_TEST_PATH        = "@TEST_FILES_PATH@/markdown/tree-construction";
}

#endif //BLOGATOR_SOURCES_H
