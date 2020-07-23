#include "content_model.h"

#include "../enums/ContentModel.h"

/**
 * Generates the content model enum->string conversion table
 * @return Content model enum2str lookup table
 */
std::vector<std::u32string> blogator::dom::html5::properties::content_model::createContentModelStrTable() {
    auto v = std::vector<std::u32string>( static_cast<size_t>( ContentModel::ENUM_END ) );

    v[ static_cast<size_t>( ContentModel::NOTHING )        ] = U"nothing";
    v[ static_cast<size_t>( ContentModel::METADATA )       ] = U"metadata";
    v[ static_cast<size_t>( ContentModel::FLOW )           ] = U"flow";
    v[ static_cast<size_t>( ContentModel::SECTIONING )     ] = U"sectioning";
    v[ static_cast<size_t>( ContentModel::HEADING )        ] = U"heading";
    v[ static_cast<size_t>( ContentModel::PHRASING )       ] = U"phrasing";
    v[ static_cast<size_t>( ContentModel::EMBEDDED )       ] = U"embedded";
    v[ static_cast<size_t>( ContentModel::INTERACTIVE )    ] = U"interactive";
    v[ static_cast<size_t>( ContentModel::PALPABLE )       ] = U"palpable";
    v[ static_cast<size_t>( ContentModel::SCRIPT_SUPPORT ) ] = U"script supporting";

    return v;
}