#ifndef BLOGATOR_HTML_DOM_DOT_DOTNODE_H
#define BLOGATOR_HTML_DOM_DOT_DOTNODE_H

#include <memory>
#include <vector>
#include <map>
#include <sstream>

#include "../../exception/DOMException.h"
#include "../../encoding/encoding.h"
#include "../html5/Tag.h"
#include "../html5/Specs.h"

namespace blogator::dom {
    /* These nodes are not meant to be copied nor deleted outside the destruction phase.
     * Once the tree structure is constructed then it is assumed it will not be modified.
     * A raw pointer (*) is used to keep track of the parent so as long as the previous statements
     * are held true then everything should be 'hunky-dory'. */
    class DOTNode {
      public:
        DOTNode();
        explicit DOTNode( html5::Tag tag, std::u32string text = U"" );

        DOTNode( const DOTNode & node ) = delete;
        DOTNode( DOTNode && node ) noexcept ;

        DOTNode & operator =( const DOTNode &node ) = delete;
        DOTNode & operator =( DOTNode &&node ) noexcept = delete;

        bool operator ==( const DOTNode &rhs ) const;
        bool operator !=( const DOTNode &rhs ) const;

        [[nodiscard]] std::unique_ptr<DOTNode> recursiveCopy() const;

        void setTextContent( std::u32string text );
        DOTNode & addChild( std::unique_ptr<DOTNode> node );

        [[nodiscard]] const std::u32string & content() const;
        [[nodiscard]] const DOTNode * parent() const;

        [[nodiscard]] std::u32string attribute( const std::u32string &key ) const;
        [[nodiscard]] bool hasAttribute() const;
        [[nodiscard]] bool hasAttribute( const std::u32string &key ) const;

        bool addAttribute( const std::u32string &key, std::u32string value );
        void replaceAttribute( const std::u32string &key, const std::u32string &value );

        [[nodiscard]] size_t childrenCount() const;
        [[nodiscard]] html5::Tag type() const;

        /**
         * Output stream operator - DFS prints from node as UTF-8 encoded condensed HTML
         * @param os   Output stream
         * @param node DOTNode to print out
         * @return Output stream
         */
        friend std::ostream & operator <<( std::ostream &os, const DOTNode &node ) {
            { //opening tag + attributes
                u32stringstream_t ss;
                ss << U"<" << html5::Specs::tagToStr( node._tag );
                node.printAttributes( ss );
                ss << U">";
                ss << node.content();

                encoding::encodeToUTF8( os, ss.str() );
            }

            for( const auto &n : node._children )
                os << *n;

            { //closing tag
                u32stringstream_t ss;
                ss << U"</" << html5::Specs::tagToStr( node._tag ) << U">";

                encoding::encodeToUTF8( os, ss.str() );
            }

            return os;
        }

        void printAttributes( u32ostream_t &os ) const;

      private:
        friend class DOT;
        friend class Parser;
        const html5::Tag                         _tag;
        std::u32string                           _text;
        DOTNode *                                _parent;
        std::vector<std::unique_ptr<DOTNode>>    _children;
        std::map<std::u32string, std::u32string> _attributes;

        std::unique_ptr<DOTNode> recursiveCopy( DOTNode *parent ) const;
    };
}

#endif //BLOGATOR_HTML_DOM_DOT_DOTNODE_H
