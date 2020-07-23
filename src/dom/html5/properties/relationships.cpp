#include "relationships.h"

#include "../enums/Attribute.h"
#include "../enums/ContentModel.h"
#include "../enums/Tag.h"

/**
 * Generates the Attribute/Tag membership lookup table
 * @return Lookup table
 */
std::vector<std::vector<bool>> blogator::dom::html5::properties::relationships::createAttr2TagRelationshipTable() {
    const auto attr_count = static_cast<size_t>( Attribute::ENUM_END );
    const auto tag_count  = static_cast<size_t>( Tag::ENUM_END );

    auto v = std::vector<std::vector<bool>>( attr_count, std::vector<bool>( tag_count, false ) );

    //Global attributes - can be used with all HTML elements
    for( size_t i = 0; i < tag_count; ++i ) {
        v[ static_cast<int>( Attribute::ACCESSKEY )       ][ i ] = true;
        v[ static_cast<int>( Attribute::CLASS )           ][ i ] = true;
        v[ static_cast<int>( Attribute::CONTENTEDITABLE ) ][ i ] = true;
        v[ static_cast<int>( Attribute::DATA_X )          ][ i ] = true;
        v[ static_cast<int>( Attribute::DIR )             ][ i ] = true;
        v[ static_cast<int>( Attribute::DRAGGABLE )       ][ i ] = true;
        v[ static_cast<int>( Attribute::HIDDEN )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ID )              ][ i ] = true;
        v[ static_cast<int>( Attribute::LANG )            ][ i ] = true;
        v[ static_cast<int>( Attribute::SPELLCHECK )      ][ i ] = true;
        v[ static_cast<int>( Attribute::STYLE )           ][ i ] = true;
        v[ static_cast<int>( Attribute::TABINDEX )        ][ i ] = true;
        v[ static_cast<int>( Attribute::TITLE )           ][ i ] = true;
        v[ static_cast<int>( Attribute::TRANSLATE )       ][ i ] = true;
    }

    //Visible elements - can be used on any HTML element that is visible
    for( size_t i = 0; i < tag_count; ++i ) {
        v[ static_cast<int>( Attribute::ONBLUR )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCHANGE )        ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCLICK )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCONTEXTMENU )   ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCOPY )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONCUT )           ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDBLCLICK )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAG )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGEND )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGENTER )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGLEAVE )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGOVER )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDRAGSTART )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONDROP )          ][ i ] = true;
        v[ static_cast<int>( Attribute::ONFOCUS )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONINPUT )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONINVALID )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONKEYDOWN )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONKEYPRESS )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONKEYUP )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEDOWN )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEMOVE )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEOUT )      ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEOVER )     ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEUP )       ][ i ] = true;
        v[ static_cast<int>( Attribute::ONMOUSEWHEEL )    ][ i ] = true;
        v[ static_cast<int>( Attribute::ONPASTE )         ][ i ] = true;
        v[ static_cast<int>( Attribute::ONSCROLL )        ][ i ] = true;
        v[ static_cast<int>( Attribute::ONSELECT )        ][ i ] = true;
        v[ static_cast<int>( Attribute::ONWHEEL )         ][ i ] = true;
    }

    //Local attributes - can be used with only some HTML elements
    v[ static_cast<int>( Attribute::ACCEPT )            ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ACCEPT_CHARSET )    ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ACTION )            ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ALT )               ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::ALT )               ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ALT )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ASYNC )             ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::AUTOCOMPLETE )      ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::AUTOCOMPLETE )      ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::AUTOFOCUS )         ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::AUTOPLAY )          ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::AUTOPLAY )          ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::CHARSET )           ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::CHARSET )           ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::CHECKED )           ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::BLOCKQUOTE )  ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::DEL )         ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::INS )         ] = true;
    v[ static_cast<int>( Attribute::CITE )              ][ static_cast<int>( Tag::Q )           ] = true;
    v[ static_cast<int>( Attribute::COLS )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::COLSPAN )           ][ static_cast<int>( Tag::TD )          ] = true;
    v[ static_cast<int>( Attribute::COLSPAN )           ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::CONTENT )           ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::CONTROLS )          ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::CONTROLS )          ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::COORDS )            ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::DATA )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::DATETIME )          ][ static_cast<int>( Tag::DEL )         ] = true;
    v[ static_cast<int>( Attribute::DATETIME )          ][ static_cast<int>( Tag::INS )         ] = true;
    v[ static_cast<int>( Attribute::DATETIME )          ][ static_cast<int>( Tag::TIME )        ] = true;
    v[ static_cast<int>( Attribute::DEFAULT )           ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::DEFER )             ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::DIRNAME )           ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::DIRNAME )           ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::FIELDSET )    ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::OPTGROUP )    ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::DISABLED )          ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::DOWNLOAD )          ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::DOWNLOAD )          ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::ENCTYPE )           ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::FOR )               ][ static_cast<int>( Tag::LABEL )       ] = true;
    v[ static_cast<int>( Attribute::FOR )               ][ static_cast<int>( Tag::OUTPUT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::FIELDSET )    ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::LABEL )       ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::OUTPUT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::FORM )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::FORMACTION )        ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::FORMACTION )        ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::HEADERS )           ][ static_cast<int>( Tag::TD )          ] = true;
    v[ static_cast<int>( Attribute::HEADERS )           ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::CANVAS )      ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::HEIGHT )            ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::HIGH )              ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::BASE )        ] = true;
    v[ static_cast<int>( Attribute::HREF )              ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::HREFLANG )          ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::HREFLANG )          ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::HREFLANG )          ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::HTTP_EQUIV )        ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::ISMAP )             ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::KIND )              ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::LABEL )             ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::LABEL )             ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::LABEL )             ][ static_cast<int>( Tag::OPTGROUP )    ] = true;
    v[ static_cast<int>( Attribute::LIST )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::LOOP )              ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::LOOP )              ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::LOW )               ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::MAX )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MAX )               ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::MAX )               ][ static_cast<int>( Tag::PROGRESS )    ] = true;
    v[ static_cast<int>( Attribute::MAXLENGTH )         ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MAXLENGTH )         ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::MEDIA )             ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::METHOD )            ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::MIN )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MIN )               ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::MULTIPLE )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::MULTIPLE )          ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::MUTED )             ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::MUTED )             ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::FIELDSET )    ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::MAP )         ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::META )        ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::OUTPUT )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::PARAM )       ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::NAME )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::NOVALIDATE )        ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::ONABORT )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONAFTERPRINT )      ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONBEFOREPRINT )     ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONBEFOREUNLOAD )    ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAY )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAYTHROUGH )  ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONCANPLAYTHROUGH )  ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONCUECHANGE )       ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::ONDURATIONCHANGE )  ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONDURATIONCHANGE )  ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONEMPTIED )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONEMPTIED )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONENDED )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONENDED )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::ONERROR )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONHASHCHANGE )      ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::ONLOAD )            ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDDATA )      ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDDATA )      ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDMETADATA )  ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADEDMETADATA )  ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADSTART )       ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONLOADSTART )       ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONOFFLINE )         ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONONLINE )          ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPAGEHIDE )        ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPAGESHOW )        ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPAUSE )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPAUSE )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAY )            ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAY )            ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAYING )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPLAYING )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONPOPSTATE )        ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONPROGRESS )        ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONPROGRESS )        ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONRATECHANGE )      ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONRATECHANGE )      ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONRESET )           ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ONRESIZE )          ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONSEARCH )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKED )          ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKED )          ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKING )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSEEKING )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONSTALLED )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSTALLED )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONSTORAGE )         ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONSUBMIT )          ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::ONSUSPEND )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONSUSPEND )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONTIMEUPDATE )      ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONTIMEUPDATE )      ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONTOGGLE )          ][ static_cast<int>( Tag::DETAILS )     ] = true;
    v[ static_cast<int>( Attribute::ONUNLOAD )          ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ONVOLUMECHANGE )    ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONVOLUMECHANGE )    ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::ONWAITING )         ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::ONWAITING )         ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::OPEN )              ][ static_cast<int>( Tag::DETAILS )     ] = true;
    v[ static_cast<int>( Attribute::OPTIMUM )           ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::PATTERN )           ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::PLACEHOLDER )       ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::PLACEHOLDER )       ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::POSTER )            ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::PRELOAD )           ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::PRELOAD )           ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::READONLY )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::READONLY )          ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::REL )               ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::REQUIRED )          ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::REQUIRED )          ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::REQUIRED )          ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::REVERSED )          ][ static_cast<int>( Tag::OL )          ] = true;
    v[ static_cast<int>( Attribute::ROWS )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;
    v[ static_cast<int>( Attribute::ROWSPAN )           ][ static_cast<int>( Tag::TD )          ] = true;
    v[ static_cast<int>( Attribute::ROWSPAN )           ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::SANDBOX )           ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::SCOPE )             ][ static_cast<int>( Tag::TH )          ] = true;
    v[ static_cast<int>( Attribute::SELECTED )          ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::SHAPE )             ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::SIZE )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::SIZE )              ][ static_cast<int>( Tag::SELECT )      ] = true;
    v[ static_cast<int>( Attribute::SIZES )             ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::SIZES )             ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::SIZES )             ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::SPAN )              ][ static_cast<int>( Tag::COL )         ] = true;
    v[ static_cast<int>( Attribute::SPAN )              ][ static_cast<int>( Tag::COLGROUP )    ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::AUDIO )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::SRC )               ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::SRCDOC )            ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::SRCLANG )           ][ static_cast<int>( Tag::TRACK )       ] = true;
    v[ static_cast<int>( Attribute::SRCSET )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::SRCSET )            ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::START )             ][ static_cast<int>( Tag::OL )          ] = true;
    v[ static_cast<int>( Attribute::STEP )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::AREA )        ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::BASE )        ] = true;
    v[ static_cast<int>( Attribute::TARGET )            ][ static_cast<int>( Tag::FORM )        ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::A )           ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::SCRIPT )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::SOURCE )      ] = true;
    v[ static_cast<int>( Attribute::TYPE )              ][ static_cast<int>( Tag::STYLE )       ] = true;
    v[ static_cast<int>( Attribute::USEMAP )            ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::USEMAP )            ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::BUTTON )      ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::LI )          ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::OPTION )      ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::METER )       ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::PROGRESS )    ] = true;
    v[ static_cast<int>( Attribute::VALUE )             ][ static_cast<int>( Tag::PARAM )       ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::CANVAS )      ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::EMBED )       ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::IFRAME )      ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::IMG )         ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::INPUT )       ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::OBJECT )      ] = true;
    v[ static_cast<int>( Attribute::WIDTH )             ][ static_cast<int>( Tag::VIDEO )       ] = true;
    v[ static_cast<int>( Attribute::WRAP )              ][ static_cast<int>( Tag::TEXTAREA )    ] = true;

    //ARIA attributes
    std::vector<int> aria_x_tag_context = { //TODO keep and use?
        static_cast<int>( Tag::A ),
        static_cast<int>( Tag::ABBR ),
        static_cast<int>( Tag::ADDRESS ),
        static_cast<int>( Tag::AREA ),
        static_cast<int>( Tag::ARTICLE ),
        static_cast<int>( Tag::ASIDE ),
        static_cast<int>( Tag::AUDIO ),
        static_cast<int>( Tag::B ),
        static_cast<int>( Tag::BASE ),
        static_cast<int>( Tag::BDI ),
        static_cast<int>( Tag::BDO ),
        static_cast<int>( Tag::BLOCKQUOTE ),
        static_cast<int>( Tag::BODY ),
        static_cast<int>( Tag::BR ),
        static_cast<int>( Tag::BUTTON ),
        static_cast<int>( Tag::CANVAS ),
        static_cast<int>( Tag::CAPTION ),
        static_cast<int>( Tag::CITE ),
        static_cast<int>( Tag::CODE ),
        static_cast<int>( Tag::COL ),
        static_cast<int>( Tag::COLGROUP ),
        static_cast<int>( Tag::COMMENT ),
        static_cast<int>( Tag::DATA ),
        static_cast<int>( Tag::DATALIST ),
        static_cast<int>( Tag::DD ),
        static_cast<int>( Tag::DEL ),
        static_cast<int>( Tag::DETAILS ),
        static_cast<int>( Tag::DFN ),
        static_cast<int>( Tag::DIALOG ),
        static_cast<int>( Tag::DIV ),
        static_cast<int>( Tag::DL ),
        static_cast<int>( Tag::DOCTYPE ),
        static_cast<int>( Tag::DT ),
        static_cast<int>( Tag::EM ),
        static_cast<int>( Tag::EMBED ),
        static_cast<int>( Tag::FIELDSET ),
        static_cast<int>( Tag::FIGCAPTION ),
        static_cast<int>( Tag::FIGURE ),
        static_cast<int>( Tag::FOOTER ),
        static_cast<int>( Tag::FORM ),
        static_cast<int>( Tag::H1 ),
        static_cast<int>( Tag::H2 ),
        static_cast<int>( Tag::H3 ),
        static_cast<int>( Tag::H4 ),
        static_cast<int>( Tag::H5 ),
        static_cast<int>( Tag::H6 ),
        static_cast<int>( Tag::HEAD ),
        static_cast<int>( Tag::HEADER ),
        static_cast<int>( Tag::HR ),
        static_cast<int>( Tag::HTML ),
        static_cast<int>( Tag::I ),
        static_cast<int>( Tag::IFRAME ),
        static_cast<int>( Tag::IMG ),
        static_cast<int>( Tag::INPUT ),
        static_cast<int>( Tag::INS ),
        static_cast<int>( Tag::KBD ),
        static_cast<int>( Tag::LABEL ),
        static_cast<int>( Tag::LEGEND ),
        static_cast<int>( Tag::LI ),
        static_cast<int>( Tag::LINK ),
        static_cast<int>( Tag::MAIN ),
        static_cast<int>( Tag::MAP ),
        static_cast<int>( Tag::MARK ),
        static_cast<int>( Tag::META ),
        static_cast<int>( Tag::METER ),
        static_cast<int>( Tag::NAV ),
        static_cast<int>( Tag::NOSCRIPT ),
        static_cast<int>( Tag::OBJECT ),
        static_cast<int>( Tag::OL ),
        static_cast<int>( Tag::OPTGROUP ),
        static_cast<int>( Tag::OPTION ),
        static_cast<int>( Tag::OUTPUT ),
        static_cast<int>( Tag::P ),
        static_cast<int>( Tag::PARAM ),
        static_cast<int>( Tag::PICTURE ),
        static_cast<int>( Tag::PRE ),
        static_cast<int>( Tag::PROGRESS ),
        static_cast<int>( Tag::Q ),
        static_cast<int>( Tag::RP ),
        static_cast<int>( Tag::RT ),
        static_cast<int>( Tag::RUBY ),
        static_cast<int>( Tag::S ),
        static_cast<int>( Tag::SAMP ),
        static_cast<int>( Tag::SCRIPT ),
        static_cast<int>( Tag::SECTION ),
        static_cast<int>( Tag::SELECT ),
        static_cast<int>( Tag::SMALL ),
        static_cast<int>( Tag::SOURCE ),
        static_cast<int>( Tag::SPAN ),
        static_cast<int>( Tag::STRONG ),
        static_cast<int>( Tag::STYLE ),
        static_cast<int>( Tag::SUB ),
        static_cast<int>( Tag::SUMMARY ),
        static_cast<int>( Tag::SUP ),
        static_cast<int>( Tag::TABLE ),
        static_cast<int>( Tag::TBODY ),
        static_cast<int>( Tag::TD ),
        static_cast<int>( Tag::TEMPLATE ),
        static_cast<int>( Tag::TEXTAREA ),
        static_cast<int>( Tag::TFOOT ),
        static_cast<int>( Tag::TH ),
        static_cast<int>( Tag::THEAD ),
        static_cast<int>( Tag::TIME ),
        static_cast<int>( Tag::TITLE ),
        static_cast<int>( Tag::TR ),
        static_cast<int>( Tag::TRACK ),
        static_cast<int>( Tag::U ),
        static_cast<int>( Tag::UL ),
        static_cast<int>( Tag::VAR ),
        static_cast<int>( Tag::VIDEO ),
        static_cast<int>( Tag::WBR ),
    };

    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::SECTION )     ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::ARTICLE )     ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::NAV )         ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::ASIDE )       ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::H1 )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::H2 )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::H3 )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::H4 )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::H5 )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::H6 )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::HEADER )      ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::FOOTER )      ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::P )           ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::ADDRESS )     ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::HR )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::PRE )         ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::BLOCKQUOTE )  ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::OL )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::UL )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::LI )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::DL )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::DT )          ] = true;
    v[ static_cast<int>( Attribute::ARIA_X )            ][ static_cast<int>( Tag::DD )          ] = true;

    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::SECTION )     ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::LINK )        ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::BODY )        ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::ARTICLE )     ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::NAV )         ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::ASIDE )       ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::H1 )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::H2 )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::H3 )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::H4 )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::H5 )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::H6 )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::HEADER )      ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::FOOTER )      ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::P )           ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::ADDRESS )     ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::HR )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::PRE )         ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::BLOCKQUOTE )  ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::OL )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::UL )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::LI )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::DL )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::DT )          ] = true;
    v[ static_cast<int>( Attribute::ROLE )              ][ static_cast<int>( Tag::DD )          ] = true;
    //TODO per tag whitelist -> see standard

    return v;
}

