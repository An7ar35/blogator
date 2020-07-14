#include "gtest/gtest.h"
#include "../../../src/dom/html5/Specs.h"

TEST( html5_Tests, strToTag_invalid_arg ) {
    using blogator::dom::html5::Specs;
    ASSERT_THROW( Specs::strToTag( U"invalid_tag" ), std::invalid_argument );
}

TEST( html5_Tests, strToTag_a ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::A, Specs::strToTag( U"a" ) );
    ASSERT_EQ( Tag::A, Specs::strToTag( U"A" ) );
}

TEST( html5_Tests, strToTag_abbr ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::ABBR, Specs::strToTag( U"abbr" ) );
    ASSERT_EQ( Tag::ABBR, Specs::strToTag( U"ABBR" ) );
    ASSERT_EQ( Tag::ABBR, Specs::strToTag( U"aBbR" ) );
}

TEST( html5_Tests, strToTag_address ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::ADDRESS, Specs::strToTag( U"address" ) );
    ASSERT_EQ( Tag::ADDRESS, Specs::strToTag( U"ADDRESS" ) );
    ASSERT_EQ( Tag::ADDRESS, Specs::strToTag( U"AdDrEsS" ) );
}

TEST( html5_Tests, strToTag_area ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::AREA, Specs::strToTag( U"area" ) );
    ASSERT_EQ( Tag::AREA, Specs::strToTag( U"AREA" ) );
    ASSERT_EQ( Tag::AREA, Specs::strToTag( U"ArEa" ) );
}

TEST( html5_Tests, strToTag_article ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::ARTICLE, Specs::strToTag( U"article" ) );
    ASSERT_EQ( Tag::ARTICLE, Specs::strToTag( U"ARTICLE" ) );
    ASSERT_EQ( Tag::ARTICLE, Specs::strToTag( U"ArTiCle" ) );
}

TEST( html5_Tests, strToTag_aside ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::ASIDE, Specs::strToTag( U"aside" ) );
    ASSERT_EQ( Tag::ASIDE, Specs::strToTag( U"ASIDE" ) );
    ASSERT_EQ( Tag::ASIDE, Specs::strToTag( U"aSiDe" ) );
}

TEST( html5_Tests, strToTag_audio ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::AUDIO, Specs::strToTag( U"audio" ) );
    ASSERT_EQ( Tag::AUDIO, Specs::strToTag( U"AUDIO" ) );
    ASSERT_EQ( Tag::AUDIO, Specs::strToTag( U"AuDiO" ) );
}

TEST( html5_Tests, strToTag_b ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::B, Specs::strToTag( U"b" ) );
    ASSERT_EQ( Tag::B, Specs::strToTag( U"B" ) );
}

TEST( html5_Tests, strToTag_base ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BASE, Specs::strToTag( U"base" ) );
    ASSERT_EQ( Tag::BASE, Specs::strToTag( U"BASE" ) );
    ASSERT_EQ( Tag::BASE, Specs::strToTag( U"BaSe" ) );
}

TEST( html5_Tests, strToTag_bdi ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BDI, Specs::strToTag( U"bdi" ) );
    ASSERT_EQ( Tag::BDI, Specs::strToTag( U"BDI" ) );
    ASSERT_EQ( Tag::BDI, Specs::strToTag( U"BdI" ) );
}

TEST( html5_Tests, strToTag_bdo ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BDO, Specs::strToTag( U"bdo" ) );
    ASSERT_EQ( Tag::BDO, Specs::strToTag( U"BDO" ) );
    ASSERT_EQ( Tag::BDO, Specs::strToTag( U"BdO" ) );
}

TEST( html5_Tests, strToTag_blockquote ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BLOCKQUOTE, Specs::strToTag( U"blockquote" ) );
    ASSERT_EQ( Tag::BLOCKQUOTE, Specs::strToTag( U"BLOCKQUOTE" ) );
    ASSERT_EQ( Tag::BLOCKQUOTE, Specs::strToTag( U"BlOcKQuOtE" ) );
}

TEST( html5_Tests, strToTag_body ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BODY, Specs::strToTag( U"body" ) );
    ASSERT_EQ( Tag::BODY, Specs::strToTag( U"BODY" ) );
    ASSERT_EQ( Tag::BODY, Specs::strToTag( U"BoDy" ) );
}

