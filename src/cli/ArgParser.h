#ifndef BLOGATOR_CLI_ARGPARSER_H
#define BLOGATOR_CLI_ARGPARSER_H

#include <filesystem>
#include <unordered_map>

#include "MsgInterface.h"

namespace blogator::cli {
    class ArgParser {
      public:
        enum class State {
            OK,    //Everything is fine. Carry-on program execution.
            STOP,  //Program does not need to continue. Stop program execution without prejudice.
            ERROR  //Failed to parse 1 or more arguments. Stop program execution with prejudice.
        };

        ArgParser();

        friend std::ostream &operator <<( std::ostream &s, const ArgParser &arg_parser ) {
            s << "> Working directory ...: " << arg_parser.getWorkingDirectory() << "\n"
              << "> Temporary directory .: " << arg_parser.getTempDirectory();
            return s;
        }

        State parse( int argc, char **argv );

        std::filesystem::path getWorkingDirectory() const;
        std::filesystem::path getTempDirectory() const;

      private:
        enum class Flag { INVALID, HELP, CREATE_CONFIG, SHOW_DEBUG_MSGS };

        MsgInterface          &_display;
        std::filesystem::path  _working_dir;

        const std::unordered_map<std::string, Flag> _flag_lookup = std::unordered_map<std::string, Flag>( {
            { "-h",              Flag::HELP },
            { "-?",              Flag::HELP },
            { "--help",          Flag::HELP },
            { "-d",              Flag::SHOW_DEBUG_MSGS },
            { "--debug",         Flag::SHOW_DEBUG_MSGS },
            { "-c",              Flag::CREATE_CONFIG },
            { "--create-config", Flag::CREATE_CONFIG }
        } );

        bool setPath( const std::string & str );
        Flag isFlag( const std::string &str ) const;

        void printHelp() const;
        bool createConfigFile() const;
    };
}

#endif //BLOGATOR_CLI_ARGPARSER_H