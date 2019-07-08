#ifndef BLOGATOR_CLI_MSGDISPLAY_H
#define BLOGATOR_CLI_MSGDISPLAY_H

#include <iomanip>
#include <queue>

namespace blogator::cli {
    class MsgDisplay {
      public:
        MsgDisplay();
        ~MsgDisplay();
        void newProcess( const std::string &process, const std::string &next_step  );
        void update( const double &progress );
        void update( const std::string &next_step, const double &progress );
        void error( const std::string &msg );
        void done();
        void flushErrorsToDisplay();

      private:
        const size_t            BAR_WIDTH  { 40 };
        const size_t            PRECISION  {  0 };
        const char              CHAR_EMPTY { ' ' };

        std::string             _progress_bar;
        std::string             _curr_process;
        std::string             _curr_step;

        std::stringstream       _err_buffer;
        std::streambuf         *_old_cerr_buffer;

        void updateDisplay( const double &progress );
    };
}

#endif //BLOGATOR_CLI_MSGDISPLAY_H