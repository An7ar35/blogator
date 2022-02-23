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
                                            // </html>
        }
    );

    return std::move( pre_order );
}

/**
 * Creates a test document (w/ 'id' & 'class' attributes)
 * @param root Unique pointer to set the Document node at
 * @return Vector of node pointers pre-order (to compare iteration with)
 */
std::vector<blogator::parser::dom::node::Node *> blogator::tests::createTestDOM_2( std::unique_ptr<blogator::parser::dom::node::Node> &root ) {
    auto pre_order = std::vector<node::Node *>();

    root = std::make_unique<node::Document>();
    auto * cdata    = root->appendChild( std::make_unique<node::CDATASection>( U"cdata" ) );
    auto * html     = root->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::infra::Element::HTML5_HTML ) );
    auto * head     = html->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::infra::Element::HTML5_HEAD ) );
    dynamic_cast<node::Element *>( head )->createAttribute( U"class", U"header" );
    auto * div1     = head->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    auto * h1       = div1->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H1 ) );
    auto * h1_txt   = h1->appendChild( std::make_unique<node::Text>( U"Heading 1" ) );
    auto * comment  = html->appendChild( std::make_unique<node::Comment>( U"comment" ) );
    auto * body     = html->appendChild( std::make_unique<node::Element>(  blogator::parser::specs::infra::Element::HTML5_BODY ) );
    auto * div2     = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    dynamic_cast<node::Element *>( div2 )->createAttribute( U"id", U"id-a" );
    auto * h2       = div2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H2 ) );
    dynamic_cast<node::Element *>( h2 )->createAttribute( U"class", U"title" );
    auto * h2_txt   = h2->appendChild( std::make_unique<node::Text>( U"Heading 2" ) );
    auto * h3_1     = div2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H3 ) );
    dynamic_cast<node::Element *>( h3_1 )->createAttribute( U"class", U"title paragraph-heading" );
    auto * h3_1_txt = h3_1->appendChild( std::make_unique<node::Text>( U"Sub-Title 1" ) );
    auto * p1       = div2->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_P ) );
    dynamic_cast<node::Element *>( p1 )->createAttribute( U"class", U"p-section" );
    auto * p1_txt1  = p1->appendChild( std::make_unique<node::Text>( U"Paragraph 1" ) );
    auto * div3     = body->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_DIV ) );
    dynamic_cast<node::Element *>( div3 )->createAttribute( U"id", U"id-b" );
    auto * h3_2     = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_H3 ) );
    dynamic_cast<node::Element *>( h3_2 )->createAttribute( U"class", U"title paragraph-heading" );
    auto * h3_2_txt = h3_2->appendChild( std::make_unique<node::Text>( U"Sub-Title 2" ) );
    auto * p2       = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_P ) );
    dynamic_cast<node::Element *>( p2 )->createAttribute( U"class", U"p-section" );
    auto * p2_txt1  = p2->appendChild( std::make_unique<node::Text>( U"Paragraph 2" ) );
    auto * p3       = div3->appendChild( std::make_unique<node::Element>( blogator::parser::specs::infra::Element::HTML5_P ) );
    dynamic_cast<node::Element *>( p3 )->createAttribute( U"class", U"p-section" );
    dynamic_cast<node::Element *>( p3 )->createAttribute( U"id", U"p3" );
    auto * p3_txt2  = p2->appendChild( std::make_unique<node::Text>( U"Paragraph 3" ) );


    pre_order.insert(
        pre_order.end(), { root.get(),
        cdata,                            // <![CDATA[cdata]]>
        html,                             // <html>
        head,                             //   <head class='header'>
        div1,                             //     <div>
        h1, h1_txt,                       //       <h1>Heading 1</h1>
                                          //   </head>
        comment,                          //   <!--comment-->
        body,                             //   <body>
        div2,                             //     <div id='id-a'>
        h2, h2_txt,                       //       <h2 class='title'>Heading 2</h2>
        h3_1, h3_1_txt,                   //       <h3 class='title paragraph-heading'>Sub-Title 1</h3>
        p1, p1_txt1,                      //       <p class='p-section'>Paragraph 1</p>
                                          //     </div>
        div3,                             //     <div id='id-b'>
        h3_2, h3_2_txt,                   //       <h3 class='title paragraph-heading'>Sub-Title 2</h3>
        p2, p2_txt1,                      //       <p class='p-section'>Paragraph 2</p>
        p2, p3_txt2,                      //       <p id='p3' class='p-section'>Paragraph 3</p>
                                          //     </div>
                                          //   </body>
                                          // </html>
        }
    );

    return std::move( pre_order );
}

/**
 * Creates a test XML/XMLNS document
 * @param root Unique pointer to set the Document node at
 * @return Vector of node pointers pre-order (to compare iteration with)
 */
