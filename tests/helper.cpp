#include "helper.h"

#include <fstream>

/**
 * Loads the content of a file into a string
 * @param path Filepath
 * @return Content string
 */
std::string blogator::tests::loadFile( const std::filesystem::path &path ) {
    std::ifstream     file;
    std::stringstream ss;

    file.open( path, std::ios::binary | std::ios::in );

    if( file ) {
        ss << file.rdbuf();
        file.close();
    }

    return ss.str();
}