#ifndef BLOGATOR_TESTDOMS_H
#define BLOGATOR_TESTDOMS_H

#include <memory>
#include <vector>

#include "../../../src/parser/dom/node/Attr.h"
#include "../../../src/parser/dom/node/CDATASection.h"
#include "../../../src/parser/dom/node/Comment.h"
#include "../../../src/parser/dom/node/Document.h"
#include "../../../src/parser/dom/node/DocumentFragment.h"
#include "../../../src/parser/dom/node/DocumentType.h"
#include "../../../src/parser/dom/node/Element.h"
#include "../../../src/parser/dom/node/Text.h"

namespace blogator::tests {
    std::vector<blogator::parser::dom::node::Node *> createTestDOM_1( std::unique_ptr<blogator::parser::dom::node::Node> &root );
}

#endif //BLOGATOR_TESTDOMS_H