#ifndef BLOGATOR_LOGGER_FORMATTER_DEFAULTPRINTCOLOUR_H
#define BLOGATOR_LOGGER_FORMATTER_DEFAULTPRINTCOLOUR_H

#include "DefaultPrintMono.h"
#include "../global/LogMsg.h"

namespace blogator::logger::formatter {
    class DefaultPrintColour : public DefaultPrintMono {
      public:
        DefaultPrintColour();

        std::string format( const LogMsg &obj ) override;

      protected:
        void formatLogLevel( std::ostream &os, const LogMsg &obj ) override;

      private:
        static const char * RESET;
        static const char * BLACK;
        static const char * RED;
        static const char * GREEN;
        static const char * YELLOW;
        static const char * BLUE;
        static const char * BROWN;
        static const char * PURPLE;
        static const char * CYAN;
        static const char * GREY;
        static const char * WHITE;
        static const char * BOLD;
    };
}

#endif //BLOGATOR_LOGGER_FORMATTER_DEFAULTPRINTCOLOUR_H