/**
 * Generates the Tag/Content Model membership lookup table
 * @return Lookup table
 */
std::vector<std::vector<bool>> blogator::dom::html5::properties::relationships::createTag2ContentModelRelationshipTable() {
    const auto tag_count = static_cast<size_t>( Tag::ENUM_END );
    const auto cm_count  = static_cast<size_t>( ContentModel::ENUM_END );

    auto v = std::vector<std::vector<bool>>( tag_count, std::vector<bool>( cm_count, false ) );

    v[ static_cast<int>( Tag::A )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::A )           ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::A )           ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::A )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::ABBR )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::ABBR )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::ABBR )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::ADDRESS )     ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::ADDRESS )     ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::AREA )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::AREA )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::ARTICLE )     ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::ARTICLE )     ][ static_cast<int>( ContentModel::SECTIONING )     ] = true;
    v[ static_cast<int>( Tag::ARTICLE )     ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::ASIDE )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::ASIDE )       ][ static_cast<int>( ContentModel::SECTIONING )     ] = true;
    v[ static_cast<int>( Tag::ASIDE )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::AUDIO )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::AUDIO )       ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::AUDIO )       ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::AUDIO )       ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::AUDIO )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::B )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::B )           ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::B )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::BASE )        ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::BDI )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::BDI )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::BDI )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::BDO )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::BDO )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::BDO )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::BLOCKQUOTE )  ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::BLOCKQUOTE )  ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::BODY )        ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::BR )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::BR )          ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::BUTTON )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::BUTTON )      ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::BUTTON )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::BUTTON )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::CANVAS )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::CANVAS )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::CANVAS )      ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::CANVAS )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::CAPTION )     ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::CITE )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::CITE )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::CITE )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::CODE )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::CODE )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::CODE )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::COL )         ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::COLGROUP )    ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::COMMENT )     ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::DATA )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DATA )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::DATA )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::DATALIST )    ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DATALIST )    ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