std::vector<blogator::parser::dom::node::Node *> blogator::tests::createTestDOM_3( std::unique_ptr<blogator::parser::dom::node::Node> &root ) {
    auto pre_order = std::vector<node::Node *>();

    root = std::make_unique<node::Document>( U"UTF-32",
                                             blogator::parser::specs::infra::ContentType::APPLICATION_XML,
                                             U"http://www.site.com/xml/info",
                                             "/src/xml/info" );

    auto xmlns  = DOMString_t( U"http://www.w3.org/2000/xmlns/" );
    auto dft_ns = DOMString_t( U"http://www.site.com/xml/default" );
    auto ug_ns  = DOMString_t( U"http://www.site.com/xml/undergrad" );
    auto pg_ns  = DOMString_t( U"http://www.site.com/xml/postgrad" );

    auto * doctype        = root->appendChild( std::make_unique<node::DocumentType>( U"xml-test-dom", U"none", U"none" ) );
    auto * r              = root->appendChild( std::make_unique<node::Element>( U"", U"root" ) );
    dynamic_cast<node::Element *>( r )->createAttributeNS( xmlns, U"xmlns", dft_ns );
    dynamic_cast<node::Element *>( r )->createAttributeNS( xmlns, U"xmlns:ug", ug_ns );
    dynamic_cast<node::Element *>( r )->createAttributeNS( xmlns, U"xmlns:pg", pg_ns );
    auto * ug_student     = r->appendChild( std::make_unique<node::Element>( ug_ns, U"ug", U"student" ) );
    auto * ug_tr          = ug_student->appendChild( std::make_unique<node::Element>( U"tr" ) );
    dynamic_cast<node::Element *>( ug_tr )->createAttribute( U"class", U"table" );
    auto * ug_td_1        = ug_tr->appendChild( std::make_unique<node::Element>( U"td" ) );
    auto * ug_name        = ug_td_1->appendChild( std::make_unique<node::Element>( ug_ns, U"ug", U"name" ) );
    auto * ug_name_txt    = ug_name->appendChild( std::make_unique<node::Text>( U"John" ) );
    auto * ug_td_2        = ug_tr->appendChild( std::make_unique<node::Element>( U"td" ) );
    auto * ug_surname     = ug_td_2->appendChild( std::make_unique<node::Element>( ug_ns, U"ug", U"surname" ) );
    auto * ug_surname_txt = ug_surname->appendChild( std::make_unique<node::Text>( U"Denver" ) );
    auto * ug_td_3        = ug_tr->appendChild( std::make_unique<node::Element>( U"td" ) );
    auto * ug_score       = ug_td_3->appendChild( std::make_unique<node::Element>( ug_ns, U"ug", U"score" ) );
    auto * ug_score_txt   = ug_score->appendChild( std::make_unique<node::Text>( U"87" ) );
    auto * pg_student     = r->appendChild( std::make_unique<node::Element>( pg_ns, U"pg", U"student" ) );
    auto * pg_tr          = pg_student->appendChild( std::make_unique<node::Element>( U"tr" ) );
    dynamic_cast<node::Element *>( pg_tr )->createAttribute( U"class", U"table" );
    auto * pg_td_1        = pg_tr->appendChild( std::make_unique<node::Element>( U"td" ) );
    auto * pg_name        = pg_td_1->appendChild( std::make_unique<node::Element>( pg_ns, U"pg", U"name" ) );
    auto * pg_name_txt    = pg_name->appendChild( std::make_unique<node::Text>( U"Maria" ) );
    auto * pg_td_2        = pg_tr->appendChild( std::make_unique<node::Element>( U"td" ) );
    auto * pg_surname     = pg_td_2->appendChild( std::make_unique<node::Element>( pg_ns, U"pg", U"surname" ) );
    auto * pg_surname_txt = pg_surname->appendChild( std::make_unique<node::Text>( U"Ebeloni" ) );
    auto * pg_td_3        = pg_tr->appendChild( std::make_unique<node::Element>( U"td" ) );
    auto * pg_score       = pg_td_3->appendChild( std::make_unique<node::Element>( pg_ns, U"pg", U"score" ) );
    auto * pg_score_txt   = pg_score->appendChild( std::make_unique<node::Text>( U"90" ) );

    pre_order.insert(
        pre_order.end(), { root.get(),
        doctype,                    //<!DOCTYPE xml-test-dom PUBLIC "none" SYSTEM "none">
        r,                          //<root xmlns="http://www.site.com/xml/default"
                                    //      xmlns:ug="http://www.site.com/xml/undergrad"
                                    //      xmlns:pg="http://www.site.com/xml/postgrad">
        ug_student,                 //  <ug:student>
        ug_tr,                      //    <tr class='table'>
        ug_td_1,                    //      <td>
        ug_name, ug_name_txt,       //        <ug:name>John</ug:name>
                                    //      </td>
        ug_td_2,                    //      <td>
        ug_surname, ug_surname_txt, //        <ug:surname>Denver</ug:surname>
                                    //      </td>
        ug_td_3,                    //      <td>
        ug_score, ug_score_txt,     //        <ug:score>87</ug:score>
                                    //      </td>
                                    //    </tr>
                                    //  </ug:student>
        pg_student,                 //  <pg:student>
        pg_tr,                      //    <tr class='table'>
        pg_td_1,                    //      <td>
        pg_name, pg_name_txt,       //        <pg:name>Maria</pg:name>
                                    //      </td>
        pg_td_2,                    //      <td>
        pg_surname, pg_surname_txt, //        <pg:surname>Ebeloni</pg:surname>
                                    //      </td>
        pg_td_3,                    //      <td>
        pg_score, pg_score_txt,     //        <pg:score>90</pg:score>
                                    //      </td>
                                    //    </tr>
                                    //  </pg:student>
                                    //</root>
        }
    );

    return std::move( pre_order );
}