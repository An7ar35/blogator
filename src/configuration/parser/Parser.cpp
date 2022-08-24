#include "Parser.h"

#include "token/ConfigTk.h"
#include "../dto/Configuration.h"

using namespace blogator::configuration;

void parser::Parser::dispatch( std::unique_ptr<token::ConfigTk> token ) {

}

std::unique_ptr<Configuration> parser::Parser::reset() {
    return std::unique_ptr<Configuration>();
}

size_t parser::Parser::errors() const {
    return 0;
}
