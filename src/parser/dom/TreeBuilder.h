#ifndef BLOGATOR_PARSER_DOM_TREECONTRUCTOR_H
#define BLOGATOR_PARSER_DOM_TREECONTRUCTOR_H

#include <deque>
#include <functional>

#include "DOM.h"
#include "../specs/blogator/Specifications.h"
#include "../specs/html5/Specifications.h"
//#include "../specs/markdown/Specifications.h"
#include "../token//blogator/tokens.h"
#include "../token/html5/tokens.h"
#include "../token/markdown/tokens.h"

namespace blogator::parser::dom {
    typedef blogator::parser::specs::html5::TokeniserState HtmlState_e;
    typedef blogator::parser::specs::html5::Namespace      HtmlNamespace_e;

    class TreeBuilder {
      public:
        TreeBuilder() = default;
        virtual ~TreeBuilder() {}; // = default;

        virtual void addToken( std::unique_ptr<tokeniser::BlogatorToken> token ) {}; //TODO
        virtual void addToken( std::unique_ptr<token::html5::HTML5Tk> token );
        virtual void addToken( std::unique_ptr<tokeniser::MarkdownToken> token ) {}; //TODO

        virtual void setChangeHTML5TokeniserStateCallback( std::function<void(specs::html5::TokeniserState)> cb );

        [[nodiscard]] virtual bool hasAdjustedCurrentNode() const;
        virtual std::pair<HtmlState_e, HtmlNamespace_e> adjustedCurrentNode();

        virtual specs::html5::Element lastOpenElement() const {}; //TODO

      private:
        const bool scripting { false };

        std::function<void( specs::html5::TokeniserState )> _changeHTML5TokeniserState_cb;

        struct {
            specs::html5::InsertionMode       mode;
            std::deque<specs::html5::Element> open_elements;
        } html5;
    };
}

#endif //BLOGATOR_PARSER_DOM_TREECONTRUCTOR_H
