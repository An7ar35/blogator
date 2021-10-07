#include "TreeBuilder.h"

#include <utility>

using namespace blogator::parser::dom;
typedef blogator::parser::specs::html5::TokeniserState HtmlState_e;
typedef blogator::parser::specs::html5::Namespace      HtmlNamespace_e;

//When a start tag token is emitted with its self-closing flag set, if the flag is not acknowledged
//when it is processed by the tree construction stage, that is a
//non-void-html-element-start-tag-with-trailing-solidus parse error.


void TreeBuilder::addToken( std::unique_ptr<token::html5::HTML5Tk> token ) { //TODO

}

void TreeBuilder::setChangeHTML5TokeniserStateCallback( std::function<void( specs::html5::TokeniserState )> cb ) {
    _changeHTML5TokeniserState_cb = std::move( cb );
}


bool TreeBuilder::hasAdjustedCurrentNode() const {
    return false;
}

/**
 *
 * @return { }
 */
std::pair<HtmlState_e, HtmlNamespace_e> TreeBuilder::adjustedCurrentNode() {
    return { HtmlState_e::RCDATA, HtmlNamespace_e::HTML5 };
}
