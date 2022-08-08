#ifndef BLOGATOR_PARSER_DOM_HTMLBUILDER_H
#define BLOGATOR_PARSER_DOM_HTMLBUILDER_H

#include <deque>
#include <stack>
#include <functional>

#include "defs.h"
#include "enum/ValidationState.h"
#include "../specs/infra/specifications.h"
#include "../token/html5/tokens.h"
#include "../dom/node/Document.h"

namespace blogator::parser::token::html5 {
    class GenericTagTk;
    class StartTagTk;
    class EndTagTk;
}

namespace blogator::parser::dom {
    /**
     * HTML5 DOM tree constructor
     */
    class TreeBuilder {
      public:
        explicit TreeBuilder( bool scripting = false );

        void setStrictChecking( bool flag = true );
        void setTokeniserStateChangeCallback( std::function<void( specs::infra::TokeniserState )> cb );
        virtual void setSelfClosingTagAckCallback( std::function<void()> cb );

        void init( std::unique_ptr<node::Document> document );
        void init( node::Element * context_element, TextPos position = { 0, 0 } );
        virtual void dispatch( std::unique_ptr<token::html5::HTML5Tk> token );
        std::unique_ptr<node::Document> reset();

        [[nodiscard]] virtual std::pair<bool, specs::infra::Namespace> adjustedCurrentNodeNS() const;
        [[nodiscard]] size_t errors() const;

        static std::unique_ptr<node::Document> createHtmlDocument( USVString_t url, std::filesystem::path origin, QuirksMode quirks = QuirksMode::NO_QUIRKS );

      private:
        typedef specs::infra::Element                       Element_e;
        typedef specs::infra::Namespace                     Namespace_e;
        typedef specs::infra::InsertionMode                 InsertionMode_e;
        typedef std::deque<node::Element *>                 OpenElements_t;
        typedef node::Element *                             FormattingElement_t;
        typedef std::deque<FormattingElement_t>             ActiveFormattingElements_t;
        typedef std::vector<char32_t>                       CharacterBuffer_t;

        static constexpr node::Element *                    FORMATTING_MARKER          { nullptr };
        static constexpr bool                               TREAT_AS_ANY_OTHER_END_TAG { true    };

        size_t                                              _error_count;
        ValidationState                                     _strict_validation;
        bool                                                _scripting;
        bool                                                _fragment_case;
        bool                                                _foster_parenting;
        bool                                                _frameset_ok;
        bool                                                _parser_cannot_change_the_mode;
        bool                                                _ignore_next_token_in_body_LF;

        std::unique_ptr<node::Document>                     _document;
        node::Element *                                     _context_element;
        node::Element *                                     _head_element;
        node::Element *                                     _form_element;

        std::deque<node::Element *>                         _open_elements;       //pushed from the front
        InsertionMode_e                                     _original_insert_mode;
        InsertionMode_e                                     _current_insert_mode;
        std::stack<InsertionMode_e>                         _template_insert_modes;
        ActiveFormattingElements_t                          _formatting_elements; //pushed from the front
        CharacterBuffer_t                                   _pending_table_char_tokens;

        std::function<void( specs::infra::TokeniserState )> _changeHTML5TokeniserState_cb;
        std::function<void()>                               _acknowledgeSelfClosingTag_cb;

        void logError( TextPos position, int err_code, const std::string & desc );
        void logError( int err_code, const std::unique_ptr<token::html5::HTML5Tk> & token );

        void processAsForeignContent( std::unique_ptr<token::html5::HTML5Tk> token, Namespace_e adj_ns );
        void processAsHTMLContent( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved = Element_e::UNKNOWN );
        template<InsertionMode_e mode> void processAsHTMLContent( std::unique_ptr<token::html5::HTML5Tk> token, Element_e resolved );

        [[nodiscard]] bool isIFrameSrcDoc() const;

        void setCurrentInsertMode( InsertionMode_e mode );
        void setOriginalInsertMode( InsertionMode_e mode );
        void setHeadElementPtr( node::Element * head_ptr );
        void setFormElementPtr( node::Element * form_ptr );

