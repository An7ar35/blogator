#include "parsing_failure.h"

using namespace blogator::exception;

parsing_failure::parsing_failure( const char * what ) :
    logic_error( what )
{}

parsing_failure::parsing_failure( const std::string &what ) :
    logic_error( what )
{}
