#include <utility>

#include "Posts.h"

#include <iostream>
#include <stack>
#include <functional>
#include <variant>

#include "../../fs/fs.h"
#include "../../html/html.h"
#include "../../exception/failed_expectation.h"
#include "../../exception/file_access_failure.h"
#include "../../cli/MsgInterface.h"
#include "../helper/OrderedPostInsertion.h"

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
    _breadcrumb_parents( { { _options->_breadcrumb.start, "../index.html" },
                           { _options->_breadcrumb.by_date, "../index/by_date/0.html" } } )
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
    const auto total_articles = '/' + std::to_string( _index->_articles.size() );

    auto &insert_points = _templates->_post->block_write_pos;

    if( insert_points.empty() )
        throw exception::failed_expectation( "No insertion points found in post template." );

    try {
        const auto css_insert_line = _templates->_post->html->findLineOfTag( "</head>" );
        size_t     curr_article_i  = 0;

        for( const auto &article : _index->_articles ) {
            if( curr_article_i == 0 )
                _display.begin( "Generating post pages", _index->_articles.size(), std::to_string( curr_article_i + 1 ) + total_articles );
            else
                _display.progress( std::to_string( curr_article_i + 1 ) + total_articles );

            const auto css_path = copyStylesheet( article );
            const auto html_out = ( _options->_paths.posts_dir / article._paths.out_html );

            if( std::filesystem::exists( html_out ) )
                throw exception::file_access_failure(
                    "File '" + html_out.string() + "' already exists. Possible files with duplicate names in source folder structure."
                );

            dto::Page page = { html_out, std::ofstream( html_out ) };

            if( !page._out.is_open() )
                throw exception::file_access_failure(
                    "File '" + html_out.string() + "' could not be opened for writing."
                );

            page._out << _options->getSoftwareSignatureStr() << "\n";

            dto::Template::WritePosIterators insert_it = { _templates->_post->block_write_pos.cbegin(),
                                                           _templates->_post->path_write_pos.cbegin() };

            dto::Line line      = { _templates->_post->html->_lines.cbegin(), 0 };
            PageInfo  page_info = { article, curr_article_i, css_insert_line, css_path };

            while( line._it != _templates->_post->html->_lines.cend() ) {
                writeTemplateLine( page, line, page_info, insert_it );
                ++line;
            }

            page._out.close();
            ++curr_article_i;
        }

    } catch( std::exception &e ) {
        _display.error( e.what() );
        return false;
    }

    _display.progress( "DONE" );
    return true;
}

/**
 * Copies and renames the custom stylesheet for an article (if any) to the post output folder
 * @param article Article DTO
 * @return Final filename of copied stylesheet
 */
std::filesystem::path blogator::output::page::Posts::copyStylesheet( const dto::Article &article ) const {
    if( article._paths.css.empty() )
        return std::filesystem::path();

    auto css_name   = article._paths.out_html.filename().stem().string() + article._paths.css.extension().string();
    auto target_css = _options->_paths.posts_dir / css_name;
    _display.debug( "Copying custom stylesheet: " + article._paths.css.string() + " -> " + target_css.string() );

    try {
        std::filesystem::copy_file( article._paths.css, target_css );
    } catch( std::exception &e ) {
        _display.error( std::string( e.what() ) + ": " + strerror(errno) );
    }

    return target_css.filename();
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
        page._out << html::createStylesheetLink( page_info.css_path ) << std::endl;

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
                      << html::encodePathToURL( fs::adaptRelPath( _index->_paths.templates.landing, page._abs_path, insert_it.path->second.string() ).string() );

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
                  << html::encodePathToURL( fs::adaptRelPath( _index->_paths.templates.landing, page._abs_path, insert_it.path->second.string() ).string() );

        col = insert_it.path->first.col;
        ++insert_it.path;
    }

    page._out << line._it->substr( col ) << "\n";
}

/**
 * Writes the relevant HTML block at the given position on the template
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
        writeContentDiv( page, page_info.article );
    } else if( block_class == "index-pane-dates" ) {
        writeIndexDateTree( page, indent, page_info.article, page_info.article_i );
    } else {
        _display.error(
            "[output::page::Posts::init(..)] "
            "HTML Div class '" + block_class + "' not recognised."
        );
    }
}

/**
 * Writes the source article lines to a file
 * @param page        Output file
 * @param article     Article DTO
 * @throws blogator::exception::file_access_failure when source file could not be opened/imported
 */
