#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "../fs/fs.h"
#include "../html/html.h"
#include "../exception/file_access_failure.h"

/**
 * Constructor
 * @param global_options Global blogator options
 */
blogator::Generator::Generator( blogator::dto::Options global_options ) :
    _options( std::move( global_options ) )
{}

/**
 * Initialize HTML page generation
 * @param master_index Master index of all posts/articles
 * @return Success
 */
bool blogator::Generator::init( const blogator::dto::Index &master_index ) {
    auto master_tag_index = dto::Index::createTagIndex( master_index );
    auto templates        = importTemplates( master_index );

    try {
        templates->_months = fs::importMonthNames( _options._paths.month_file );
    } catch ( std::exception &e ) {
        std::cout << "Using default month strings (eng)." << std::endl;
    }

    auto posts_ok = createPostPages( master_index, *templates, *master_tag_index );
    auto index_ok = createIndexPages( master_index, *templates, *master_tag_index );
    auto start_ok = createStartPage( master_index, *templates, *master_tag_index );

    return ( index_ok && posts_ok && start_ok );
}

/**
 * Imports the HTML templates to use
 * @param options Option DTO (with the paths to the html template files)
 * @return Template DTO loaded and analysed for insertion points
 */
std::unique_ptr<blogator::dto::Template>
    blogator::Generator::importTemplates( const blogator::dto::Index &master_index ) const
{
    auto templates = std::make_unique<dto::Template>();

    templates->_post.html  = fs::importHTML( master_index._paths.post_template );
    templates->_index.html = fs::importHTML( master_index._paths.index_template );
    templates->_start.html = fs::importHTML( master_index._paths.start_template );

    html::reader::findInsertPositions( *templates->_post.html, templates->_post.div_write_pos );
    html::reader::findInsertPositions( *templates->_index.html, templates->_index.div_write_pos );
    html::reader::findInsertPositions( *templates->_start.html, templates->_start.div_write_pos );

    return std::move( templates );
}

/**
 * Generates all post/article HTML pages
 * @param master_index     Master index
 * @param templates        Template DTO
 * @param master_tag_index Master tag index
 * @return Success
 */
bool blogator::Generator::createPostPages( const dto::Index &master_index,
                                           const blogator::dto::Template &templates,
                                           const blogator::Generator::TagIndexMap_t &master_tag_index ) const
{
    auto consecutive_div_pos = dto::Template::getConsecutiveWritePositions( templates._post.div_write_pos );

    std::unique_ptr<dto::IndexDateTree> html_date_tree;
    std::unique_ptr<dto::IndexTagTree>  html_tag_tree;

    if( templates._post.div_write_pos.find( "index_pane_dates" ) != templates._post.div_write_pos.end() )
        html_date_tree = generateIndexDateTreeHTML( master_index, templates );

    if( templates._post.div_write_pos.find( "index_pane_tags" ) != templates._post.div_write_pos.end() )
        html_tag_tree = generateIndexTagTreeHTML( master_index, master_tag_index );

    try {
        const auto css_insert_line = html::reader::findLineOfTag( "</head>", *templates._post.html );
        size_t     curr_article_i  = 0;

        for( const auto &article : master_index._articles ) {
            const auto html_out    = ( _options._paths.posts_dir / article._paths.out_html );
            const auto css_link    = html::createStylesheetLink( article._paths.css, html_out );

            if( std::filesystem::exists( html_out ) )
                throw exception::file_access_failure(
                    "File '" + html_out.string() + "' already exists. Possible files with duplicate names in source folder structure."
                );

            auto article_out = std::ofstream( html_out );

            if( !article_out.is_open() )
                throw exception::file_access_failure(
                    "File '" + html_out.string() + "' could not be opened for writing."
                );

            size_t curr_src_line = 0;
            auto   insert_point  = consecutive_div_pos.begin();
            for( const auto &template_line : templates._post.html->_lines ) {
                if( !article._paths.css.empty() && css_insert_line == curr_src_line )
                    article_out << css_link << std::endl;

                if( insert_point->first.line == curr_src_line ) {
                    const auto char_it = find_if( template_line.begin(),
                                                  template_line.end(),
                                                  []( char c ) { return !isspace( c ); } );
                    const auto spaces  = template_line.substr( 0, char_it - template_line.begin() );
                    article_out << template_line.substr( 0, insert_point->first.col ) << std::endl;

                    if( insert_point->second == "page_index" ) {
                        writePageNavDiv( article_out, spaces, master_index, curr_article_i );
                    } else if( insert_point->second == "post_content" ) {
                        writeContentDiv( article._paths.src_html, spaces, article_out );
                    } else if( insert_point->second == "index_pane_dates" ) {
                        writeIndexDateTree( article_out, spaces, article, curr_article_i, *html_date_tree );
                    } else if( insert_point->second == "index_pane_tags" ) {
                        writeIndexTagTree( article_out, spaces, article, curr_article_i, *html_tag_tree );
                    } else {
                        std::cerr << "Tag HTML Div class '" << insert_point->second << "' not recognised." << std::endl;
                    }

                    article_out << spaces << template_line.substr( insert_point->first.col ) << std::endl;
                    ++insert_point;

                } else {
                    article_out << template_line << std::endl;
                }

                ++curr_src_line;
            }

            article_out.close();
            ++curr_article_i;
        }

        return true;

    } catch( exception::file_access_failure &e ) {
        std::cerr << e.what() << std::endl;
    }

    return false;
}

