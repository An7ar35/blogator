#ifndef BLOGATOR_PARSER_DOM_TREECONTRUCTOR_H
#define BLOGATOR_PARSER_DOM_TREECONTRUCTOR_H

#include <deque>
#include <functional>

#include "DOM.h"
#include "../specs/native/Specifications.h"
#include "../specs/html5/Specifications.h"
//#include "../specs/markdown/Specifications.h"
#include "../token/native/tokens.h"
#include "../token/html5/tokens.h"
#include "../token/markdown/tokens.h"
#include "../token/native/NativeTk.h"

namespace blogator::parser::dom {
    typedef blogator::parser::specs::html5::TokeniserState HtmlState_e;
    typedef blogator::parser::specs::html5::Namespace      HtmlNamespace_e;

    class TreeBuilder {
      public:
        explicit TreeBuilder( DOM &dom );
        virtual ~TreeBuilder() = default;

        virtual void addToken( std::unique_ptr<token::native::NativeTk> token );
        virtual void addToken( std::unique_ptr<token::html5::HTML5Tk> token );
        virtual void addToken( std::unique_ptr<token::markdown::MarkdownTk> token );

        virtual void setChangeHTML5TokeniserStateCb( std::function<void( specs::html5::TokeniserState)> cb );

        [[nodiscard]] virtual bool hasAdjustedCurrentNode() const;
        virtual std::pair<HtmlState_e, HtmlNamespace_e> adjustedCurrentNode();

        virtual specs::native::Element lastOpenedBlogatorElement() const {} //TODO
        virtual specs::html5::Element lastOpenedHTML5Element() const {}; //TODO
//        virtual specs::markdown::Element lastOpenedMakrdownElement() const {}; //TODO



      private:
        DOM & _dom;

        void dispatchToken( std::unique_ptr<token::html5::HTML5Tk> token );

        const bool scripting { false };

        std::function<void( specs::html5::TokeniserState )> _changeHTML5TokeniserState_cb;

        struct {
            std::deque<specs::native::Element> open_elements;
        } _blogator;

        struct {
            specs::html5::InsertionMode       mode;
            std::deque<specs::html5::Element> open_elements;
        } _html5;

        struct {

        } _markdown;
    };
}

#endif //BLOGATOR_PARSER_DOM_TREECONTRUCTOR_H
