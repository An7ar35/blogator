#ifndef BLOGATOR_INDEXER_H
#define BLOGATOR_INDEXER_H

#include <string>
#include <vector>
#include <regex>
#include <filesystem>

#include "FeatAggregator.h"
#include "../dto/Article.h"
#include "../dto/Options.h"
#include "../dto/Index.h"

namespace blogator::indexer {
    std::shared_ptr<dto::Index> index( const std::shared_ptr<dto::Options> &global_options );
    void indexStylesheets( const dto::Options &global_options, dto::Index &index, std::unordered_map<std::string, std::filesystem::path> &css_cache );
    void indexTemplates( const dto::Options &global_options, dto::Index &index );
    void indexPosts( const dto::Options &global_options, dto::Index &index, std::unordered_map<std::string, std::filesystem::path> &css_cache, FeatAggregator &feat_aggregator );

    void addTags( const dto::Options &global_options, const dto::Article &article, dto::Index &master_index );
    void addCSS( std::unordered_map<std::string, std::filesystem::path> &found_stylesheets, dto::Article &article );
    void addOutputPath( const dto::Options::AbsPaths &global_paths, dto::Article &article );

    void sortChronologically( dto::Index::Articles_t &articles );
    void generateTagIndexTargets( dto::Index &master_index, const dto::Options &global_options );
    void generateTopTags( dto::Index &master_index, const dto::Options &global_options );
    void generateDateIndexTargets( blogator::dto::Index & master_index, const dto::Options & global_options );

    dto::DateStamp convertDate( const std::string &date );
    dto::Article readFileProperties( const std::filesystem::path &path );

    std::filesystem::path makeFileName( const size_t &n );
    std::filesystem::path makeFileName( const std::string &prefix, const size_t &n );
    size_t calcPageCount ( const size_t &items_per_page, const size_t &item_count );
}

#endif //BLOGATOR_INDEXER_H