void blogator::output::page::Posts::writeContentDiv( dto::Page &page,
                                                     const dto::Article &article ) const
{
    if( !_options->_posts.adapt_rel_paths && _options->_posts.toc.generate_toc < 1 ) {
        writeContentVerbatim( page, article._paths.src_html );
    } else {
        writeContentModified( page, article );
    }
}

/**
 * Write the source content verbatim to the target page
 * @param page        Output file
 * @param source_path Post's HTML source path
 * @throws blogator::exception::file_access_failure when source file could not be opened/imported
 */
void blogator::output::page::Posts::writeContentVerbatim( dto::Page &page,
                                                          const std::filesystem::path &source_path ) const
{
    auto in = std::ifstream( source_path );
    if( !in.is_open() )
        throw exception::file_access_failure( "Cannot read source file '" + source_path.string() + "'." );

    std::string line;
    while( getline( in, line ) )
        page._out << line << std::endl;

    in.close();
}

/**
 * Writes the source article lines with modifications to a file
 * @param page        Output file
 * @param article     Article DTO
 * @throws blogator::exception::file_access_failure when source file could not be opened/imported
 */
void blogator::output::page::Posts::writeContentModified( blogator::dto::Page &page,
                                                          const blogator::dto::Article &article ) const
{
    using helper::OrderedPostInsertion;

    auto      html             = fs::importHTML( article._paths.src_html );
    auto      insert_positions = helper::OrderedPostInsertion();
    auto      path_positions   = dto::Templates::extractRelativePaths( *html );
    dto::Line line             = { html->_lines.cbegin(), 0 };

    const bool toc_flag      = ( article._toc != nullptr );
    const bool rel_path_flag = ( _options->_posts.adapt_rel_paths && !path_positions.empty() );

    if( rel_path_flag ) {
        for( const auto &path : path_positions )
            insert_positions.pushPath( path );
    }

    if( toc_flag ) {
        article._toc->finaliseToC(); //<-- generate all the numbering/depth info for the headings
        for( const auto &heading : article._toc->headings() )
            insert_positions.pushHeading( heading );

        for( const auto &toc : article._toc->tocPositions() )
            insert_positions.pushToC( toc );
    }

    /* LAMBDA Helpers */
    auto insertPath = [&]( const OrderedPostInsertion::PathPosition_t &path_pos,
                           const std::string::size_type &column )
    {
        page._out << line._it->substr( column, path_pos.first.col - column )
                  << html::encodePathToURL( fs::adaptRelPath( article._paths.src_html, page._abs_path, path_pos.second.string() ).string() );
        return path_pos.first.col;
    };

    auto insertTOC  = [&]( const OrderedPostInsertion::ToCPosition_t &toc_pos,
                           const std::string &indent,
                           const std::string::size_type &column )
    {
        page._out << line._it->substr( column, toc_pos.col - column ) << "\n";
        writeTocTree( page, indent + "\t", *article._toc );
        page._out << indent;
        return toc_pos.col;
    };

    auto modifyHeading = [&]( const OrderedPostInsertion::HeadingPosition_t &mod_info,
                              const std::string::size_type &column,
                              bool add_numbering )
    {
        auto id_col  = mod_info.first.col - column;
        auto str_col = mod_info.second.str_pos.col - id_col;

        page._out << line._it->substr( column, id_col )
                  << " id=\"" << mod_info.second.printID() << "\"";

        if( add_numbering )
                  page._out << line._it->substr( id_col, str_col )
                            << mod_info.second.printNumbering() << " ";

        return ( add_numbering ? mod_info.second.str_pos.col : mod_info.first.col );
    };
    /* ======= */

    page._out << "\n";

    while( line._it != html->_lines.cend() ) {
        const auto indent = html::reader::getIndent( *line._it );
        std::string::size_type column = 0;

        while( !insert_positions.empty() ) {
            const auto & pos = OrderedPostInsertion::getInsertPosition( insert_positions.top() );

            if( pos.line != line._num )
                break;

            switch( insert_positions.top().first ) {
                case helper::InsertPositionType::PATH:
                {
                    const auto &e = std::get<OrderedPostInsertion::PathPosition_t>( insert_positions.top().second );
                    column = insertPath( e, column );
                    break;
                }

                case helper::InsertPositionType::TOC:
                {
                    const auto &e = std::get<OrderedPostInsertion::ToCPosition_t>( insert_positions.top().second );
                    column = insertTOC( e, indent, column );
                    break;
                }

                case helper::InsertPositionType::HEADING:
                {
                    const auto &e = std::get<OrderedPostInsertion::HeadingPosition_t>( insert_positions.top().second );
                    column = modifyHeading( e, column, _options->_posts.toc.numbering );
                    break;
                }
            }

            insert_positions.pop();
        }

        page._out << line._it->substr( column ) << "\n";
        ++line;
    }
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
    const bool is_latest   = article_pos == 0;
    const bool is_earliest = article_pos == _index->_articles.size() - 1;

    const std::filesystem::path &latest   = _index->_articles.at( 0 )._paths.out_html;
    const std::filesystem::path &earliest = _index->_articles.at( _index->_articles.size() - 1 )._paths.out_html;
    const std::filesystem::path &prev  = ( is_latest ? latest : _index->_articles.at( article_pos - 1 )._paths.out_html );
    const std::filesystem::path &next  = ( is_earliest ? earliest : _index->_articles.at( article_pos + 1 )._paths.out_html );

    page._out << indent << "<ul>\n"
              << indent << "\t<li class=\"first\">"
                        << ( is_earliest
                             ? html::createHyperlink( earliest, _options->_page_nav.first, "disabled" )
                             : html::createHyperlink( earliest, _options->_page_nav.first ) )
                        << "</li>\n"
              << indent << "\t<li class=\"prev\">"
                        << ( is_earliest
                             ? html::createHyperlink( next, _options->_page_nav.backward, "disabled", "prev" )
                             : html::createHyperlink( next, _options->_page_nav.backward, "", "prev" ) )
                        << "</li>\n"
              << indent << "\t<li class=\"curr\">"
                        << std::to_string( _index->_articles.size() - article_pos ) << _options->_page_nav.separator << std::to_string( _index->_articles.size() )
                        << "</li>\n"
              << indent << "\t<li class=\"next\">"
                        << ( is_latest
                             ? html::createHyperlink( prev, _options->_page_nav.forward, "disabled", "next" )
                             : html::createHyperlink( prev, _options->_page_nav.forward, "", "next" ) )
                        << "</li>\n"
              << indent << "\t<li class=\"last\">"
                        << ( is_latest
                             ? html::createHyperlink( latest, _options->_page_nav.last, "disabled" )
                             : html::createHyperlink( latest, _options->_page_nav.last ) )
                        << "</li>\n"
              << indent << "</ul>"
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
            page._out << indent << "\t" << html::writer::setHyperlinkClass( line, "current-post" ) << std::endl;
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
            page._out << indent << "\t" << html::writer::setHyperlinkClass( line, "current-post" ) << std::endl;
            ++link_line;
        } else {
            page._out << indent << "\t" << line << std::endl;
        }
        ++i;
    }
}