TEST( html5_Tests, strToTag_br ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BR, Specs::strToTag( U"br" ) );
    ASSERT_EQ( Tag::BR, Specs::strToTag( U"BR" ) );
    ASSERT_EQ( Tag::BR, Specs::strToTag( U"Br" ) );
}

TEST( html5_Tests, strToTag_button ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::BUTTON, Specs::strToTag( U"button" ) );
    ASSERT_EQ( Tag::BUTTON, Specs::strToTag( U"BUTTON" ) );
    ASSERT_EQ( Tag::BUTTON, Specs::strToTag( U"BuTtOn" ) );
}

TEST( html5_Tests, strToTag_canvas ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::CANVAS, Specs::strToTag( U"canvas" ) );
    ASSERT_EQ( Tag::CANVAS, Specs::strToTag( U"CANVAS" ) );
    ASSERT_EQ( Tag::CANVAS, Specs::strToTag( U"CaNvAS" ) );
}

TEST( html5_Tests, strToTag_caption ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::CAPTION, Specs::strToTag( U"caption" ) );
    ASSERT_EQ( Tag::CAPTION, Specs::strToTag( U"CAPTION" ) );
    ASSERT_EQ( Tag::CAPTION, Specs::strToTag( U"CaPtIoN" ) );
}

TEST( html5_Tests, strToTag_cite ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::CITE, Specs::strToTag( U"cite" ) );
    ASSERT_EQ( Tag::CITE, Specs::strToTag( U"CITE" ) );
    ASSERT_EQ( Tag::CITE, Specs::strToTag( U"CiTe" ) );
}
TEST( html5_Tests, strToTag_code ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::CODE, Specs::strToTag( U"code" ) );
    ASSERT_EQ( Tag::CODE, Specs::strToTag( U"CODE" ) );
    ASSERT_EQ( Tag::CODE, Specs::strToTag( U"CoDe" ) );
}
TEST( html5_Tests, strToTag_col ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::COL, Specs::strToTag( U"col" ) );
    ASSERT_EQ( Tag::COL, Specs::strToTag( U"COL" ) );
    ASSERT_EQ( Tag::COL, Specs::strToTag( U"CoL" ) );
}
TEST( html5_Tests, strToTag_colgroup ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::COLGROUP, Specs::strToTag( U"colgroup" ) );
    ASSERT_EQ( Tag::COLGROUP, Specs::strToTag( U"COLGROUP" ) );
    ASSERT_EQ( Tag::COLGROUP, Specs::strToTag( U"CoLGrOuP" ) );
}

TEST( html5_Tests, strToTag_comment ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::COMMENT, Specs::strToTag( U"!--" ) );
}

TEST( html5_Tests, strToTag_data ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DATA, Specs::strToTag( U"data" ) );
    ASSERT_EQ( Tag::DATA, Specs::strToTag( U"DATA" ) );
    ASSERT_EQ( Tag::DATA, Specs::strToTag( U"dAtA" ) );
}

TEST( html5_Tests, strToTag_datalist ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DATALIST, Specs::strToTag( U"datalist" ) );
    ASSERT_EQ( Tag::DATALIST, Specs::strToTag( U"DATALIST" ) );
    ASSERT_EQ( Tag::DATALIST, Specs::strToTag( U"DaTaLiSt" ) );
}

TEST( html5_Tests, strToTag_dd ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DD, Specs::strToTag( U"dd" ) );
    ASSERT_EQ( Tag::DD, Specs::strToTag( U"DD" ) );
    ASSERT_EQ( Tag::DD, Specs::strToTag( U"Dd" ) );
}

TEST( html5_Tests, strToTag_del ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DEL, Specs::strToTag( U"del" ) );
    ASSERT_EQ( Tag::DEL, Specs::strToTag( U"DEL" ) );
    ASSERT_EQ( Tag::DEL, Specs::strToTag( U"DeL" ) );
}

TEST( html5_Tests, strToTag_details ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DETAILS, Specs::strToTag( U"details" ) );
    ASSERT_EQ( Tag::DETAILS, Specs::strToTag( U"DETAILS" ) );
    ASSERT_EQ( Tag::DETAILS, Specs::strToTag( U"DeTaIlS" ) );
}

