#include "Checker.h"

#include "../fs/fs.h"

blogator::checker::Checker::Checker( std::shared_ptr<const dto::Index>   master_index,
                                     std::shared_ptr<const dto::Options> global_options ) :
    _master_index( std::move( master_index ) ),
    _options( std::move( global_options ) ),
    _display( cli::MsgInterface::getInstance() )
{}



bool blogator::checker::Checker::init() const {
    //html/css links in given paths (recursive)
    //html validator?
    //
    return false;
}

bool blogator::checker::Checker::checkRelHref( const std::filesystem::path &file_path ) const {
    using blogator::fs::importHTML;
    using blogator::fs::checkRelPaths;


    return false;
}