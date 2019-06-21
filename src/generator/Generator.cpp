#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>

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
 * @param master_index
 * @return
 */
bool blogator::Generator::init( const blogator::dto::Index &master_index ) {
    std::unique_ptr<dto::IndexPane> html_pane;

    auto master_tag_index = createTagIndex( master_index );

    if( _options._navigation.index_div_location != dto::Options::Navigation::IndexNavPos::NONE )
        html_pane = generateIndexPaneHTML( master_index, *master_tag_index );

    if( html_pane ) {
        std::cout << "====INDEX PANE HTML: DATE TREE====" << std::endl;
        for( const auto &line : html_pane->_date_tree.html._lines )
            std::cout << line << std::endl;

        std::cout << "----Article Index Map----" << std::endl;
        for( const auto &a : html_pane->_date_tree.article_line_map )
            std::cout << a.first << " -> line #" << a.second << std::endl;

        std::cout << "----Date Checkbox Map----" << std::endl;
        for( const auto &date : html_pane->_date_tree.date_line_map )
            std::cout << date.first << " -> line #" << date.second << std::endl;

        std::cout << "====INDEX PANE HTML: TAG TREE====" << std::endl;
        for( const auto &line : html_pane->_tag_tree.html._lines )
            std::cout << line << std::endl;
    }


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

    _header = fs::importHTML( master_index._paths.header );
    _footer = fs::importHTML( master_index._paths.footer );

    const auto header_insert_index = html::reader::findLineOfTag( "</head>", *_header );

    for( const auto &article : master_index._articles ) {
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
 * @param master_index Master Index
 * @return TagIndexMap_t (K=tag string, V=vector of indices to the master Article index)
 */
std::unique_ptr<blogator::Generator::TagIndexMap_t>
    blogator::Generator::createTagIndex( const blogator::dto::Index &master_index ) const
{
    auto tag_index = std::make_unique<TagIndexMap_t>();

    size_t i = 0;
    for( const auto &article : master_index._articles ) {
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

/**
 * Generate the HTML for the index pane
 * @param master_index Master Index of articles
 * @param tag_index    Master Index of tags to articles
 * @return Generated HTML for the Index pane
 */
std::unique_ptr<blogator::dto::IndexPane>
    blogator::Generator::generateIndexPaneHTML( const dto::Index    &master_index,
                                                const TagIndexMap_t &tag_index ) const
{
    auto pane = std::make_unique<dto::IndexPane>();
    pane = generateIndexPane_ByDate_HTML( master_index, tag_index, std::move( pane ) );
    pane = generateIndexPane_ByTags_HTML( master_index, tag_index, std::move( pane ) );
    return std::move( pane );
}

/**
 * Generates the HTML for the By-Date section of the index pane
 * @param master_index Master Index of articles
 * @param tag_index    Master Index of tags to articles
 * @param index_pane   Index Pane DTO
 * @return IndexPane DTO
 */
std::unique_ptr<blogator::dto::IndexPane>
    blogator::Generator::generateIndexPane_ByDate_HTML( const blogator::dto::Index & master_index,
                                                        const blogator::Generator::TagIndexMap_t & tag_index,
                                                        std::unique_ptr<blogator::dto::IndexPane> index_pane ) const
{
    auto pane = std::move( index_pane );

    auto article = master_index._articles.cbegin();
    if( article != master_index._articles.cend() ) {
        size_t curr_i    = 0; //article position in master index
        auto   curr_date = article->_datestamp;

        html::writer::openTree( pane->_date_tree );
        html::writer::openYearNode( article->_datestamp, pane->_date_tree );
        html::writer::openMonthNode( article->_datestamp, _month_map, pane->_date_tree );

        while( article != master_index._articles.cend() ) {

            if( article->_datestamp._year != curr_date._year ) {
                html::writer::closeMonthNode( pane->_date_tree );
                html::writer::closeYearNode( pane->_date_tree );
                html::writer::openYearNode( article->_datestamp, pane->_date_tree );
                html::writer::openMonthNode( article->_datestamp, _month_map, pane->_date_tree );

            } else if( article->_datestamp._month != curr_date._month ) {
                html::writer::closeMonthNode( pane->_date_tree );
                html::writer::openMonthNode( article->_datestamp, _month_map, pane->_date_tree );
            }

            html::writer::addArticleLeaf( *article, curr_i, pane->_date_tree );

            curr_date = article->_datestamp;
            ++curr_i;
            ++article;
        }

        html::writer::closeMonthNode( pane->_date_tree );
        html::writer::closeYearNode( pane->_date_tree );
        html::writer::closeTree( pane->_date_tree );
    }

    return std::move( pane );
}

/**
 * Generates the HTML for the By-Tag section of the index pane
 * @param master_index Master Index of articles
 * @param tag_index    Master Index of tags to articles
 * @param index_pane   Index Pane DTO
 * @return IndexPane DTO
 */
std::unique_ptr<blogator::dto::IndexPane>
    blogator::Generator::generateIndexPane_ByTags_HTML( const blogator::dto::Index & master_index,
                                                        const blogator::Generator::TagIndexMap_t & tag_index,
                                                        std::unique_ptr<blogator::dto::IndexPane> index_pane ) const
{
    auto pane = std::move( index_pane );
//TODO
    auto mapTag = []( const std::string &tag, dto::IndexPane &pane ) {

    };
    //====INDEX BY-TAGS====
    html::writer::openTree( pane->_tag_tree );

    for( const auto &tag : master_index._global_tags ) {

    }

    html::writer::closeTree( pane->_tag_tree );
//    auto mapArticle( size_t article_index, )
    //    void mapTag( const std::string &tag );
    //    void mapArticleToByTagLine( size_t article_index );
    return std::move( pane );
}
