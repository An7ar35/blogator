#include "ParserLog.h"

#include <iostream> //TODO temp/remove

using namespace blogator::parser::logging;

void ParserLog::log( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position ) { //TODO
    auto err = ErrorObject( src, ctx, err_code, position );

    std::cout << "{\n"
              << "\tpath: " << err.filepath() << "\n"
              << "\tcontext: " << err.context() << "\n"
              << "\terror: " << err.error() << "\n"
              << "\tdetailed: " << err.detailed() << "\n"
              << "}\n";
}