bool blogator::Generator::createIndexPages( const blogator::dto::Index &master_index, //TODO
                                            const blogator::dto::Template &templates,
                                            const blogator::Generator::TagIndexMap_t &master_tag_index ) const
{
    return false;
}

bool blogator::Generator::createStartPage( const blogator::dto::Index &master_index, //TODO
                                           const blogator::dto::Template &templates,
                                           const blogator::Generator::TagIndexMap_t &master_tag_index ) const
{
    return false;
}


/**
 * Generates the HTML for the By-Date section of the index pane
 * @param master_index    Master Index of articles
 * @param master_template Master templates (for the month strings lookup)
 * @return IndexDateTree DTO
 */
std::unique_ptr<blogator::dto::IndexDateTree>
    blogator::Generator::generateIndexDateTreeHTML( const blogator::dto::Index &master_index,
                                                    const blogator::dto::Template &master_template ) const
{
    auto tree = std::make_unique<dto::IndexDateTree>();

    auto article = master_index._articles.cbegin();
    if( article != master_index._articles.cend() ) {
        size_t curr_i    = 0; //article position in master index
        auto   curr_date = article->_datestamp;

        html::writer::openTree( *tree );
        html::writer::openYearNode( article->_datestamp, *tree );
        html::writer::openMonthNode( article->_datestamp, master_template._months, *tree );

        while( article != master_index._articles.cend() ) {

            if( article->_datestamp._year != curr_date._year ) {
                html::writer::closeMonthNode( *tree );
                html::writer::closeYearNode( *tree );
                html::writer::openYearNode( article->_datestamp, *tree );
                html::writer::openMonthNode( article->_datestamp, master_template._months, *tree );

            } else if( article->_datestamp._month != curr_date._month ) {
                html::writer::closeMonthNode( *tree );
                html::writer::openMonthNode( article->_datestamp, master_template._months, *tree );
            }

            html::writer::addArticleLeaf( *article, curr_i, *tree );

            curr_date = article->_datestamp;
            ++curr_i;
            ++article;
        }

        html::writer::closeMonthNode( *tree );
        html::writer::closeYearNode( *tree );
        html::writer::closeTree( *tree );
    }

    return std::move( tree );
}

/**
 * Generates the HTML for the By-Tag section of the index pane
 * @param master_index    Master Index of articles
 * @param tag_index       Master Index of tags to articles
 * @return IndexTagTree DTO
 */
std::unique_ptr<blogator::dto::IndexTagTree>
    blogator::Generator::generateIndexTagTreeHTML( const blogator::dto::Index &master_index,
                                                   const blogator::Generator::TagIndexMap_t &tag_index ) const
{
    auto tree = std::make_unique<dto::IndexTagTree>();

    html::writer::openTree( *tree );

    for( const auto &tag : tag_index ) {
        html::writer::openTagNode( tag.first, *tree );

        for( auto article_i : tag.second ) {
            try {
                html::writer::addArticleLeaf( master_index._articles.at( article_i ),
                                              article_i,
                                              *tree );

            } catch( std::out_of_range &e ) {
                std::cerr << "Could not find article in i=" << article_i << " referenced in tag '"
                          << tag.first << "'. Skipping." << std::endl;
            }
        }

        html::writer::closeTagNode( *tree );
    }

    html::writer::closeTree( *tree );

    return std::move( tree );
}

/**
 * Writes the source article lines to a file
 * @param in         Source article
 * @param fore_space Space to place before the output line
 * @param out        Target file
 * @throws blogator::exception::file_access_failure when source file could not be opened
 */
void blogator::Generator::writeContentDiv( const std::filesystem::path &source_path,
                                           const std::string &fore_space,
                                           std::ofstream &out ) const
{
    auto in = std::ifstream( source_path );

    if( !in.is_open() )
        throw exception::file_access_failure( "Cannot read source file '" + source_path.string() + "'." );

    std::string line;
    while( getline( in, line ) )
        out << fore_space << "\t" << line << std::endl;

    in.close();
}

/**
 * Writes the page navigation html to a file
 * @param file        Target file
 * @param fore_space  Space to place before the output line
 * @param index       Master Index
 * @param article_pos Position of the current article for the page in the master index
 */
