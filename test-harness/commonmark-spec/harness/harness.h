#ifndef TEST_HARNESS_COMMONMARK_SPEC_HARNESS_HARNESS_H
#define TEST_HARNESS_COMMONMARK_SPEC_HARNESS_HARNESS_H

#include <vector>

#include "../../../src/parser/dom/node/Document.h"
#include "CommonMarkTest.h"

namespace test_harness::commonmark_spec_tests {
    std::vector<std::pair<CommonMarkTest, std::filesystem::path>> loadMarkdownTests( const std::filesystem::path &test_dir );
    std::string printDocumentBody( blogator::parser::dom::node::Document & document );
}

#endif //TEST_HARNESS_COMMONMARK_SPEC_HARNESS_HARNESS_H