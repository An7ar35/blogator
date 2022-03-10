#include "terminate.h"

#include <syslog.h>
#include <iostream>
#include <libgen.h> //basename(..)

#include "../logger/global/LogLevel.h"

/**
 * Terminates the program
 * @param file Source file ('__FILE__')
 * @param line Line number ('__LINE__')
 * @param desc Description/reason for call
 */
void blogator::terminate( const char * file, int line, const char * desc ) {
    syslog( static_cast<int>( logger::LogLevel::CRITICAL ),
            "(%s:%i) %s",
            basename( (char *) file ), line, desc );

    std::cerr << "(" << basename( (char *) file ) << ":" << line << ") "
              << desc
              << std::endl;

    std::terminate();
}