TEST( html5_Tests, strToTag_dfn ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DFN, Specs::strToTag( U"dfn" ) );
    ASSERT_EQ( Tag::DFN, Specs::strToTag( U"DFN" ) );
    ASSERT_EQ( Tag::DFN, Specs::strToTag( U"dFN" ) );
}

TEST( html5_Tests, strToTag_dialog ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DIALOG, Specs::strToTag( U"dialog" ) );
    ASSERT_EQ( Tag::DIALOG, Specs::strToTag( U"DIALOG" ) );
    ASSERT_EQ( Tag::DIALOG, Specs::strToTag( U"DiAlOg" ) );
}

TEST( html5_Tests, strToTag_div ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DIV, Specs::strToTag( U"div" ) );
    ASSERT_EQ( Tag::DIV, Specs::strToTag( U"DIV" ) );
    ASSERT_EQ( Tag::DIV, Specs::strToTag( U"dIV" ) );
}

TEST( html5_Tests, strToTag_doctype ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DOCTYPE, Specs::strToTag( U"!doctype" ) );
    ASSERT_EQ( Tag::DOCTYPE, Specs::strToTag( U"!DOCTYPE" ) );
    ASSERT_EQ( Tag::DOCTYPE, Specs::strToTag( U"!docTYPE" ) );
}

TEST( html5_Tests, strToTag_dt ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::DT, Specs::strToTag( U"dt" ) );
    ASSERT_EQ( Tag::DT, Specs::strToTag( U"DT" ) );
    ASSERT_EQ( Tag::DT, Specs::strToTag( U"dT" ) );
}

TEST( html5_Tests, strToTag_em ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::EM, Specs::strToTag( U"em" ) );
    ASSERT_EQ( Tag::EM, Specs::strToTag( U"EM" ) );
    ASSERT_EQ( Tag::EM, Specs::strToTag( U"eM" ) );
}

TEST( html5_Tests, strToTag_embed ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::EMBED, Specs::strToTag( U"embed" ) );
    ASSERT_EQ( Tag::EMBED, Specs::strToTag( U"EMBED" ) );
    ASSERT_EQ( Tag::EMBED, Specs::strToTag( U"EmBeD" ) );
}

TEST( html5_Tests, strToTag_fieldset ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::FIELDSET, Specs::strToTag( U"fieldset" ) );
    ASSERT_EQ( Tag::FIELDSET, Specs::strToTag( U"FIELDSET" ) );
    ASSERT_EQ( Tag::FIELDSET, Specs::strToTag( U"FiElDsEt" ) );
}

TEST( html5_Tests, strToTag_figcaption ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::FIGCAPTION, Specs::strToTag( U"figcaption" ) );
    ASSERT_EQ( Tag::FIGCAPTION, Specs::strToTag( U"FIGCAPTION" ) );
    ASSERT_EQ( Tag::FIGCAPTION, Specs::strToTag( U"fIgCaPtIoN" ) );
}

TEST( html5_Tests, strToTag_figure ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::FIGURE, Specs::strToTag( U"figure" ) );
    ASSERT_EQ( Tag::FIGURE, Specs::strToTag( U"FIGURE" ) );
    ASSERT_EQ( Tag::FIGURE, Specs::strToTag( U"FiGuRe" ) );
}

TEST( html5_Tests, strToTag_footer ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::FOOTER, Specs::strToTag( U"footer" ) );
    ASSERT_EQ( Tag::FOOTER, Specs::strToTag( U"FOOTER" ) );
    ASSERT_EQ( Tag::FOOTER, Specs::strToTag( U"FoOtEr" ) );
}

TEST( html5_Tests, strToTag_form ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::FORM, Specs::strToTag( U"form" ) );
    ASSERT_EQ( Tag::FORM, Specs::strToTag( U"FORM" ) );
    ASSERT_EQ( Tag::FORM, Specs::strToTag( U"FoRm" ) );
}

TEST( html5_Tests, strToTag_h1 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::H1, Specs::strToTag( U"h1" ) );
    ASSERT_EQ( Tag::H1, Specs::strToTag( U"H1" ) );
}