//    v[ static_cast<int>( Tag::DD )          ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::DEL )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DEL )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::DETAILS )     ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DETAILS )     ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::DETAILS )     ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::DFN )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DFN )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::DFN )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::DIALOG )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DIV )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DIV )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::DL )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::DL )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::DOCTYPE )     ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::DT )          ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::EM )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::EM )          ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::EM )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::EMBED )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::EMBED )       ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::EMBED )       ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::EMBED )       ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::EMBED )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::FIELDSET )    ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::FIELDSET )    ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::FIGCAPTION )  ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::FIGURE )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::FIGURE )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::FOOTER )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::FOOTER )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::FORM )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::FORM )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::H1 )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::H1 )          ][ static_cast<int>( ContentModel::HEADING )        ] = true;
    v[ static_cast<int>( Tag::H1 )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::H2 )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::H2 )          ][ static_cast<int>( ContentModel::HEADING )        ] = true;
    v[ static_cast<int>( Tag::H2 )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::H3 )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::H3 )          ][ static_cast<int>( ContentModel::HEADING )        ] = true;
    v[ static_cast<int>( Tag::H3 )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::H4 )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::H4 )          ][ static_cast<int>( ContentModel::HEADING )        ] = true;
    v[ static_cast<int>( Tag::H4 )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::H5 )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::H5 )          ][ static_cast<int>( ContentModel::HEADING )        ] = true;
    v[ static_cast<int>( Tag::H5 )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::H6 )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::H6 )          ][ static_cast<int>( ContentModel::HEADING )        ] = true;
    v[ static_cast<int>( Tag::H6 )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::HEAD )        ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::HEADER )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::HEADER )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::HR )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
