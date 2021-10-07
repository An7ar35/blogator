#include "ParserLog.h"

#include <iostream> //TODO temp/remove
#include <utility>

using namespace blogator::parser::logging;

ParserLog::OutputCallback_f ParserLog::_out_cb;

void ParserLog::attachOutputCallback( ParserLog::OutputCallback_f cb ) {
    ParserLog::_out_cb = std::move( cb );
}

void ParserLog::log( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position ) { //TODO
    auto err = ErrorObject( src, ctx, err_code, position );

    if( ParserLog::_out_cb ) {
        ParserLog::_out_cb( err );
    }

    std::cout << "{\n"
              << "\tpath: " << err.filepath() << "\n"
              << "\tcontext: " << err.context() << "\n"
              << "\terror: " << err.error() << "\n"
              << "\tdetailed: " << err.detailed() << "\n"
              << "}\n";
}