TEST( html5_Tests, strToTag_h2 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::H2, Specs::strToTag( U"h2" ) );
    ASSERT_EQ( Tag::H2, Specs::strToTag( U"H2" ) );
}

TEST( html5_Tests, strToTag_h3 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::H3, Specs::strToTag( U"h3" ) );
    ASSERT_EQ( Tag::H3, Specs::strToTag( U"H3" ) );
}

TEST( html5_Tests, strToTag_h4 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::H4, Specs::strToTag( U"h4" ) );
    ASSERT_EQ( Tag::H4, Specs::strToTag( U"H4" ) );
}

TEST( html5_Tests, strToTag_h5 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::H5, Specs::strToTag( U"h5" ) );
    ASSERT_EQ( Tag::H5, Specs::strToTag( U"H5" ) );
}

TEST( html5_Tests, strToTag_h6 ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::H6, Specs::strToTag( U"h6" ) );
    ASSERT_EQ( Tag::H6, Specs::strToTag( U"H6" ) );
}

TEST( html5_Tests, strToTag_head ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::HEAD, Specs::strToTag( U"head" ) );
    ASSERT_EQ( Tag::HEAD, Specs::strToTag( U"HEAD" ) );
    ASSERT_EQ( Tag::HEAD, Specs::strToTag( U"HeAd" ) );
}

TEST( html5_Tests, strToTag_header ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::HEADER, Specs::strToTag( U"header" ) );
    ASSERT_EQ( Tag::HEADER, Specs::strToTag( U"HEADER" ) );
    ASSERT_EQ( Tag::HEADER, Specs::strToTag( U"HeAdEr" ) );
}

TEST( html5_Tests, strToTag_hr ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::HR, Specs::strToTag( U"hr" ) );
    ASSERT_EQ( Tag::HR, Specs::strToTag( U"HR" ) );
    ASSERT_EQ( Tag::HR, Specs::strToTag( U"Hr" ) );
}

TEST( html5_Tests, strToTag_html ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::HTML, Specs::strToTag( U"html" ) );
    ASSERT_EQ( Tag::HTML, Specs::strToTag( U"HTML" ) );
    ASSERT_EQ( Tag::HTML, Specs::strToTag( U"HtMl" ) );
}

TEST( html5_Tests, strToTag_i ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::I, Specs::strToTag( U"i" ) );
    ASSERT_EQ( Tag::I, Specs::strToTag( U"I" ) );
}

TEST( html5_Tests, strToTag_iframe ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::IFRAME, Specs::strToTag( U"iframe" ) );
    ASSERT_EQ( Tag::IFRAME, Specs::strToTag( U"IFRAME" ) );
    ASSERT_EQ( Tag::IFRAME, Specs::strToTag( U"iFRAME" ) );
}

TEST( html5_Tests, strToTag_img ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::IMG, Specs::strToTag( U"img" ) );
    ASSERT_EQ( Tag::IMG, Specs::strToTag( U"IMG" ) );
    ASSERT_EQ( Tag::IMG, Specs::strToTag( U"ImG" ) );
}

TEST( html5_Tests, strToTag_input ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::INPUT, Specs::strToTag( U"input" ) );
    ASSERT_EQ( Tag::INPUT, Specs::strToTag( U"INPUT" ) );
    ASSERT_EQ( Tag::INPUT, Specs::strToTag( U"InPuT" ) );
}

TEST( html5_Tests, strToTag_ins ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::INS, Specs::strToTag( U"ins" ) );
    ASSERT_EQ( Tag::INS, Specs::strToTag( U"INS" ) );
    ASSERT_EQ( Tag::INS, Specs::strToTag( U"InS" ) );
}

TEST( html5_Tests, strToTag_kbd ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::KBD, Specs::strToTag( U"kbd" ) );
    ASSERT_EQ( Tag::KBD, Specs::strToTag( U"KBD" ) );
    ASSERT_EQ( Tag::KBD, Specs::strToTag( U"kBd" ) );
}

TEST( html5_Tests, strToTag_label ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::LABEL, Specs::strToTag( U"label" ) );
    ASSERT_EQ( Tag::LABEL, Specs::strToTag( U"LABEL" ) );
    ASSERT_EQ( Tag::LABEL, Specs::strToTag( U"LaBeL" ) );
}