/**
 * Writes the ToC tree for a 'post' page
 * @param page   Target 'post' file
 * @param indent Space to place before the output line (i.e.: html indent)
 * @param toc    Article's Table of Content
 */
void blogator::output::page::Posts::writeTocTree( dto::Page &page,
                                                  const std::string &indent,
                                                  const dto::TableOfContents &toc ) const
{
    auto html       = dto::HTML();
    auto curr_depth = 1;

    html::writer::openTree( html );

    for( const auto &heading : toc.headings() ) {

        auto target_depth = heading.second.depth();

        while( curr_depth > target_depth ) {
            html::writer::closeSubTree( html, curr_depth - 1 );
            --curr_depth;
        }

        while( curr_depth < target_depth ) {
            html::writer::openSubTree( html, curr_depth );
            ++curr_depth;
        }

        //depth == prev_depth
        if( _options->_posts.toc.numbering )
            html::writer::addLeaf( html,
                                   html::createHyperlink( "#" + heading.second.printID(), heading.second.print() ),
                                   target_depth
            );
        else
            html::writer::addLeaf( html,
                                   html::createHyperlink( "#" + heading.second.printID(), heading.second.str ),
                                   target_depth
            );

        curr_depth = target_depth;
    }

    while( curr_depth > 1 ) {
        html::writer::closeSubTree( html, curr_depth-- );
    }

    html::writer::closeTree( html );

    if( !_options->_posts.toc.heading.empty() )
        page._out << indent << _options->_posts.toc.heading << std::endl;

    for( const auto &line : html._lines )
        page._out << indent << line << "\n";
}