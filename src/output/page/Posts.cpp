#include <utility>

#include "Posts.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "../../fs/fs.h"
#include "../../html/html.h"
#include "../../exception/failed_expectation.h"
#include "../../exception/file_access_failure.h"
#include "../../cli/MsgInterface.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::output::page::Posts::Posts( std::shared_ptr<const dto::Index>     master_index,
                                      std::shared_ptr<const dto::Templates> templates,
                                      std::shared_ptr<const dto::Options>   global_options  ) :
    Page( std::move( master_index ),
               std::move( templates ),
               std::move( global_options ) ),
    _breadcrumb_parents( { { _options->_breadcrumb.start, "../index.html" } } )
{
    auto date_tree_it = _templates->_post->block_classes.find( "index-pane-dates" );
    if( date_tree_it != _templates->_post->block_classes.end() && date_tree_it->second )
        _html_date_tree = html::generateIndexDateTreeHTML( *_index, *_options );

    auto tag_tree_it = _templates->_post->block_classes.find( "index-pane-tags" );
    if( tag_tree_it != _templates->_post->block_classes.end() && tag_tree_it->second )
        _html_tag_tree = html::generateIndexTagTreeHTML( *_index, *_options );
}

/**
 * Initialize HTML post pages generation
 * @return Success
 * @throws exception::failed_expectation when no insertion points are found in the post template
 */