//    v[ static_cast<int>( Tag::HTML )        ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::I )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::I )           ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::I )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::IFRAME )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::IFRAME )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::IFRAME )      ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::IFRAME )      ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::IFRAME )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::IMG )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::IMG )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::IMG )         ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::IMG )         ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::IMG )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::INPUT )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::INPUT )       ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::INPUT )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::INS )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::INS )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::INS )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::KBD )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::KBD )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::KBD )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::LABEL )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::LABEL )       ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::LABEL )       ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::LABEL )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::LEGEND )      ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::LI )          ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::LINK )        ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::LINK )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::LINK )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::MAIN )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::MAIN )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::MAP )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::MAP )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::MAP )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::MARK )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::MARK )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::MARK )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::META )        ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::METER )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::METER )       ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::METER )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::NAV )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::NAV )         ][ static_cast<int>( ContentModel::SECTIONING )     ] = true;
    v[ static_cast<int>( Tag::NAV )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::NOSCRIPT )    ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::NOSCRIPT )    ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::NOSCRIPT )    ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::OBJECT )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::OBJECT )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::OBJECT )      ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::OBJECT )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::OL )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::OL )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::OPTGROUP )    ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::OPTION )      ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::OUTPUT )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::OUTPUT )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::OUTPUT )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::P )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::P )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::PARAM )       ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::PICTURE )     ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::PICTURE )     ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::PICTURE )     ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::PRE )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::PRE )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::PROGRESS )    ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::PROGRESS )    ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::PROGRESS )    ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::Q )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::Q )           ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::Q )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::RP )          ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::RT )          ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::RUBY )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::RUBY )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::RUBY )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::S )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::S )           ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::S )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::SAMP )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SAMP )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SAMP )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::SCRIPT )      ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::SCRIPT )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SCRIPT )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SCRIPT )      ][ static_cast<int>( ContentModel::SCRIPT_SUPPORT ) ] = true;
    v[ static_cast<int>( Tag::SECTION )     ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SECTION )     ][ static_cast<int>( ContentModel::SECTIONING )     ] = true;
    v[ static_cast<int>( Tag::SECTION )     ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::SELECT )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SELECT )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SELECT )      ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::SELECT )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::SMALL )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SMALL )       ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SMALL )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::SOURCE )      ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::SPAN )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SPAN )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SPAN )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::STRONG )      ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::STRONG )      ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::STRONG )      ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::STYLE )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::STYLE )       ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::SUB )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SUB )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SUB )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::SUMMARY )     ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::SUP )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SUP )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SUP )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::TABLE )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::TABLE )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::TBODY )       ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::TD )          ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::TEMPLATE )    ][ static_cast<int>( ContentModel::METADATA )       ] = true;
    v[ static_cast<int>( Tag::TEMPLATE )    ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::TEMPLATE )    ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::TEMPLATE )    ][ static_cast<int>( ContentModel::SCRIPT_SUPPORT ) ] = true;
    v[ static_cast<int>( Tag::TEXTAREA )    ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::TEXTAREA )    ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::TEXTAREA )    ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::TEXTAREA )    ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
