#include "TestDOMs.h"

using namespace blogator::parser::dom;

/**
 * Creates a test document
 * @param root Unique pointer to set the Document node at
 * @return Vector of node pointers pre-order (to compare iteration with)
 */
std::vector<blogator::parser::dom::node::Node *> blogator::tests::createTestDOM_1( std::unique_ptr<blogator::parser::dom::node::Node> &root ) {
    auto pre_order = std::vector<node::Node *>();

    root = std::make_unique<node::Document>();
    auto * cdata    = root->appendChild( std::make_unique<node::CDATASection>( U"cdata" ) );
    auto * comment1 = root->appendChild( std::make_unique<node::Comment>( U"comment 1" ) );
    auto * html     = root->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * head     = html->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::infra::Element::HTML5_HEAD ) );
    auto * div1     = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * h1       = div1->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * h1_txt   = h1->appendChild( std::make_unique<node::Text>( U"Heading 1" ) );
    auto * head_txt = head->appendChild( std::make_unique<node::Text>( U"head text" ) );
    auto * head_a   = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_A ) );
    auto * a_span   = head_a->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_SPAN ) );
    auto * span_txt = a_span->appendChild( std::make_unique<node::Text>( U"link" ) );
    auto * div2     = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * head_h4  = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H4 ) );
    auto * h4_text  = head_h4->appendChild( std::make_unique<node::Text>( U"h4 text" ) );
    auto * img      = div2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_IMG ) );
    auto * body     = html->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::infra::Element::HTML5_BODY ) );
    auto * comment2 = body->appendChild( std::make_unique<node::Comment>( U"comment 2" ) );
    auto * div3     = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * h2       = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    auto * h2_txt   = h2->appendChild( std::make_unique<node::Text>( U"Heading 2" ) );
    auto * h3       = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H3 ) );
    auto * h3_txt   = h3->appendChild( std::make_unique<node::Text>( U"Heading 3" ) );
    auto * div4     = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * p1       = div4->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_P ) );
    auto * p1_txt1  = p1->appendChild( std::make_unique<node::Text>( U"Paragraph 1.1\n" ) );
    auto * p1_txt2  = p1->appendChild( std::make_unique<node::Text>( U"Paragraph 1.2" ) );
    auto * p2       = div4->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_P ) );
    auto * p2_txt2  = p2->appendChild( std::make_unique<node::Text>( U"Paragraph 2: " ) );
    auto * p2_bold  = p2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_B ) );
    auto * p2_txt3  = p2_bold->appendChild( std::make_unique<node::Text>( U"bold text" ) );


    pre_order.insert(
        pre_order.end(), { root.get(),
          cdata,                            // <![CDATA[cdata]]>
          comment1,                         // <!--comment 1-->
          html,                             // <html>
            head,                           //   <head>
              div1,                         //     <div>
                h1, h1_txt,                 //       <h1>Heading 1</h1>
                                            //     </div>
              head_txt,                     //  head text
              head_a,                       //     <a>
                a_span, span_txt,           //       <span>link</span>
                                            //     </a>
              div2,                         //     <div>
                img,                        //       <img />
                                            //     </div>
              head_h4, h4_text,             //     <h4>h4 text</h4>
                                            //   </head>
            body,                           //   <body>
              comment2,                     //     <!--comment 2-->
              div3,                         //     <div>
                h2, h2_txt,                 //       <h2>Heading 2</h2>
                h3, h3_txt,                 //       <h3>Heading 3</h3>
                                            //     </div>
              div4,                         //     <div>
              p1, p1_txt1,                  //       <p>Paragraph 1.1
                  p1_txt2,                  // Paragraph 1.2</p>
              p2, p2_txt2, p2_bold, p2_txt3 //       <p>Paragraph 2: <b>bold text</b></p>
                                            //     </div>
                                            //   </body>
        }
    );

    return std::move( pre_order );
}
