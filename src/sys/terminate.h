#ifndef BLOGATOR_SYS_TERMINATE_H
#define BLOGATOR_SYS_TERMINATE_H

#include "stdexcept"

#define TERMINATE( desc ) blogator::terminate( __FILE__, __LINE__, desc );

namespace blogator {
    void terminate( const char * file, int line, const char * dec );
};

#endif //BLOGATOR_TERMINATE_H