//    v[ static_cast<int>( Tag::TFOOT )       ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::TH )          ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::THEAD )       ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::TIME )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::TIME )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::TIME )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::TITLE )       ][ static_cast<int>( ContentModel::METADATA )       ] = true;
//    v[ static_cast<int>( Tag::TR )          ][ static_cast<int>( ContentModel:: ) ] = true;
//    v[ static_cast<int>( Tag::TRACK )       ][ static_cast<int>( ContentModel:: ) ] = true;
    v[ static_cast<int>( Tag::U )           ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::U )           ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::U )           ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::UL )          ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::UL )          ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::VAR )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::VAR )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::VAR )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::VIDEO )       ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::VIDEO )       ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::VIDEO )       ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::VIDEO )       ][ static_cast<int>( ContentModel::INTERACTIVE )    ] = true;
    v[ static_cast<int>( Tag::VIDEO )       ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::WBR )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::WBR )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;

    v[ static_cast<int>( Tag::MATH )        ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::MATH )        ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::MATH )        ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::MATH )        ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;
    v[ static_cast<int>( Tag::SVG )         ][ static_cast<int>( ContentModel::FLOW )           ] = true;
    v[ static_cast<int>( Tag::SVG )         ][ static_cast<int>( ContentModel::PHRASING )       ] = true;
    v[ static_cast<int>( Tag::SVG )         ][ static_cast<int>( ContentModel::EMBEDDED )       ] = true;
    v[ static_cast<int>( Tag::SVG )         ][ static_cast<int>( ContentModel::PALPABLE )       ] = true;

    return v;
}

