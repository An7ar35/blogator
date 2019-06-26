#ifndef BLOGATOR_INDEXER_H
#define BLOGATOR_INDEXER_H

#include <string>
#include <vector>
#include <regex>
#include <filesystem>

#include "../dto/Article.h"
#include "../dto/Options.h"
#include "../dto/Index.h"

namespace blogator::indexer {
    std::unique_ptr<dto::Index> index( const std::shared_ptr<dto::Options>& global_options );

    void addTags( const dto::Options &global_options, const dto::Article &article, dto::Index &master_index );
    void addCSS( std::unordered_map<std::string, std::filesystem::path> &found_stylesheets, dto::Article &article );
    void addOutputPath( const dto::Options::Paths &global_paths, dto::Article &article );

    void generateTagDirPaths( blogator::dto::Index &master_index, const dto::Options &global_options );
    void createTagToArticleIndexMap( blogator::dto::Index & master_index );

    dto::DateStamp convertDate( const std::string &date );
    dto::Article readFileProperties( const std::filesystem::path &path );
}

#endif //BLOGATOR_INDEXER_H