TEST( html5_Tests, strToTag_legend ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::LEGEND, Specs::strToTag( U"legend" ) );
    ASSERT_EQ( Tag::LEGEND, Specs::strToTag( U"LEGEND" ) );
    ASSERT_EQ( Tag::LEGEND, Specs::strToTag( U"LeGeNd" ) );
}

TEST( html5_Tests, strToTag_li ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::LI, Specs::strToTag( U"li" ) );
    ASSERT_EQ( Tag::LI, Specs::strToTag( U"LI" ) );
    ASSERT_EQ( Tag::LI, Specs::strToTag( U"Li" ) );
}

TEST( html5_Tests, strToTag_link ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::LINK, Specs::strToTag( U"link" ) );
    ASSERT_EQ( Tag::LINK, Specs::strToTag( U"LINK" ) );
    ASSERT_EQ( Tag::LINK, Specs::strToTag( U"LiNk" ) );
}

TEST( html5_Tests, strToTag_main ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::MAIN, Specs::strToTag( U"main" ) );
    ASSERT_EQ( Tag::MAIN, Specs::strToTag( U"MAIN" ) );
    ASSERT_EQ( Tag::MAIN, Specs::strToTag( U"MaIn" ) );
}

TEST( html5_Tests, strToTag_map ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::MAP, Specs::strToTag( U"map" ) );
    ASSERT_EQ( Tag::MAP, Specs::strToTag( U"MAP" ) );
    ASSERT_EQ( Tag::MAP, Specs::strToTag( U"MaP" ) );
}

TEST( html5_Tests, strToTag_mark ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::MARK, Specs::strToTag( U"mark" ) );
    ASSERT_EQ( Tag::MARK, Specs::strToTag( U"MARK" ) );
    ASSERT_EQ( Tag::MARK, Specs::strToTag( U"MaRk" ) );
}

TEST( html5_Tests, strToTag_meta ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::META, Specs::strToTag( U"meta" ) );
    ASSERT_EQ( Tag::META, Specs::strToTag( U"META" ) );
    ASSERT_EQ( Tag::META, Specs::strToTag( U"meTA" ) );
}

TEST( html5_Tests, strToTag_meter ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::METER, Specs::strToTag( U"meter" ) );
    ASSERT_EQ( Tag::METER, Specs::strToTag( U"METER" ) );
    ASSERT_EQ( Tag::METER, Specs::strToTag( U"mEtEr" ) );
}

TEST( html5_Tests, strToTag_nav ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::NAV, Specs::strToTag( U"nav" ) );
    ASSERT_EQ( Tag::NAV, Specs::strToTag( U"NAV" ) );
    ASSERT_EQ( Tag::NAV, Specs::strToTag( U"NaV" ) );
}

TEST( html5_Tests, strToTag_noscript ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::NOSCRIPT, Specs::strToTag( U"noscript" ) );
    ASSERT_EQ( Tag::NOSCRIPT, Specs::strToTag( U"NOSCRIPT" ) );
    ASSERT_EQ( Tag::NOSCRIPT, Specs::strToTag( U"noSCRIPT" ) );
}

TEST( html5_Tests, strToTag_object ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::OBJECT, Specs::strToTag( U"object" ) );
    ASSERT_EQ( Tag::OBJECT, Specs::strToTag( U"OBJECT" ) );
    ASSERT_EQ( Tag::OBJECT, Specs::strToTag( U"ObJeCt" ) );
}

TEST( html5_Tests, strToTag_ol ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::OL, Specs::strToTag( U"ol" ) );
    ASSERT_EQ( Tag::OL, Specs::strToTag( U"OL" ) );
    ASSERT_EQ( Tag::OL, Specs::strToTag( U"Ol" ) );
}

TEST( html5_Tests, strToTag_optgroup ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::OPTGROUP, Specs::strToTag( U"optgroup" ) );
    ASSERT_EQ( Tag::OPTGROUP, Specs::strToTag( U"OPTGROUP" ) );
    ASSERT_EQ( Tag::OPTGROUP, Specs::strToTag( U"OptGroup" ) );
}

TEST( html5_Tests, strToTag_option ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::OPTION, Specs::strToTag( U"option" ) );
    ASSERT_EQ( Tag::OPTION, Specs::strToTag( U"OPTION" ) );
    ASSERT_EQ( Tag::OPTION, Specs::strToTag( U"OpTiOn" ) );
}

