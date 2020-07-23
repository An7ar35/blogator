#ifndef BLOGATOR_HTML_DOM_DOT_DOT_H
#define BLOGATOR_HTML_DOM_DOT_DOT_H

#include <stack>
#include <filesystem>

#include "DOTNode.h"
#include "../dto/GlobalMaps.h"
#include "../html5/enums/Tag.h"
#include "../dto/Text.h"
#include "../../cli/MsgInterface.h"

namespace blogator::dom {
    /**
     * Document Object Tree
     * --------------------
     * All transitional data in the Document Object Tree is formatted as UTF-32 so as to have
     * maximum character encoding coverage when it is time to convert and output to a specific
     * encoding. Once the tree is constructed during the parsing stage its node structure is
     * assumed to be immutable.
     */
    class DOT {
      public:
        DOT();
        explicit DOT( const std::string    &html );
        explicit DOT( const std::u32string &html );
        explicit DOT( const dto::Text      &html );
        explicit DOT( const std::filesystem::path &src );
        explicit DOT( std::unique_ptr<DOTNode> root );

        DOT( const DOT &dot ) = delete;
        DOT( DOT &&dot ) noexcept;

        DOT & operator =( const DOT &dot ) = delete;
        DOT & operator =( DOT &&dot ) noexcept;

        bool operator ==( const DOT &rhs ) const;
        bool operator !=( const DOT &rhs ) const;

        //add merge methods for output?

        [[nodiscard]] const dom::DOTNode & getElementById( const std::u32string &id ) const;
        [[nodiscard]] std::vector<const dom::DOTNode *> getElementByClass( const std::u32string &class_name ) const;

        /**
         * Print out content in-order
         * @param os  Output stream
         * @param dot DOT object
         * @return Output stream
         */
        friend std::ostream & operator <<( std::ostream &os, const DOT &dot ) {
            if( dot._root )
                os << *dot._root;
            return os;
        }

        std::ostream & prettyPrint( std::ostream &os ) const;

      private:
        cli::MsgInterface &                                         _display;
        std::unique_ptr<DOTNode>                                    _root;
        dto::GlobalMaps                                             _global_maps;
        std::map<std::u32string, const dom::DOTNode *>              _css_id_map;
        std::map<std::u32string, std::vector<const dom::DOTNode *>> _css_class_map;

        void parse( const dto::Text &text );

        [[nodiscard]] static dto::Text readFile( const std::filesystem::path &path ) ;
    };
}

#endif //BLOGATOR_HTML_DOM_DOT_DOT_H