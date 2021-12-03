#include "TreeBuilder.h"
#include "../token/native/NativeTk.h"

#include <utility>

using namespace blogator::parser::dom;

/**
 * Constructor
 * @param dom
 */
TreeBuilder::TreeBuilder( DOM &dom ) :
    _dom( dom )
{} //TODO

//When a start tag token is emitted with its self-closing flag set, if the flag is not acknowledged
//when it is processed by the tree construction stage, that is a
//non-void-html-element-start-tag-with-trailing-solidus parse error.

void TreeBuilder::addToken( std::unique_ptr<token::native::NativeTk> token ) { //TODO


//    If the stack of open elements is empty
//    If the adjusted current node is an element in the HTML namespace
//    If the adjusted current node is a MathML text integration point and the token is a start tag whose tag name is neither "mglyph" nor "malignmark"
//    If the adjusted current node is a MathML text integration point and the token is a character token
//    If the adjusted current node is a MathML annotation-xml element and the token is a start tag whose tag name is "svg"
//    If the adjusted current node is an HTML integration point and the token is a start tag
//    If the adjusted current node is an HTML integration point and the token is a character token
//    If the token is an end-of-file token
//    Process the token according to the rules given in the section corresponding to the current insertion mode in HTML content.
//        Otherwise
//    Process the token according to the rules given in the section for parsing tokens in foreign content.
}

void TreeBuilder::addToken( std::unique_ptr<token::html5::HTML5Tk> token ) { //TODO

}

void TreeBuilder::addToken( std::unique_ptr<token::markdown::MarkdownTk> token ) { //TODO

}

void TreeBuilder::setChangeHTML5TokeniserStateCb( std::function<void( specs::html5::TokeniserState )> cb ) {
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



void TreeBuilder::dispatchToken( std::unique_ptr<token::html5::HTML5Tk> token ) {

}