TEST( html5_Tests, strToTag_output ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::OUTPUT, Specs::strToTag( U"output" ) );
    ASSERT_EQ( Tag::OUTPUT, Specs::strToTag( U"OUTPUT" ) );
    ASSERT_EQ( Tag::OUTPUT, Specs::strToTag( U"OuTpUt" ) );
}

TEST( html5_Tests, strToTag_p ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::P, Specs::strToTag( U"p" ) );
    ASSERT_EQ( Tag::P, Specs::strToTag( U"P" ) );
}

TEST( html5_Tests, strToTag_param ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::PARAM, Specs::strToTag( U"param" ) );
    ASSERT_EQ( Tag::PARAM, Specs::strToTag( U"PARAM" ) );
    ASSERT_EQ( Tag::PARAM, Specs::strToTag( U"PaRaM" ) );
}

TEST( html5_Tests, strToTag_picture ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::PICTURE, Specs::strToTag( U"picture" ) );
    ASSERT_EQ( Tag::PICTURE, Specs::strToTag( U"PICTURE" ) );
    ASSERT_EQ( Tag::PICTURE, Specs::strToTag( U"PiCtUre" ) );
}

TEST( html5_Tests, strToTag_pre ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::PRE, Specs::strToTag( U"pre" ) );
    ASSERT_EQ( Tag::PRE, Specs::strToTag( U"PRE" ) );
    ASSERT_EQ( Tag::PRE, Specs::strToTag( U"PrE" ) );
}

TEST( html5_Tests, strToTag_progress ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::PROGRESS, Specs::strToTag( U"progress" ) );
    ASSERT_EQ( Tag::PROGRESS, Specs::strToTag( U"PROGRESS" ) );
    ASSERT_EQ( Tag::PROGRESS, Specs::strToTag( U"PrOgReSs" ) );
}

TEST( html5_Tests, strToTag_q ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::Q, Specs::strToTag( U"q" ) );
    ASSERT_EQ( Tag::Q, Specs::strToTag( U"Q" ) );
}

TEST( html5_Tests, strToTag_rp ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::RP, Specs::strToTag( U"rp" ) );
    ASSERT_EQ( Tag::RP, Specs::strToTag( U"RP" ) );
    ASSERT_EQ( Tag::RP, Specs::strToTag( U"Rp" ) );
}

TEST( html5_Tests, strToTag_rt ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::RT, Specs::strToTag( U"rt" ) );
    ASSERT_EQ( Tag::RT, Specs::strToTag( U"RT" ) );
    ASSERT_EQ( Tag::RT, Specs::strToTag( U"Rt" ) );
}
//{ U"ruby",       html5::Tag::RUBY },
TEST( html5_Tests, strToTag_ruby ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::RUBY, Specs::strToTag( U"ruby" ) );
    ASSERT_EQ( Tag::RUBY, Specs::strToTag( U"RUBY" ) );
    ASSERT_EQ( Tag::RUBY, Specs::strToTag( U"RuBy" ) );
}

TEST( html5_Tests, strToTag_s ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::S, Specs::strToTag( U"s" ) );
    ASSERT_EQ( Tag::S, Specs::strToTag( U"S" ) );
}

TEST( html5_Tests, strToTag_samp ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SAMP, Specs::strToTag( U"samp" ) );
    ASSERT_EQ( Tag::SAMP, Specs::strToTag( U"SAMP" ) );
    ASSERT_EQ( Tag::SAMP, Specs::strToTag( U"SaMp" ) );
}

TEST( html5_Tests, strToTag_script ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SCRIPT, Specs::strToTag( U"script" ) );
    ASSERT_EQ( Tag::SCRIPT, Specs::strToTag( U"SCRIPT" ) );
    ASSERT_EQ( Tag::SCRIPT, Specs::strToTag( U"ScRiPt" ) );
}

TEST( html5_Tests, strToTag_section ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SECTION, Specs::strToTag( U"section" ) );
    ASSERT_EQ( Tag::SECTION, Specs::strToTag( U"SECTION" ) );
    ASSERT_EQ( Tag::SECTION, Specs::strToTag( U"SecTion" ) );
}

