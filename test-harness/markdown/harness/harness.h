#ifndef TEST_HARNESS_MARKDOWN_HARNESS_HARNESS_H
#define TEST_HARNESS_MARKDOWN_HARNESS_HARNESS_H

#include <ostream>
#include <vector>

#include "MarkdownTkTest.h"
#include "MarkdownToHtmlTest.h"

#include "../../../src/parser/token/markdown/MarkdownTk.h"
#include "../../../src/reporter/dto/ReporterObject.h"
#include "../../../src/parser/dom/node/Document.h"

namespace test_harness::markdown {
    std::string to_string( const blogator::parser::token::markdown::MarkdownTk & token );
    std::string to_string( const blogator::reporter::ReporterObject & error );

    std::vector<std::pair<MarkdownTkTest, std::filesystem::path>> loadTokeniserTests( const std::filesystem::path &test_dir );
    std::vector<std::pair<MarkdownToHtmlTest, std::filesystem::path>> loadTreeBuilderTests( const std::filesystem::path &test_dir );
    std::string printDocumentBody( blogator::parser::dom::node::Document & document );
}

#endif //TEST_HARNESS_MARKDOWN_HARNESS_HARNESS_H