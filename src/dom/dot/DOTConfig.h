#ifndef BLOGATOR_DOM_DOT_DOTCONFIG_H
#define BLOGATOR_DOM_DOT_DOTCONFIG_H

#include <type_traits>
#include <initializer_list>

#include "enums/ComplianceLevel.h"
#include "enums/DOTSetting.h"

namespace blogator::dom {
    class DOTConfig {
      public:
        DOTConfig() = default;
        template<typename ...Args> explicit DOTConfig( Args ...args );
        DOTConfig( std::initializer_list<DOTSetting> list );

        struct {
            ComplianceLevel attribute_compliance { ComplianceLevel::NONE };
            ComplianceLevel hierarchy_compliance { ComplianceLevel::NONE };

        } html5;

      private:
        template<typename H, typename ...T> void processArgs_( H head, T ...tail );
        void                                     processArgs_() {}; //recurse stopper
        void                                     setItem( DOTSetting setting );
    };
}

/**
 * Constructor
 * @tparam Args DOTConfig enum
 * @param args  Configuration arguments
 */
template<typename ...Args> blogator::dom::DOTConfig::DOTConfig( Args... args ) {
    processArgs_( args... );
}

/**
 * Process configuration arguments
 * @tparam H    Head type
 * @tparam Args Remaining type
 * @param head  DOTConfig argument to process
 * @param tail  Remaining argument to process
 */
template<typename H, typename ...T> void blogator::dom::DOTConfig::processArgs_( H head, T... tail ) {
    static_assert( std::is_same_v<H, DOTSetting>, "Only dom::DOTSettings enums can be passed as args." );
    setItem( static_cast<DOTSetting>( head ) );
    processArgs_( tail... );
}

#endif //BLOGATOR_DOM_DOT_DOTCONFIG_H
