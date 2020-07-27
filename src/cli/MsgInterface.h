#ifndef BLOGATOR_CLI_MSGINTERFACE_H
#define BLOGATOR_CLI_MSGINTERFACE_H

#include <iostream>
#include <list>
#include <mutex>

#include "MsgDisplay.h"

namespace blogator::cli {
    class MsgInterface {
      public:
        static MsgInterface & getInstance();
        virtual ~MsgInterface() = default;

        void begin( const std::string &process_name, const std::string &next_step );
        void begin( const std::string &process_name, const unsigned &steps, const std::string &next_step );
        void addSteps( const unsigned &steps );
        void end( const std::string &msg );

        double total() const;
        bool   isActive() const;

        void progress();
        void progress( const std::string &next_step );
        void progress( const std::string &next_step, const unsigned &step_count );
        void resetLinePos();
        void debug( const std::string & msg );
        void msg( const std::string &msg );
        void msg( const std::string &msg, bool cond, const std::string &yes, const std::string &no );
        void warning( const std::string &msg );
        void error( const std::string &msg );

        void setShowDebug( bool state );
        bool showDebug() const;

      private:
        MsgInterface();
        std::unique_ptr<MsgDisplay> _display_out;
        unsigned                    _current_step { 0 };
        unsigned                    _total_steps  { 0 };
    };
}

#endif //BLOGATOR_CLI_MSGINTERFACE_H