        /* 'Stack of Opened Elements' methods */
        [[nodiscard]] node::Element * currentNode();
        [[nodiscard]] OpenElements_t::iterator currentNodeIterator();
        [[nodiscard]] OpenElements_t::const_iterator currentNodeIterator() const;
        [[nodiscard]] Element_e currentNodeType() const;
        [[nodiscard]] node::Element * getOpenElement( size_t index_from_top );
        [[nodiscard]] node::Element * adjustedCurrentNode();
        void pushOpenElements( node::Element * node );
        void popOpenElements();
        void popOpenElementsUntil( Element_e element, bool inclusive );
        void popOpenElementsUntil( const DOMString_t & element, bool inclusive );
        void popOpenElementsUntil( const std::set<Element_e> & elements, bool inclusive );
        [[nodiscard]] bool hasOpenElement( Element_e element ) const;
        [[nodiscard]] OpenElements_t::iterator findOpenElement( const node::Element * node_ptr );
        [[nodiscard]] OpenElements_t::iterator findFurthestBlockInOpenElements( OpenElements_t::iterator ref_it );
        OpenElements_t::iterator removeOpenElement( const OpenElements_t::iterator & it );

        /* 'List of Active Formatting Element' methods */
        void pushActiveFormattingElement( node::Element * node );
        void reconstructActiveFormattingElements( const TextPos & position );
        void clearActiveFormattingElementsUpToLastMarker();
        [[nodiscard]] ActiveFormattingElements_t::iterator findActiveFormattingElement( const node::Element * node_ptr );
        [[nodiscard]] ActiveFormattingElements_t::iterator findActiveFormattingElementAfterLastMarker( Element_e element );
        ActiveFormattingElements_t::iterator insertActiveFormattingElement( size_t index, node::Element * element );
        ActiveFormattingElements_t::iterator removeActiveFormattingElement( const ActiveFormattingElements_t::iterator & it );

        /* 'Stack of Template Insertion Modes' methods */
        void popTemplateInsertionModes();
        void pushTemplateInsertionModes( InsertionMode_e mode );

        node::Element * insertHtmlElement( token::html5::StartTagTk * token, Element_e resolved_element );
        node::Element * insertForeignElement( token::html5::StartTagTk * token, Namespace_e ns_hint );
        void insertCharacters( const DOMString_t &text );
        template<typename InputIt> void insertCharacters( InputIt begin, InputIt end );
        void insertGenericRawTextElement( token::html5::StartTagTk * token );
        void insertGenericRCDATAElement( token::html5::StartTagTk * token );
        void insertComment( const std::unique_ptr<token::html5::HTML5Tk> & token, InsertionPoint position = { nullptr,nullptr } );
        dom::InsertionPoint appropriateInsertionPoint( node::Node * override_target = nullptr );
        void resetInsertionModeAppropriately();
        void generateImpliedEndTags( const std::set<Element_e> & skip_el = {} );
        void generateImpliedEndTags( const std::set<DOMString_t> & skip_el );
        void generateAllImpliedEndTagsThoroughly();
        bool runAdoptionAgencyAlgorithm( const token::html5::GenericTagTk * token , Element_e resolved_element );
        void closeElement( Element_e el, const TextPos & pos );
        void closeElement( const DOMString_t & el, const TextPos & pos );
        void clearStackBackToTableContext();
        void clearStackBackToTableBodyContext();
        void clearStackBackToTableRowContext();
        void closeCell( const TextPos & pos );

        [[nodiscard]] bool hasTargetInSpecificScope( Element_e target, const std::set<Element_e> & element_types, bool exclusive = true ) const;
        [[nodiscard]] bool hasParticularElementInScope( Element_e target ) const;
        [[nodiscard]] bool hasParticularElementInListItemScope( Element_e target ) const;
        [[nodiscard]] bool hasParticularElementInButtonScope( Element_e target ) const;
        [[nodiscard]] bool hasParticularElementInTableScope( Element_e target ) const;
        [[nodiscard]] bool hasParticularElementInSelectScope( Element_e target ) const;

        void stopParsing();

        Element_e resolveElement( Element_e curr_element, Namespace_e ns_hint, DOMString_t & tag_name );
        static bool isSpecialElement( Element_e element, const std::set<Element_e> & exceptions );
        static bool isFormattingElement( Element_e element );
        static bool isMathMLIntegrationPoint( node::Element * node );
        static bool isHTMLIntegrationPoint( node::Element * node );
        static bool validateStartTagInForeignContent( token::html5::StartTagTk & token ) ;
        static bool validateEndTagInForeignContent( token::html5::EndTagTk & token ) ;
        static bool isXLink( const DOMString_t &attr_name );
        static bool isXML( const DOMString_t &attr_name );
        static bool isXMLNS( const DOMString_t &attr_name );

        static void printDBG( const std::unique_ptr<token::html5::HTML5Tk> & token ); //TODO remove once everything is finished
    };
}

namespace blogator {
    std::string to_string( std::deque<blogator::parser::dom::node::Element *> & elements );
}

#endif //BLOGATOR_PARSER_DOM_HTMLBUILDER_H