TEST( html5_Tests, strToTag_select ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SELECT, Specs::strToTag( U"select" ) );
    ASSERT_EQ( Tag::SELECT, Specs::strToTag( U"SELECT" ) );
    ASSERT_EQ( Tag::SELECT, Specs::strToTag( U"SeLeCt" ) );
}

TEST( html5_Tests, strToTag_small ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SMALL, Specs::strToTag( U"small" ) );
    ASSERT_EQ( Tag::SMALL, Specs::strToTag( U"SMALL" ) );
    ASSERT_EQ( Tag::SMALL, Specs::strToTag( U"SmAlL" ) );
}

TEST( html5_Tests, strToTag_source ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SOURCE, Specs::strToTag( U"source" ) );
    ASSERT_EQ( Tag::SOURCE, Specs::strToTag( U"SOURCE" ) );
    ASSERT_EQ( Tag::SOURCE, Specs::strToTag( U"SoUrCe" ) );
}

TEST( html5_Tests, strToTag_span ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SPAN, Specs::strToTag( U"span" ) );
    ASSERT_EQ( Tag::SPAN, Specs::strToTag( U"SPAN" ) );
    ASSERT_EQ( Tag::SPAN, Specs::strToTag( U"SpAn" ) );
}

TEST( html5_Tests, strToTag_strong ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::STRONG, Specs::strToTag( U"strong" ) );
    ASSERT_EQ( Tag::STRONG, Specs::strToTag( U"STRONG" ) );
    ASSERT_EQ( Tag::STRONG, Specs::strToTag( U"StRoNg" ) );
}

TEST( html5_Tests, strToTag_style ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::STYLE, Specs::strToTag( U"style" ) );
    ASSERT_EQ( Tag::STYLE, Specs::strToTag( U"STYLE" ) );
    ASSERT_EQ( Tag::STYLE, Specs::strToTag( U"StYlE" ) );
}

TEST( html5_Tests, strToTag_sub ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SUB, Specs::strToTag( U"sub" ) );
    ASSERT_EQ( Tag::SUB, Specs::strToTag( U"SUB" ) );
    ASSERT_EQ( Tag::SUB, Specs::strToTag( U"SuB" ) );
}

TEST( html5_Tests, strToTag_summary ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SUMMARY, Specs::strToTag( U"summary" ) );
    ASSERT_EQ( Tag::SUMMARY, Specs::strToTag( U"SUMMARY" ) );
    ASSERT_EQ( Tag::SUMMARY, Specs::strToTag( U"SuMmArY" ) );
}

TEST( html5_Tests, strToTag_sup ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SUP, Specs::strToTag( U"sup" ) );
    ASSERT_EQ( Tag::SUP, Specs::strToTag( U"SUP" ) );
    ASSERT_EQ( Tag::SUP, Specs::strToTag( U"SuP" ) );
}

TEST( html5_Tests, strToTag_svg ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::SVG, Specs::strToTag( U"svg" ) );
    ASSERT_EQ( Tag::SVG, Specs::strToTag( U"SVG" ) );
    ASSERT_EQ( Tag::SVG, Specs::strToTag( U"sVg" ) );
}

TEST( html5_Tests, strToTag_table ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TABLE, Specs::strToTag( U"table" ) );
    ASSERT_EQ( Tag::TABLE, Specs::strToTag( U"TABLE" ) );
    ASSERT_EQ( Tag::TABLE, Specs::strToTag( U"TaBlE" ) );
}

TEST( html5_Tests, strToTag_tbody ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TBODY, Specs::strToTag( U"tbody" ) );
    ASSERT_EQ( Tag::TBODY, Specs::strToTag( U"TBODY" ) );
    ASSERT_EQ( Tag::TBODY, Specs::strToTag( U"TbOdY" ) );
}

TEST( html5_Tests, strToTag_td ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TD, Specs::strToTag( U"td" ) );
    ASSERT_EQ( Tag::TD, Specs::strToTag( U"TD" ) );
    ASSERT_EQ( Tag::TD, Specs::strToTag( U"Td" ) );
}