void blogator::Generator::writePageNavDiv( std::ofstream &file,
                                           const std::string &fore_space,
                                           const blogator::dto::Index &master_index,
                                           const size_t &article_pos ) const
{
    const bool is_first = article_pos == 0;
    const bool is_last  = article_pos != master_index._articles.size() - 1;

    file << fore_space << "\t"
         << ( is_first
            ? html::createHyperlink( master_index._articles.at( 0 )._paths.out_html, _options._page_nav.first, "disabled" )
            : html::createHyperlink( master_index._articles.at( 0 )._paths.out_html, _options._page_nav.first ) )
         << ( is_first
            ? html::createHyperlink( master_index._articles.at( 0 )._paths.out_html, _options._page_nav.backwards, "disabled" )
            : html::createHyperlink( master_index._articles.at( article_pos - 1 )._paths.out_html, _options._page_nav.backwards, "disabled" ) )
         << std::to_string( article_pos + 1 ) << _options._page_nav.separator << std::to_string( master_index._articles.size() )
         << ( is_last
            ? html::createHyperlink( master_index._articles.at( master_index._articles.size() - 1 )._paths.out_html, _options._page_nav.forward, "disabled" )
            : html::createHyperlink( master_index._articles.at( master_index._articles.size() - 1 )._paths.out_html, _options._page_nav.forward ) )
         << ( is_last
            ? html::createHyperlink( master_index._articles.at( master_index._articles.size() - 1 )._paths.out_html, _options._page_nav.last, "disabled" )
            : html::createHyperlink( master_index._articles.at( master_index._articles.size() - 1 )._paths.out_html, _options._page_nav.last ) )
         << std::endl;
}

/**
 * Writes the indexed date tree for a 'post' page
 * @param file        Target 'post' file
 * @param fore_space  Space to place before the output line
 * @param article     Article DTO
 * @param article_pos Position of article in the master index
 * @param tree        IndexDateTree source html lines
 */
void blogator::Generator::writeIndexDateTree( std::ofstream &file,
                                              const std::string &fore_space,
                                              const blogator::dto::Article &article,
                                              const size_t &article_pos,
                                              const blogator::dto::IndexDateTree &tree ) const
{
    auto year_line    = tree.date_line_map.find( article._datestamp._year );
    auto month_line   = tree.date_line_map.find( dto::DateStamp::concatYYYYMM( article._datestamp ) );
    auto article_line = tree.article_line_map.find( article_pos );

    size_t i = 0;
    for( const auto &line : tree.html._lines ) {
        if( year_line != tree.date_line_map.end() && year_line->second == i )
            file << fore_space << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
        else if( month_line != tree.date_line_map.end() && month_line->second == i )
            file << fore_space << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
        else if( article_line != tree.article_line_map.end() && article_line->second == i )
            file << fore_space << "\t" << html::writer::setHyperlinkClass( line, "current" ) << std::endl;
        else
            file << fore_space << "\t" << line << std::endl;
        ++i;
    }
}

/**
 * Write the indexed tag tree for a 'post' page
 * @param file        Target 'post' file
 * @param fore_space  Space to place before the output line
 * @param article     Article DTO
 * @param article_pos Position of article in the master index
 * @param tree        IndexTagTree source html lines
 */
void blogator::Generator::writeIndexTagTree( std::ofstream &file,
                                             const std::string &fore_space,
                                             const blogator::dto::Article &article,
                                             const size_t &article_pos,
                                             const blogator::dto::IndexTagTree &tree ) const
{
    auto tag_checkbox_lines = std::vector<dto::HTML::LineIndex_t>();
    auto article_link_lines = std::vector<dto::HTML::LineIndex_t>();

    //<input type="checkbox"..> lines
    for( const auto &t : article._tags ) {
        auto it = tree.tag_line_map.find( t );
        if( it != tree.tag_line_map.end() )
            tag_checkbox_lines.emplace_back( it->second );
    }

    //<a>...</a> article link lines
    auto it = tree.articles_line_map.find( article_pos );
    if( it != tree.articles_line_map.end() )
        std::copy( it->second.begin(), it->second.end(), std::back_inserter( article_link_lines ) );

    //sort by ascending line number
    std::sort( tag_checkbox_lines.begin(), tag_checkbox_lines.end() );
    std::sort( article_link_lines.begin(), article_link_lines.end() );

    //writing index tree html
    auto checkbox_line = tag_checkbox_lines.cbegin();
    auto link_line     = article_link_lines.cbegin();

    size_t i = 0;
    for( const auto &line : tree.html._lines ) {
        if( checkbox_line != tag_checkbox_lines.cend() && *checkbox_line == i ) {
            file << fore_space << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
            ++checkbox_line;
        } else if( link_line != article_link_lines.cend() && *link_line == i ) {
            file << fore_space << "\t" << html::writer::setHyperlinkClass( line, "current" ) << std::endl;
            ++link_line;
        } else {
            file << fore_space << "\t" << line << std::endl;
        }
        ++i;
    }
}
