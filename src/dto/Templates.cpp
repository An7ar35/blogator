#include "Templates.h"

/**
 * Constructor
 */
blogator::dto::Templates::Templates() :
    _post( std::make_unique<Template>( Template::Type::POST ) ),
    _index( std::make_shared<Template>( Template::Type::INDEX ) ),
    _index_list( std::make_shared<Template>( Template::Type::INDEX_LIST ) ),
    _index_entry( std::make_shared<Template>( Template::Type::INDEX_ENTRY ) ),
    _landing( std::make_unique<Template>( Template::Type::LANDING ) )
{}

/**
 * Extracts any relative paths found in an HTML text
 * @param source Source HTML DTO to extract any relative paths from
 * @return Consecutive positions and extracted paths in the HTML
 */
blogator::dto::ConsecutivePathPositions_t blogator::dto::Templates::extractRelativePaths( dto::HTML &source )  {
    const auto path_rx = std::regex( R"((.*?<.*?(?:src|href)=")([^/]{1}[a-zA-Z\%\.\-_\d/]+\.[a-zA-Z\d_]{2,4})("))" );
    auto       pos     = ConsecutivePathPositions_t();
    size_t     line_i  = 0;

    std::string::size_type last_size = 0;

    for( auto &line : source._lines ) {
        std::stringstream ss;
        auto it = std::sregex_iterator( line.begin(), line.end(), path_rx );
        auto substr_start = 0;
        auto stream_size  = 0;

        while( it != std::sregex_iterator() ) {
            auto col = substr_start + it->str( 1 ).length() - last_size;

            pos.insert( std::make_pair( InsertPosition( line_i, col ), it->str( 2 ) ) );
            ss << it->str( 1 ) << it->str( 3 );

            stream_size  += it->str( 1 ).size() + it->str( 3 ).size();
            substr_start += it->str( 0 ).length();
            last_size     = it->str( 2 ).length();

            ++it;
        }

        ss << line.substr( substr_start );
        line = ss.str();
        last_size = 0;
        ++line_i;
    }

    return pos;
}