bool blogator::output::page::Posts::init() const {
    auto       &display       = cli::MsgInterface::getInstance();
    const auto total_articles = '/' + std::to_string( _index->_articles.size() );

    auto &insert_points = _templates->_post->block_write_pos;

    if( insert_points.empty() )
        throw exception::failed_expectation( "No insertion points found in post template." );

    try {
        const auto css_insert_line = html::reader::findLineOfTag( "</head>", *_templates->_post->html );
        size_t     curr_article_i  = 0;

        for( const auto &article : _index->_articles ) {
            if( curr_article_i == 0 )
                display.begin( "Generating posts", _index->_articles.size(), std::to_string( curr_article_i + 1 ) + total_articles );
            else
                display.progress( std::to_string( curr_article_i + 1 ) + total_articles );

            const auto html_out = ( _options->_paths.posts_dir / article._paths.out_html );
//            const auto css_link = html::createStylesheetLink( article._paths.css, html_out );//TODO

            if( std::filesystem::exists( html_out ) )
                throw exception::file_access_failure(
                    "File '" + html_out.string() + "' already exists. Possible files with duplicate names in source folder structure."
                );

            dto::Page page = { html_out, std::ofstream( html_out ) };

            if( !page._out.is_open() )
                throw exception::file_access_failure(
                    "File '" + html_out.string() + "' could not be opened for writing."
                );

            page._out << _options->BLOGATOR_SIGNATURE << "\n";

            dto::Template::WritePosIterators insert_it = { _templates->_post->block_write_pos.cbegin(),
                                                           _templates->_post->path_write_pos.cbegin() };

            dto::Line line     = { _templates->_post->html->_lines.cbegin(), 0 };
            PageInfo page_info = { article, curr_article_i, css_insert_line };

            while( line._it != _templates->_post->html->_lines.cend() ) {
                writeTemplateLine( page, line, page_info, insert_it );
                ++line;
            }

            page._out.close();
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
 * Inserts any paths/blocks that are on a line
 * @param page      Output page DTO
 * @param line      Line DTO
 * @param page_info Page info (Article, Article i, css line)
 * @param insert_it Iterators to the current block and path insertion point
 */
void blogator::output::page::Posts::writeTemplateLine( dto::Page &page,
                                                       const dto::Line &line,
                                                       const PageInfo  &page_info,
                                                       dto::Template::WritePosIterators &insert_it ) const
{
    auto hasBlock = [&]() { return ( insert_it.block != _templates->_post->block_write_pos.cend() && insert_it.block->first.line == line._num ); };
    auto hasPath  = [&]() { return ( insert_it.path  != _templates->_post->path_write_pos.cend()  && insert_it.path->first.line  == line._num ); };

    std::string::size_type col = 0;
    const auto indent = html::reader::getIndent( *line._it );

    if( !page_info.article._paths.css.empty() && page_info.css_line == line._num )
        page._out << html::createStylesheetLink( page_info.article._paths.css, page._abs_path ) << std::endl;

    //EARLY RETURN: when there's nothing to add/edit on the line
    if( !hasBlock() && !hasPath() ) {
        page._out << *line._it << "\n";
        return;
    }

    while( hasBlock() && hasPath() ) {
        if( insert_it.block->first.col < insert_it.path->first.col ) { //do the block first
            page._out << line._it->substr( col, insert_it.block->first.col - col ) << "\n";
            writeHtmlBlock( page, indent + "\t", page_info, insert_it.block->second );
            page._out << indent;

            col = insert_it.block->first.col;
            ++insert_it.block;

        } else { //do the path first
            page._out << line._it->substr( col, insert_it.path->first.col - col )
                      << fs::adaptRelPath( _index->_paths.templates.landing, page._abs_path, insert_it.path->second.string() ).string();

            col = insert_it.path->first.col;
            ++insert_it.path;
        }
    }

    while( hasBlock() ) { //just block(s) left to insert
        page._out << line._it->substr( col, insert_it.block->first.col - col ) << "\n";
        writeHtmlBlock( page, indent + "\t", page_info, insert_it.block->second );
        page._out << indent;

        col = insert_it.block->first.col;
        ++insert_it.block;
    }

    while( hasPath() ) { //just path(s) left to insert
        page._out << line._it->substr( col, insert_it.path->first.col - col )
                  << fs::adaptRelPath( _index->_paths.templates.landing, page._abs_path, insert_it.path->second.string() ).string();

        col = insert_it.path->first.col;
        ++insert_it.path;
    }

    page._out << line._it->substr( col ) << "\n";
}

/**
 * Writes the relevant HTML block at the given position
 * @param page        Output file
 * @param indent      Space to place before the output lines (i.e.: html indent)
 * @param page_info   Page info (Article, Article i, css line)
 * @param block_class Block's class name
 */
void blogator::output::page::Posts::writeHtmlBlock( dto::Page &page,
                                                    const std::string &indent,
                                                    const PageInfo    &page_info,
                                                    const std::string &block_class ) const
{
    if( block_class == "breadcrumb" ) {
        writeBreadcrumb( page, indent, _breadcrumb_parents, page_info.article._heading );
    } else if( block_class == "page-nav" ) {
        writePageNavDiv( page, indent, page_info.article_i );
    } else if( block_class == "post-content" ) {
        writeContentDiv( page_info.article._paths.src_html, indent, page._out );
    } else if( block_class == "index-pane-dates" ) {
        writeIndexDateTree( page, indent, page_info.article, page_info.article_i );
    } else if( block_class == "index-pane-tags" ) {
        writeIndexTagTree( page, indent, page_info.article, page_info.article_i );
    } else {
        std::cerr << "output::page::Posts::init(..)] "
                  << "HTML Div class '" << block_class << "' not recognised."
                  << std::endl;
    }
}

/**
 * Writes the source article lines to a file
 * @param in     Source article
 * @param indent Space to place before the output line (i.e.: html indent)
 * @param page   Target file
 * @throws blogator::exception::file_access_failure when source file could not be opened
 */
void blogator::output::page::Posts::writeContentDiv( const std::filesystem::path &source_path,
                                                     const std::string &indent,
                                                     std::ofstream &page ) const
{
    auto in = std::ifstream( source_path );

    if( !in.is_open() )
        throw exception::file_access_failure( "Cannot read source file '" + source_path.string() + "'." );
//    <span class="post_number"></span> //TODO look into inserting number in the post?
    std::string line;
    while( getline( in, line ) )
        page << indent << "\t" << line << std::endl;

    in.close();
}

/**
 * Writes the page navigation html to a file
 * @param page        Target file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param article_pos Position of the current article for the page in the master index
 */
void blogator::output::page::Posts::writePageNavDiv( dto::Page &page,
                                                     const std::string &indent,
                                                     const size_t &article_pos ) const
{
    const bool is_first = article_pos == 0;
    const bool is_last  = article_pos == _index->_articles.size() - 1;

    const std::filesystem::path &first = _index->_articles.at( 0 )._paths.out_html;
    const std::filesystem::path &last  = _index->_articles.at( _index->_articles.size() - 1 )._paths.out_html;
    const std::filesystem::path &prev  = ( is_first ? first : _index->_articles.at( article_pos - 1 )._paths.out_html );
    const std::filesystem::path &next  = ( is_last  ? last  : _index->_articles.at( article_pos + 1 )._paths.out_html );

    page._out << indent << "\t"
              << ( is_first
                   ? html::createHyperlink( first, _options->_page_nav.first, "disabled" )
                   : html::createHyperlink( first, _options->_page_nav.first ) )
              << ( is_first
                   ? html::createHyperlink( prev, _options->_page_nav.backward, "disabled", "prev" )
                   : html::createHyperlink( prev, _options->_page_nav.backward, "", "prev" ) )
              << std::to_string( article_pos + 1 ) << _options->_page_nav.separator << std::to_string( _index->_articles.size() )
              << ( is_last
                   ? html::createHyperlink( next, _options->_page_nav.forward, "disabled", "next" )
                   : html::createHyperlink( next, _options->_page_nav.forward, "", "next" ) )
              << ( is_last
                   ? html::createHyperlink( last, _options->_page_nav.last, "disabled" )
                   : html::createHyperlink( last, _options->_page_nav.last ) )
              << std::endl;
}

/**
 * Writes the indexed date tree for a 'post' page
 * @param page        Target 'post' file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param article     Article DTO
 * @param article_pos Position of article in the master index
 */
void blogator::output::page::Posts::writeIndexDateTree( dto::Page &page,
                                                        const std::string &indent,
                                                        const blogator::dto::Article &article,
                                                        const size_t &article_pos ) const
{
    if( !_html_date_tree )
        throw exception::failed_expectation(
            "[output::page::Posts::writeIndexDateTree(..)] "
            "No Tree was created and yet the post tree writer method was called."
        );

    const auto &tree  = *_html_date_tree;
    auto year_line    = tree.date_line_map.find( article._datestamp._year );
    auto month_line   = tree.date_line_map.find( dto::DateStamp::concatYYYYMM( article._datestamp ) );
    auto article_line = tree.article_line_map.find( article_pos );

    size_t i = 0;
    for( const auto &line : tree.html._lines ) {
        if( year_line != tree.date_line_map.end() && year_line->second == i )
            page._out << indent << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
        else if( month_line != tree.date_line_map.end() && month_line->second == i )
            page._out << indent << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
        else if( article_line != tree.article_line_map.end() && article_line->second == i )
            page._out << indent << "\t" << html::writer::setHyperlinkClass( line, "current" ) << std::endl;
        else
            page._out << indent << "\t" << line << std::endl;
        ++i;
    }
}

/**
 * Write the indexed tag tree for a 'post' page
 * @param page        Target 'post' file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param article     Article DTO
 * @param article_pos Position of article in the master index
 */
void blogator::output::page::Posts::writeIndexTagTree( dto::Page &page,
                                                       const std::string &indent,
                                                       const blogator::dto::Article &article,
                                                       const size_t &article_pos ) const
{
    if( !_html_tag_tree )
        throw exception::failed_expectation(
            "[output::page::Posts::writeIndexTagTree(..)] "
            "No Tree was created and yet the post tree writer method was called."
        );

    const auto &tree        = *_html_tag_tree;
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
            page._out << indent << "\t" << html::writer::setInputCheckboxState( line, "checked" ) << std::endl;
            ++checkbox_line;
        } else if( link_line != article_link_lines.cend() && *link_line == i ) {
            page._out << indent << "\t" << html::writer::setHyperlinkClass( line, "current" ) << std::endl;
            ++link_line;
        } else {
            page._out << indent << "\t" << line << std::endl;
        }
        ++i;
    }
}