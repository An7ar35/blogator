#include <utility>

#include "PostMaker.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "../fs/fs.h"
#include "../html/html.h"
#include "../exception/failed_expectation.h"
#include "../exception/file_access_failure.h"
#include "../cli/MsgInterface.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::generator::PostMaker::PostMaker( std::shared_ptr<const dto::Index>    master_index,
                                           std::shared_ptr<const dto::Template> templates,
                                           std::shared_ptr<const dto::Options>  global_options  ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) )
{}

/**
 * Initialize HTML post pages generation
 * @return Success
 * @throws exception::failed_expectation when no insertion points are found in the post template
 */
bool blogator::generator::PostMaker::init() {
    auto       &display       = cli::MsgInterface::getInstance();
    const auto total_articles = '/' + std::to_string( _master_index->_articles.size() );

    auto insert_points = dto::Template::getConsecutiveWritePositions( _templates->_post.div_write_pos );

    if( insert_points.empty() )
        throw exception::failed_expectation( "No insertion points found in post template." );

    std::unique_ptr<dto::IndexDateTree> html_date_tree;
    std::unique_ptr<dto::IndexTagTree>  html_tag_tree;

    if( _templates->_post.div_write_pos.find( "index-pane-dates" ) != _templates->_post.div_write_pos.end() )
        html_date_tree = generateIndexDateTreeHTML();

    if( _templates->_post.div_write_pos.find( "index-pane-tags" ) != _templates->_post.div_write_pos.end() )
        html_tag_tree = generateIndexTagTreeHTML();

    try {
        const auto css_insert_line = html::reader::findLineOfTag( "</head>", *_templates->_post.html );
        size_t     curr_article_i  = 0;

        for( const auto &article : _master_index->_articles ) {
            if( curr_article_i == 0 )
                display.begin( "Generating posts", _master_index->_articles.size(), std::to_string( curr_article_i + 1 ) + total_articles );
            else
                display.progress( std::to_string( curr_article_i + 1 ) + total_articles );

            const auto html_out    = ( _options->_paths.posts_dir / article._paths.out_html );
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

            article_out << _options->BLOGATOR_SIGNATURE << "\n";

            size_t curr_src_line = 0;
            auto   insert_point  = insert_points.cbegin();
            for( const auto &template_line : _templates->_post.html->_lines ) {
                if( !article._paths.css.empty() && css_insert_line == curr_src_line )
                    article_out << css_link << std::endl;

                if( insert_point != insert_points.cend() && insert_point->first.line == curr_src_line ) {

                    const auto indent = html::reader::getIndent( template_line );
                    article_out << template_line.substr( 0, insert_point->first.col ) << std::endl;

                    if( insert_point->second == "breadcrumb" ) {
                        writeBreadcrumb( article_out, indent, article );
                    } else if( insert_point->second == "page-nav" ) {
                        writePageNavDiv( article_out, indent, curr_article_i );
                    } else if( insert_point->second == "post-content" ) {
                        writeContentDiv( article._paths.src_html, indent, article_out );
                    } else if( insert_point->second == "index-pane-dates" ) {
                        writeIndexDateTree( article_out, indent, article, curr_article_i, *html_date_tree );
                    } else if( insert_point->second == "index-pane-tags" ) {
                        writeIndexTagTree( article_out, indent, article, curr_article_i, *html_tag_tree );
                    } else {
                        std::cerr << "generator::PostMaker::init(..)] "
                                  << "HTML Div class '" << insert_point->second << "' not recognised."
                                  << std::endl;
                    }

                    article_out << indent << template_line.substr( insert_point->first.col ) << std::endl;
                    ++insert_point;

                } else {
                    article_out << template_line << std::endl;
                }

                ++curr_src_line;
            }

            article_out.close();
            ++curr_article_i;
        }

    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    display.progress( "DONE" );
    return true;
}

/**
 * Generates the HTML for the By-Date section of the index pane
 * @return IndexDateTree DTO
 */
std::unique_ptr<blogator::dto::IndexDateTree>
    blogator::generator::PostMaker::generateIndexDateTreeHTML() const
{
    auto tree = std::make_unique<dto::IndexDateTree>();

    auto article = _master_index->_articles.cbegin();
    if( article != _master_index->_articles.cend() ) {
        size_t curr_i    = 0; //article position in master index
        auto   curr_date = article->_datestamp;

        html::writer::openTree( *tree );
        html::writer::openYearNode( article->_datestamp, *tree );
        html::writer::openMonthNode( article->_datestamp, _options->_months, *tree );

        while( article != _master_index->_articles.cend() ) {

            if( article->_datestamp._year != curr_date._year ) {
                html::writer::closeMonthNode( *tree );
                html::writer::closeYearNode( *tree );
                html::writer::openYearNode( article->_datestamp, *tree );
                html::writer::openMonthNode( article->_datestamp, _options->_months, *tree );

            } else if( article->_datestamp._month != curr_date._month ) {
                html::writer::closeMonthNode( *tree );
                html::writer::openMonthNode( article->_datestamp, _options->_months, *tree );
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
 * @return IndexTagTree DTO
 */
std::unique_ptr<blogator::dto::IndexTagTree>
    blogator::generator::PostMaker::generateIndexTagTreeHTML() const
{
    auto tree = std::make_unique<dto::IndexTagTree>();

    html::writer::openTree( *tree );

    for( const auto &tag : _master_index->_indices.byTag.tags ) {
        html::writer::openTagNode( tag.first, *tree );

        for( auto article_i : tag.second.article_indices ) {
            try {
                html::writer::addArticleLeaf( _master_index->_articles.at( article_i ),
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
 * @param in     Source article
 * @param indent Space to place before the output line (i.e.: html indent)
 * @param out    Target file
 * @throws blogator::exception::file_access_failure when source file could not be opened
 */
void blogator::generator::PostMaker::writeContentDiv( const std::filesystem::path &source_path,
                                                      const std::string &indent,
                                                      std::ofstream &out ) const
{
    auto in = std::ifstream( source_path );

    if( !in.is_open() )
        throw exception::file_access_failure( "Cannot read source file '" + source_path.string() + "'." );
//    <span class="post_number"></span> //TODO what the hell is this comment for? if bs then remove
    std::string line;
    while( getline( in, line ) )
        out << indent << "\t" << line << std::endl;

    in.close();
}

/**
 * Writes the page navigation html to a file
 * @param file        Target file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param article_pos Position of the current article for the page in the master index
 */
void blogator::generator::PostMaker::writePageNavDiv( std::ofstream &file,
                                                      const std::string &indent,
                                                      const size_t &article_pos ) const
{
    const bool is_first = article_pos == 0;
    const bool is_last  = article_pos == _master_index->_articles.size() - 1;

    file << indent << "\t"
         << ( is_first
              ? html::createHyperlink( _master_index->_articles.at( 0 )._paths.out_html, _options->_page_nav.first, "disabled" )
              : html::createHyperlink( _master_index->_articles.at( 0 )._paths.out_html, _options->_page_nav.first ) )
         << ( is_first
              ? html::createHyperlink( _master_index->_articles.at( 0 )._paths.out_html, _options->_page_nav.backward, "disabled" )
              : html::createHyperlink( _master_index->_articles.at( article_pos - 1 )._paths.out_html, _options->_page_nav.backward, "disabled" ) )
         << std::to_string( article_pos + 1 ) << _options->_page_nav.separator << std::to_string( _master_index->_articles.size() )
         << ( is_last
              ? html::createHyperlink( _master_index->_articles.at( _master_index->_articles.size() - 1 )._paths.out_html, _options->_page_nav.forward, "disabled" )
              : html::createHyperlink( _master_index->_articles.at( article_pos + 1 )._paths.out_html, _options->_page_nav.forward ) )
         << ( is_last
              ? html::createHyperlink( _master_index->_articles.at( _master_index->_articles.size() - 1 )._paths.out_html, _options->_page_nav.last, "disabled" )
              : html::createHyperlink( _master_index->_articles.at( _master_index->_articles.size() - 1 )._paths.out_html, _options->_page_nav.last ) )
         << std::endl;
}

/**
 * Writes the indexed date tree for a 'post' page
 * @param file        Target 'post' file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param article     Article DTO
 * @param article_pos Position of article in the master index
 * @param tree        IndexDateTree source html lines
 */
void blogator::generator::PostMaker::writeIndexDateTree( std::ofstream &file,
                                                         const std::string &indent,
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
            file << indent << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
        else if( month_line != tree.date_line_map.end() && month_line->second == i )
            file << indent << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
        else if( article_line != tree.article_line_map.end() && article_line->second == i )
            file << indent << "\t" << html::writer::setHyperlinkClass( line, "current" ) << std::endl;
        else
            file << indent << "\t" << line << std::endl;
        ++i;
    }
}

/**
 * Write the indexed tag tree for a 'post' page
 * @param file        Target 'post' file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param article     Article DTO
 * @param article_pos Position of article in the master index
 * @param tree        IndexTagTree source html lines
 */
void blogator::generator::PostMaker::writeIndexTagTree( std::ofstream &file,
                                                        const std::string &indent,
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
            file << indent << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
            ++checkbox_line;
        } else if( link_line != article_link_lines.cend() && *link_line == i ) {
            file << indent << "\t" << html::writer::setHyperlinkClass( line, "current" ) << std::endl;
            ++link_line;
        } else {
            file << indent << "\t" << line << std::endl;
        }
        ++i;
    }
}

/**
 * Writes the breadcrumb for a 'post' page
 * @param file    Target 'post' file
 * @param indent  Space to place before the output line (i.e.: html indent)
 * @param article Article
 */
void blogator::generator::PostMaker::writeBreadcrumb( std::ofstream &file,
                                                      const std::string &indent,
                                                      const dto::Article &article ) const
{
    file << indent << "\t<ul>\n"
         << indent << "\t\t<li>" << html::createHyperlink( "../index.html", _options->_breadcrumb.start ) << "</li>\n"
         << indent << "\t\t<li>" << article._heading << "</li>\n"
         << indent << "\t</ul>\n";
}
