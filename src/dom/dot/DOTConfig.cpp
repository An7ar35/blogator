#include "DOTConfig.h"

#include <sstream>

#include "../../exception/not_implemented.h"

/**
 * Constructor
 * @param list List of DOTSetting(s)
 */
blogator::dom::DOTConfig::DOTConfig( std::initializer_list<DOTSetting> list ) {
    for( auto e : list )
        setItem( e );
}

/**
 * Sets a given setting flag to true
 * @param setting DOTSetting
 */
void blogator::dom::DOTConfig::setItem( blogator::dom::DOTSetting setting ) {
    switch( setting ) {
        case DOTSetting::HTML5_NOCHECK:
            html5.attribute_compliance = ComplianceLevel::NONE;
            html5.hierarchy_compliance = ComplianceLevel::NONE;
            break;

        case DOTSetting::HTML5_PARTIAL:
            html5.attribute_compliance = ComplianceLevel::PARTIAL;
            html5.hierarchy_compliance = ComplianceLevel::PARTIAL;
            break;

        case DOTSetting::HTML5_STRICT:
            html5.attribute_compliance = ComplianceLevel::STRICT;
            html5.hierarchy_compliance = ComplianceLevel::STRICT;
            break;

        default:
            std::stringstream ss;
            ss << "[blogator::dom::DOTConfig::setItem( " << static_cast<int>( setting ) << " )] Enum not implemented in switch().";
            throw blogator::not_implemented( ss.str() );
    }
}