///**
// * Generates the Tag usage context lookup table
// * @return Lookup table
// */
//std::vector<std::vector<bool>> blogator::dom::html5::rules::factory::Relationships::createTagContextRelationshipTable() { //TODO
//    const auto tag_count  = static_cast<int>( Tag::ENUM_END ) + 1;
//
//    auto v = std::vector<std::vector<bool>>( tag_count, std::vector<bool>( tag_count, false ) );
//
//    v[ static_cast<int>( Tag::A )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::ABBR )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::ADDRESS )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::AREA )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::ARTICLE )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::ASIDE )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::AUDIO )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::B )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::BASE )        ][ static_cast<int>( Tag::HEAD ) ] = true;
//    v[ static_cast<int>( Tag::BDI )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::BDO )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::BLOCKQUOTE )  ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::BODY )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::BR )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::BUTTON )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::CANVAS )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::CAPTION )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::CITE )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::CODE )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::COL )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::COLGROUP )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::COMMENT )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DATA )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DATALIST )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DD )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DEL )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DETAILS )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DFN )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DIALOG )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DIV )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DL )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DOCTYPE )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::DT )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::EM )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::EMBED )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::FIELDSET )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::FIGCAPTION )  ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::FIGURE )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::FOOTER )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::FORM )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::H1 )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::H2 )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::H3 )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::H4 )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::H5 )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::H6 )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::HEAD )        ][ static_cast<int>( Tag::HTML ) ] = true;
//    v[ static_cast<int>( Tag::HEADER )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::HR )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::HTML )        ][ static_cast<int>( Tag::NONE ) ] = true;
//    v[ static_cast<int>( Tag::I )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::IFRAME )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::IMG )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::INPUT )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::INS )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::KBD )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::LABEL )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::LEGEND )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::LI )          ][ static_cast<int>( Tag::OL ) ] = true;
//    v[ static_cast<int>( Tag::LI )          ][ static_cast<int>( Tag::UL ) ] = true;
//    v[ static_cast<int>( Tag::LINK )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::MAIN )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::MAP )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::MARK )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::META )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::METER )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::NAV )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::NOSCRIPT )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::OBJECT )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::OL )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::OPTGROUP )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::OPTION )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::OUTPUT )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::P )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::PARAM )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::PICTURE )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::PRE )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::PROGRESS )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::Q )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::RP )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::RT )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::RUBY )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::S )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SAMP )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SCRIPT )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SECTION )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SELECT )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SMALL )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SOURCE )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SPAN )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::STRONG )      ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::STYLE )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SUB )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SUMMARY )     ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::SUP )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TABLE )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TBODY )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TD )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TEMPLATE )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TEXTAREA )    ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TFOOT )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TH )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::THEAD )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TIME )        ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TITLE )       ][ static_cast<int>( Tag::HEAD ) ] = true;
//    v[ static_cast<int>( Tag::TR )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::TRACK )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::U )           ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::UL )          ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::VAR )         ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::VIDEO )       ][ static_cast<int>( Tag:: ) ] = true;
//    v[ static_cast<int>( Tag::WBR )         ][ static_cast<int>( Tag:: ) ] = true;
//
//    return v;
//}