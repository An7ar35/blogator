#ifndef TEST_HARNESS_HTML5LIB_TESTS_HARNESS_TREECONSTRUCTIONTEST_H
#define TEST_HARNESS_HTML5LIB_TESTS_HARNESS_TREECONSTRUCTIONTEST_H

#include <string>
#include <vector>
#include <filesystem>
#include <ostream>

namespace test_harness::html5lib_tests {
    struct TreeConstructionTest {
        typedef std::string              Data_t;
        typedef std::vector<std::string> Errors_t;
        typedef std::string              Output_t;

        size_t                id              {};
        size_t                line            {};
        std::filesystem::path src             {};
        Data_t                data            {};
        bool                  is_fragment     { false };
        Data_t                ctx_prefix      {};
        Data_t                ctx_local_name  {};
        Errors_t              errors          {};
        bool                  scripting       { false };
        Output_t              expected_output {};
    };

    std::ostream & operator <<( std::ostream &os, const TreeConstructionTest &test );
    std::string to_string( const TreeConstructionTest::Errors_t &errors );
}

#endif //TEST_HARNESS_HTML5LIB_TESTS_HARNESS_TREECONSTRUCTIONTEST_H