TEST( html5_Tests, strToTag_template ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TEMPLATE, Specs::strToTag( U"template" ) );
    ASSERT_EQ( Tag::TEMPLATE, Specs::strToTag( U"TEMPLATE" ) );
    ASSERT_EQ( Tag::TEMPLATE, Specs::strToTag( U"TeMpLaTe" ) );
}

TEST( html5_Tests, strToTag_textarea ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TEXTAREA, Specs::strToTag( U"textarea" ) );
    ASSERT_EQ( Tag::TEXTAREA, Specs::strToTag( U"TEXTAREA" ) );
    ASSERT_EQ( Tag::TEXTAREA, Specs::strToTag( U"TEXTarea" ) );
}

TEST( html5_Tests, strToTag_tfoot ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TFOOT, Specs::strToTag( U"tfoot" ) );
    ASSERT_EQ( Tag::TFOOT, Specs::strToTag( U"TFOOT" ) );
    ASSERT_EQ( Tag::TFOOT, Specs::strToTag( U"TfOoT" ) );
}

TEST( html5_Tests, strToTag_th ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TH, Specs::strToTag( U"th" ) );
    ASSERT_EQ( Tag::TH, Specs::strToTag( U"TH" ) );
    ASSERT_EQ( Tag::TH, Specs::strToTag( U"Th" ) );
}

TEST( html5_Tests, strToTag_thead ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::THEAD, Specs::strToTag( U"thead" ) );
    ASSERT_EQ( Tag::THEAD, Specs::strToTag( U"THEAD" ) );
    ASSERT_EQ( Tag::THEAD, Specs::strToTag( U"tHeAd" ) );
}

TEST( html5_Tests, strToTag_time ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TIME, Specs::strToTag( U"time" ) );
    ASSERT_EQ( Tag::TIME, Specs::strToTag( U"TIME" ) );
    ASSERT_EQ( Tag::TIME, Specs::strToTag( U"TiMe" ) );
}

TEST( html5_Tests, strToTag_title ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TITLE, Specs::strToTag( U"title" ) );
    ASSERT_EQ( Tag::TITLE, Specs::strToTag( U"TITLE" ) );
    ASSERT_EQ( Tag::TITLE, Specs::strToTag( U"TiTlE" ) );
}

TEST( html5_Tests, strToTag_tr ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TR, Specs::strToTag( U"tr" ) );
    ASSERT_EQ( Tag::TR, Specs::strToTag( U"TR" ) );
    ASSERT_EQ( Tag::TR, Specs::strToTag( U"Tr" ) );
}

TEST( html5_Tests, strToTag_track ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::TRACK, Specs::strToTag( U"track" ) );
    ASSERT_EQ( Tag::TRACK, Specs::strToTag( U"TRACK" ) );
    ASSERT_EQ( Tag::TRACK, Specs::strToTag( U"TrAcK" ) );
}

TEST( html5_Tests, strToTag_u ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::U, Specs::strToTag( U"u" ) );
    ASSERT_EQ( Tag::U, Specs::strToTag( U"U" ) );
}

TEST( html5_Tests, strToTag_ul ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::UL, Specs::strToTag( U"ul" ) );
    ASSERT_EQ( Tag::UL, Specs::strToTag( U"UL" ) );
    ASSERT_EQ( Tag::UL, Specs::strToTag( U"Ul" ) );
}

TEST( html5_Tests, strToTag_var ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::VAR, Specs::strToTag( U"var" ) );
    ASSERT_EQ( Tag::VAR, Specs::strToTag( U"VAR" ) );
    ASSERT_EQ( Tag::VAR, Specs::strToTag( U"VaR" ) );
}

TEST( html5_Tests, strToTag_video ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::VIDEO, Specs::strToTag( U"video" ) );
    ASSERT_EQ( Tag::VIDEO, Specs::strToTag( U"VIDEO" ) );
    ASSERT_EQ( Tag::VIDEO, Specs::strToTag( U"ViDeO" ) );
}

TEST( html5_Tests, strToTag_wbr ) {
    using blogator::dom::html5::Tag;
    using blogator::dom::html5::Specs;

    ASSERT_EQ( Tag::WBR, Specs::strToTag( U"wbr" ) );
    ASSERT_EQ( Tag::WBR, Specs::strToTag( U"WBR" ) );
    ASSERT_EQ( Tag::WBR, Specs::strToTag( U"wBr" ) );
}