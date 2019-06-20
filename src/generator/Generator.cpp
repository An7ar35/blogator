#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>

#include "../fs/fs.h"
#include "../html/html.h"

/**
 * Constructor
 * @param global_options Global blogator options
 */
blogator::Generator::Generator( blogator::dto::Options global_options ) :
    _options( std::move( global_options ) ),
    _month_map( {
        { 1, "January" }, { 2, "February" }, { 3, "March" }, { 4, "April" }, { 5, "May" },
        { 6, "June" }, { 7, "July" }, { 8, "August" }, { 9, "September" }, { 10, "October" },
        { 11, "November" }, { 12, "December" }
    } )
{}

/**
 * Assigns a map of K=MM, V=MMMM for months strings (e.g.: 9=August)
 * @param month_map Integer->Month strings map
 */
void blogator::Generator::useMonthStrings( const std::unordered_map<unsigned, std::string> &month_map ) {
    //Validate the months
    auto validated = 0;
    for( auto i = 1; i <= 12; ++i )
        validated += ( month_map.find( i ) == month_map.end() ) ? 0 : 1;

    if( validated < 12 )
        std::cout << validated << + "/12 month strings in map. Using defaults (eng)." << std::endl;
    else
        _month_map = month_map;
}

/**
 *
 * @param index
 * @return
 */
bool blogator::Generator::init( const blogator::dto::Index &index ) {
    auto master_tag_index = createTagIndex( index );

    std::cout << "====MASTER TAG INDEX====" << std::endl;
    for( const auto &t : *master_tag_index ) { //TODO remove on release
        std::cout << t.first << " = { ";
        for( auto e : t.second )
            std::cout << e << " ";
        std::cout << "}" << std::endl;
    }

    std::cout << "====MONTH STRINGS====" << std::endl;
    for( const auto &m : _month_map ) //TODO remove on release
        std::cout << "{ "<< m.first << ", " << m.second << " }" << std::endl;

    _header = fs::importHTML( index._paths.header );
    _footer = fs::importHTML( index._paths.footer );

    const auto header_insert_index = html::findLineOfTag( "</head>", *_header );

    for( const auto &article : index._file_index ) {
        const auto html_out = ( _options._paths.posts_dir / article._html_filepath.filename() );
        const auto css_rel  = ( article._css_filepath.parent_path().lexically_relative( html_out.parent_path() ) / article._css_filepath.filename() );

        auto article_in  = std::ifstream( article._html_filepath );
        auto article_out = std::ofstream( html_out );

        if( article_out.is_open() ) {
            //HEADER
            size_t counter = 0;
            for( const auto & line : _header->_lines ) {
                if( !article._css_filepath.empty() && counter == header_insert_index ) {
                    auto css_ref = "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"" + css_rel.string() + "\"/>";
                    article_out << css_ref << std::endl;
                }
                article_out << line << std::endl;
                ++counter;
            }

            //CONTENT
            if( article_in.is_open() ) {
                std::string line;

                while( getline( article_in, line ) )
                    article_out << line << std::endl;

                article_in.close();
            }

            //FOOTER
            for( const auto &line : _footer->_lines )
                article_out << line << std::endl;

            article_out.close();
        } else {
            std::cerr << "File '" << html_out << "' could not be opened for writing." << std::endl;
        }
    }

    return false; //TODO
}

/**
 * Generate a tag index map
 * @param index Master Index
 * @return TagIndexMap_t (K=tag string, V=vector of indices to the master Article index)
 */
std::unique_ptr<blogator::Generator::TagIndexMap_t>
    blogator::Generator::createTagIndex( const blogator::dto::Index &index ) const
{
    auto tag_index = std::make_unique<TagIndexMap_t>();

    size_t i = 0;
    for( const auto &article : index._file_index ) {
        for( const auto &tag : article._tags ) {
            if( tag_index->find( tag ) == tag_index->end() ) {
                tag_index->insert( std::make_pair( tag, std::vector<size_t>( { i } ) ) );
            } else {
                tag_index->at( tag ).emplace_back( i );
            }
            ++i;
        }
    }

    return std::move( tag_index );
}
