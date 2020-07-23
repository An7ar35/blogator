#include "attribute.h"

/**
 * Generates the Attribute->string lookup table
 * @return Lookup vector
 */
std::vector<std::u32string> blogator::dom::html5::properties::attribute::createAttributeStrTable() {
    auto v = std::vector<std::u32string>( static_cast<size_t>( Attribute::ENUM_END ) );

    v[ static_cast<size_t>( html5::Attribute::ACCEPT )              ] = U"accept";
    v[ static_cast<size_t>( html5::Attribute::ACCEPT_CHARSET )      ] = U"accept-charset";
    v[ static_cast<size_t>( html5::Attribute::ACCESSKEY )           ] = U"accesskey";
    v[ static_cast<size_t>( html5::Attribute::ACTION )              ] = U"action";
    v[ static_cast<size_t>( html5::Attribute::ALT )                 ] = U"alt";
    v[ static_cast<size_t>( html5::Attribute::ASYNC )               ] = U"async";
    v[ static_cast<size_t>( html5::Attribute::AUTOCOMPLETE )        ] = U"autocomplete";
    v[ static_cast<size_t>( html5::Attribute::AUTOFOCUS )           ] = U"autofocus";
    v[ static_cast<size_t>( html5::Attribute::AUTOPLAY )            ] = U"autoplay";
    v[ static_cast<size_t>( html5::Attribute::CHARSET )             ] = U"charset";
    v[ static_cast<size_t>( html5::Attribute::CHECKED )             ] = U"checked";
    v[ static_cast<size_t>( html5::Attribute::CITE )                ] = U"cite";
    v[ static_cast<size_t>( html5::Attribute::CLASS )               ] = U"class";
    v[ static_cast<size_t>( html5::Attribute::COLS )                ] = U"cols";
    v[ static_cast<size_t>( html5::Attribute::COLSPAN )             ] = U"colspan";
    v[ static_cast<size_t>( html5::Attribute::CONTENT )             ] = U"content";
    v[ static_cast<size_t>( html5::Attribute::CONTENTEDITABLE )     ] = U"contenteditable";
    v[ static_cast<size_t>( html5::Attribute::CONTROLS )            ] = U"controls";
    v[ static_cast<size_t>( html5::Attribute::COORDS )              ] = U"coords";
    v[ static_cast<size_t>( html5::Attribute::DATA )                ] = U"data";
    v[ static_cast<size_t>( html5::Attribute::DATA_X )              ] = U"data-"; //edge case 'data-*'
    v[ static_cast<size_t>( html5::Attribute::DATETIME )            ] = U"datetime";
    v[ static_cast<size_t>( html5::Attribute::DEFAULT )             ] = U"default";
    v[ static_cast<size_t>( html5::Attribute::DEFER )               ] = U"defer";
    v[ static_cast<size_t>( html5::Attribute::DIR )                 ] = U"dir";
    v[ static_cast<size_t>( html5::Attribute::DIRNAME )             ] = U"dirname";
    v[ static_cast<size_t>( html5::Attribute::DISABLED )            ] = U"disabled";
    v[ static_cast<size_t>( html5::Attribute::DOWNLOAD )            ] = U"download";
    v[ static_cast<size_t>( html5::Attribute::DRAGGABLE )           ] = U"draggable";
    v[ static_cast<size_t>( html5::Attribute::ENCTYPE )             ] = U"enctype";
    v[ static_cast<size_t>( html5::Attribute::FOR )                 ] = U"for";
    v[ static_cast<size_t>( html5::Attribute::FORM )                ] = U"form";
    v[ static_cast<size_t>( html5::Attribute::FORMACTION )          ] = U"formaction";
    v[ static_cast<size_t>( html5::Attribute::HEADERS )             ] = U"headers";
    v[ static_cast<size_t>( html5::Attribute::HEIGHT )              ] = U"height";
    v[ static_cast<size_t>( html5::Attribute::HIDDEN )              ] = U"hidden";
    v[ static_cast<size_t>( html5::Attribute::HIGH )                ] = U"high";
    v[ static_cast<size_t>( html5::Attribute::HREF )                ] = U"href";
    v[ static_cast<size_t>( html5::Attribute::HREFLANG )            ] = U"hreflang";
    v[ static_cast<size_t>( html5::Attribute::HTTP_EQUIV )          ] = U"http-equiv";
    v[ static_cast<size_t>( html5::Attribute::ID )                  ] = U"id";
    v[ static_cast<size_t>( html5::Attribute::ISMAP )               ] = U"ismap";
    v[ static_cast<size_t>( html5::Attribute::KIND )                ] = U"kind";
    v[ static_cast<size_t>( html5::Attribute::LABEL )               ] = U"label";
    v[ static_cast<size_t>( html5::Attribute::LANG )                ] = U"lang";
    v[ static_cast<size_t>( html5::Attribute::LIST )                ] = U"list";
    v[ static_cast<size_t>( html5::Attribute::LOOP )                ] = U"loop";
    v[ static_cast<size_t>( html5::Attribute::LOW )                 ] = U"low";
    v[ static_cast<size_t>( html5::Attribute::MAX )                 ] = U"max";
    v[ static_cast<size_t>( html5::Attribute::MAXLENGTH )           ] = U"maxlength";
    v[ static_cast<size_t>( html5::Attribute::MEDIA )               ] = U"media";
    v[ static_cast<size_t>( html5::Attribute::METHOD )              ] = U"method";
    v[ static_cast<size_t>( html5::Attribute::MIN )                 ] = U"min";
    v[ static_cast<size_t>( html5::Attribute::MULTIPLE )            ] = U"multiple";
    v[ static_cast<size_t>( html5::Attribute::MUTED )               ] = U"muted";
    v[ static_cast<size_t>( html5::Attribute::NAME )                ] = U"name";
    v[ static_cast<size_t>( html5::Attribute::NOVALIDATE )          ] = U"novalidate";
    v[ static_cast<size_t>( html5::Attribute::ONABORT )             ] = U"onabort";
    v[ static_cast<size_t>( html5::Attribute::ONAFTERPRINT )        ] = U"onafterprint";
    v[ static_cast<size_t>( html5::Attribute::ONBEFOREPRINT )       ] = U"onbeforeprint";
    v[ static_cast<size_t>( html5::Attribute::ONBEFOREUNLOAD )      ] = U"onbeforeunload";
    v[ static_cast<size_t>( html5::Attribute::ONBLUR )              ] = U"onblur";
    v[ static_cast<size_t>( html5::Attribute::ONCANPLAY )           ] = U"oncanplay";
    v[ static_cast<size_t>( html5::Attribute::ONCANPLAYTHROUGH )    ] = U"oncanplaythrough";
    v[ static_cast<size_t>( html5::Attribute::ONCHANGE )            ] = U"onchange";
    v[ static_cast<size_t>( html5::Attribute::ONCLICK )             ] = U"onclick";
    v[ static_cast<size_t>( html5::Attribute::ONCONTEXTMENU )       ] = U"oncontextmenu";
    v[ static_cast<size_t>( html5::Attribute::ONCOPY )              ] = U"oncopy";
    v[ static_cast<size_t>( html5::Attribute::ONCUECHANGE )         ] = U"oncuechange";
    v[ static_cast<size_t>( html5::Attribute::ONCUT )               ] = U"oncut";
    v[ static_cast<size_t>( html5::Attribute::ONDBLCLICK )          ] = U"ondblclick";
    v[ static_cast<size_t>( html5::Attribute::ONDRAG )              ] = U"ondrag";
    v[ static_cast<size_t>( html5::Attribute::ONDRAGEND )           ] = U"ondragend";
    v[ static_cast<size_t>( html5::Attribute::ONDRAGENTER )         ] = U"ondragcenter";
    v[ static_cast<size_t>( html5::Attribute::ONDRAGLEAVE )         ] = U"ondragleave";
    v[ static_cast<size_t>( html5::Attribute::ONDRAGOVER )          ] = U"ondragover";
    v[ static_cast<size_t>( html5::Attribute::ONDRAGSTART )         ] = U"ondragstart";
    v[ static_cast<size_t>( html5::Attribute::ONDROP )              ] = U"ondrop";
    v[ static_cast<size_t>( html5::Attribute::ONDURATIONCHANGE )    ] = U"ondurationchange";
    v[ static_cast<size_t>( html5::Attribute::ONEMPTIED )           ] = U"onemptied";
    v[ static_cast<size_t>( html5::Attribute::ONENDED )             ] = U"onended";
    v[ static_cast<size_t>( html5::Attribute::ONERROR )             ] = U"onerror";
    v[ static_cast<size_t>( html5::Attribute::ONFOCUS )             ] = U"onfocus";
    v[ static_cast<size_t>( html5::Attribute::ONHASHCHANGE )        ] = U"onhashchange";
    v[ static_cast<size_t>( html5::Attribute::ONINPUT )             ] = U"oninput";
    v[ static_cast<size_t>( html5::Attribute::ONINVALID )           ] = U"oninvalid";
    v[ static_cast<size_t>( html5::Attribute::ONKEYDOWN )           ] = U"onkeydown";
    v[ static_cast<size_t>( html5::Attribute::ONKEYPRESS )          ] = U"onkeypress";
    v[ static_cast<size_t>( html5::Attribute::ONKEYUP )             ] = U"onkeyup";
    v[ static_cast<size_t>( html5::Attribute::ONLOAD )              ] = U"onload";
    v[ static_cast<size_t>( html5::Attribute::ONLOADEDDATA )        ] = U"onloadeddata";
    v[ static_cast<size_t>( html5::Attribute::ONLOADEDMETADATA )    ] = U"onloadedmetadata";
    v[ static_cast<size_t>( html5::Attribute::ONLOADSTART )         ] = U"onloadstart";
    v[ static_cast<size_t>( html5::Attribute::ONMOUSEDOWN )         ] = U"onmousedown";
    v[ static_cast<size_t>( html5::Attribute::ONMOUSEMOVE )         ] = U"onmousemove";
    v[ static_cast<size_t>( html5::Attribute::ONMOUSEOUT )          ] = U"onmouseout";
    v[ static_cast<size_t>( html5::Attribute::ONMOUSEOVER )         ] = U"onmouseover";
    v[ static_cast<size_t>( html5::Attribute::ONMOUSEUP )           ] = U"onmouseup";
    v[ static_cast<size_t>( html5::Attribute::ONMOUSEWHEEL )        ] = U"onmousewheel";
    v[ static_cast<size_t>( html5::Attribute::ONOFFLINE )           ] = U"onoffline";
    v[ static_cast<size_t>( html5::Attribute::ONONLINE )            ] = U"ononline";
    v[ static_cast<size_t>( html5::Attribute::ONPAGEHIDE )          ] = U"onepagehide";
    v[ static_cast<size_t>( html5::Attribute::ONPAGESHOW )          ] = U"onpageshow";
    v[ static_cast<size_t>( html5::Attribute::ONPASTE )             ] = U"onpaste";
    v[ static_cast<size_t>( html5::Attribute::ONPAUSE )             ] = U"onpause";
    v[ static_cast<size_t>( html5::Attribute::ONPLAY )              ] = U"onplay";
    v[ static_cast<size_t>( html5::Attribute::ONPLAYING )           ] = U"onplaying";
    v[ static_cast<size_t>( html5::Attribute::ONPOPSTATE )          ] = U"onpopstate";
    v[ static_cast<size_t>( html5::Attribute::ONPROGRESS )          ] = U"onprogress";
    v[ static_cast<size_t>( html5::Attribute::ONRATECHANGE )        ] = U"onratechange";
    v[ static_cast<size_t>( html5::Attribute::ONRESET )             ] = U"onreset";
    v[ static_cast<size_t>( html5::Attribute::ONRESIZE )            ] = U"onresize";
    v[ static_cast<size_t>( html5::Attribute::ONSCROLL )            ] = U"onscroll";
    v[ static_cast<size_t>( html5::Attribute::ONSEARCH )            ] = U"onsearch";
    v[ static_cast<size_t>( html5::Attribute::ONSEEKED )            ] = U"onseeked";
    v[ static_cast<size_t>( html5::Attribute::ONSEEKING )           ] = U"onseeking";
    v[ static_cast<size_t>( html5::Attribute::ONSELECT )            ] = U"onselect";
    v[ static_cast<size_t>( html5::Attribute::ONSTALLED )           ] = U"onstalled";
    v[ static_cast<size_t>( html5::Attribute::ONSTORAGE )           ] = U"onstorage";
    v[ static_cast<size_t>( html5::Attribute::ONSUBMIT )            ] = U"onsubmit";
    v[ static_cast<size_t>( html5::Attribute::ONSUSPEND )           ] = U"onsuspend";
    v[ static_cast<size_t>( html5::Attribute::ONTIMEUPDATE )        ] = U"ontimeupdate";
    v[ static_cast<size_t>( html5::Attribute::ONTOGGLE )            ] = U"ontoggle";
    v[ static_cast<size_t>( html5::Attribute::ONUNLOAD )            ] = U"onunload";
    v[ static_cast<size_t>( html5::Attribute::ONVOLUMECHANGE )      ] = U"onvolumechange";
    v[ static_cast<size_t>( html5::Attribute::ONWAITING )           ] = U"onwaiting";
    v[ static_cast<size_t>( html5::Attribute::ONWHEEL )             ] = U"onwheel";
    v[ static_cast<size_t>( html5::Attribute::OPEN )                ] = U"open";
    v[ static_cast<size_t>( html5::Attribute::OPTIMUM )             ] = U"optimum";
    v[ static_cast<size_t>( html5::Attribute::PATTERN )             ] = U"pattern";
    v[ static_cast<size_t>( html5::Attribute::PLACEHOLDER )         ] = U"placeholder";
    v[ static_cast<size_t>( html5::Attribute::POSTER )              ] = U"poster";
    v[ static_cast<size_t>( html5::Attribute::PRELOAD )             ] = U"preload";
    v[ static_cast<size_t>( html5::Attribute::READONLY )            ] = U"readonly";
    v[ static_cast<size_t>( html5::Attribute::REL )                 ] = U"rel";
    v[ static_cast<size_t>( html5::Attribute::REQUIRED )            ] = U"required";
    v[ static_cast<size_t>( html5::Attribute::REVERSED )            ] = U"reversed";
    v[ static_cast<size_t>( html5::Attribute::ROWS )                ] = U"rows";
    v[ static_cast<size_t>( html5::Attribute::ROWSPAN )             ] = U"rowspan";
    v[ static_cast<size_t>( html5::Attribute::SANDBOX )             ] = U"sandbox";
    v[ static_cast<size_t>( html5::Attribute::SCOPE )               ] = U"scope";
    v[ static_cast<size_t>( html5::Attribute::SELECTED )            ] = U"selected";
    v[ static_cast<size_t>( html5::Attribute::SHAPE )               ] = U"shape";
    v[ static_cast<size_t>( html5::Attribute::SIZE )                ] = U"size";
    v[ static_cast<size_t>( html5::Attribute::SIZES )               ] = U"sizes";
    v[ static_cast<size_t>( html5::Attribute::SPAN )                ] = U"span";
    v[ static_cast<size_t>( html5::Attribute::SPELLCHECK )          ] = U"spellcheck";
    v[ static_cast<size_t>( html5::Attribute::SRC )                 ] = U"src";
    v[ static_cast<size_t>( html5::Attribute::SRCDOC )              ] = U"srcdoc";
    v[ static_cast<size_t>( html5::Attribute::SRCLANG )             ] = U"srclang";
    v[ static_cast<size_t>( html5::Attribute::SRCSET )              ] = U"srcset";
    v[ static_cast<size_t>( html5::Attribute::START )               ] = U"start";
    v[ static_cast<size_t>( html5::Attribute::STEP )                ] = U"step";
    v[ static_cast<size_t>( html5::Attribute::STYLE )               ] = U"style";
    v[ static_cast<size_t>( html5::Attribute::TABINDEX )            ] = U"tabindex";
    v[ static_cast<size_t>( html5::Attribute::TARGET )              ] = U"target";
    v[ static_cast<size_t>( html5::Attribute::TITLE )               ] = U"title";
    v[ static_cast<size_t>( html5::Attribute::TRANSLATE )           ] = U"translate";
    v[ static_cast<size_t>( html5::Attribute::TYPE )                ] = U"type";
    v[ static_cast<size_t>( html5::Attribute::USEMAP )              ] = U"usemap";
    v[ static_cast<size_t>( html5::Attribute::VALUE )               ] = U"value";
    v[ static_cast<size_t>( html5::Attribute::WIDTH )               ] = U"width";
    v[ static_cast<size_t>( html5::Attribute::WRAP )                ] = U"wrap";
    /* ARIA attributes */
    v[ static_cast<size_t>( html5::Attribute::ARIA_X )              ] = U"aria-"; //edge case 'aria-*'
    v[ static_cast<size_t>( html5::Attribute::ROLE )                ] = U"role";
    /* XML namespace attributes */
    v[ static_cast<size_t>( html5::Attribute::XMLNS )               ] = U"xmlns"; //edge case 'xmlns*'

    return v;
}

/**
 * Generates the string->Attribute lookup map from the Attribute->string map
 * @param str2attr Attribute->string map
 * @return Lookup map
 */
std::unordered_map<std::u32string, blogator::dom::html5::Attribute> blogator::dom::html5::properties::attribute::createStrAttributeMap(
    const std::vector<std::u32string> &attr2str )
{
    auto map = std::unordered_map<std::u32string, Attribute>();

    if( attr2str.size() != static_cast<size_t>( Attribute::ENUM_END ) ) {
        //TODO throw some shit
    }

    for( size_t i = 0; i < static_cast<size_t>( Attribute::ENUM_END ); ++i )
        map.emplace( attr2str.at( i ), static_cast<Attribute>( i ) );

    return map;
}