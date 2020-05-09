#include "CategoryLister.h"

#include <utility>
#include <iostream>

#include "../../exception/file_access_failure.h"
#include "../../html/html.h"
#include "../../html/reader/reader.h"
#include "../../fs/fs.h"
#include "../../cli/MsgInterface.h"
#include "../../html/editor/editor.h"

/**
 *
 * @param global_options Global blogator options
 * @param articles       Articles
 * @param page_template  Page Template to use
 * @param entry_maker    EntryMaker instance to use
 */
blogator::output::generic::CategoryLister::CategoryLister( std::shared_ptr<const dto::Options> global_options,
                                                           const dto::Index::Articles_t &articles,
                                                           const dto::Template          &page_template,
                                                           generic::EntryWriter         entry_maker ) :
    Lister(
        std::move( global_options ),
        articles,
        page_template
    ),
    _entry_maker( std::move( entry_maker ) )
{}

/**
 * Writes categorised index pages
 * @param cat_index  Index::Category index
 * @param breadcrumb Parent Breadcrumb
 * @param dir_path   Absolute path to the output directory of the index files
 */
void blogator::output::generic::CategoryLister::write( const dto::Index::Category  &cat_index,
                                                       const BreadCrumb_t          &breadcrumb,
                                                       const std::filesystem::path &dir_path )
{
    auto &display = cli::MsgInterface::getInstance();

    _breadcrumb_parents = breadcrumb;

    auto cat_it = cat_index.cats.cbegin();
    while( cat_it != cat_index.cats.cend() ) {
        auto article_i_it = cat_it->second.article_indices.cbegin();
        auto page_path_it = cat_it->second.file_names.cbegin();

        while( article_i_it != cat_it->second.article_indices.cend() ) {
            _entry_counter = 0; //reset on new page

            if( page_path_it == cat_it->second.file_names.cend() )
                throw std::invalid_argument(
                    "Page paths found for categorised index pages are not sufficient for the number of articles. "
                    "(dir: '" + dir_path.filename().string() + "', cat = '" + cat_it->first + "')"
                );

            auto abs_file_path = dir_path / *page_path_it;
            display.progress( dir_path.stem() / *page_path_it );

            if( std::filesystem::exists( abs_file_path ) )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' already exists."
                );

            dto::Page page = { abs_file_path, std::ofstream( abs_file_path ) };

            if( !page._out.is_open() )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' could not be opened for writing: " + strerror(errno)
            );

            page._out << _options->getSoftwareSignatureStr() << "\n";

            dto::Template::WritePosIterators insert_points = { _template.block_write_pos.cbegin(), _template.path_write_pos.cbegin() };
            dto::Line                        line          = { _template.html->_lines.cbegin(), 0 };

            while( line._it != _template.html->_lines.cend() ) {
                writeTemplateLine( page, line, cat_it, page_path_it, article_i_it, insert_points );
                ++line;
            }

            page._out.close();
            ++page_path_it;
        }

        ++cat_it;
    }
}

/**
 * Inserts any paths/block that are on a line
 * @param page         Output page DTO
 * @param line         Line DTO
 * @param cat_it       Iterator to the categories
 * @param page_path_it Iterator to the current page path
 * @param article_i_it Iterator to the article index position
 * @param insert_it    Iterators to the block/path insertion points for the template
 */
void blogator::output::generic::CategoryLister::writeTemplateLine(
    dto::Page         &page,
    const dto::Line   &line,
    const dto::Index::ListIndexPaths_t::const_iterator &cat_it,
    const dto::Index::PagePaths_t::const_iterator      &page_path_it,
    std::vector<size_t>::const_iterator                &article_i_it,
    dto::Template::WritePosIterators                   &insert_it )
{
    auto hasBlock = [&]() { return ( insert_it.block != _template.block_write_pos.cend() && insert_it.block->first.line == line._num ); };
    auto hasPath  = [&]() { return ( insert_it.path  != _template.path_write_pos.cend()  && insert_it.path->first.line  == line._num ); };

    std::string::size_type col = 0;
    const auto indent = html::reader::getIndent( *line._it );
    bool carriage_return = false;

    //EARLY RETURN: when there's nothing to add/edit on the line
    if( !hasBlock() && !hasPath() ) {
        page._out << *line._it << "\n";
        return;
    }

    while( hasBlock() && hasPath() ) {
        if( insert_it.block->first.col < insert_it.path->first.col ) { //do the block first
            if( carriage_return )
                page._out << indent;

            page._out << line._it->substr( col, insert_it.block->first.col - col );

            if( ( carriage_return = writeHtmlBlock( page, indent, insert_it.block->second, cat_it, page_path_it, article_i_it ) ) )
                page._out << indent;

            col = insert_it.block->first.col;
            ++insert_it.block;

        } else { //do the path first
            page._out << line._it->substr( col, insert_it.path->first.col - col )
                      << html::encodePathToURL( fs::adaptRelPath( _template.src, page._abs_path, insert_it.path->second.string() ).string() );

            col = insert_it.path->first.col;
            ++insert_it.path;
        }
    }

    while( hasBlock() ) { //just block(s) left to insert
        if( carriage_return )
            page._out << indent;

        page._out << line._it->substr( col, insert_it.block->first.col - col );

        if( ( carriage_return = writeHtmlBlock( page, indent, insert_it.block->second, cat_it, page_path_it, article_i_it ) ) )
            page._out << indent;

        col = insert_it.block->first.col;
        ++insert_it.block;
    }

    while( hasPath() ) { //just path(s) left to insert
        page._out << line._it->substr( col, insert_it.path->first.col - col )
                  << html::encodePathToURL( fs::adaptRelPath( _template.src, page._abs_path, insert_it.path->second.string() ).string() );

        col = insert_it.path->first.col;
        ++insert_it.path;
    }

    page._out << line._it->substr( col ) << "\n";
}

/**
 * Write the HTML block to the page
 * @param page         Output page DTO
 * @param indent       Space to place before the output lines (i.e.: html indent)
 * @param block_name   HTML block's name (i.e.: the tag's class)
 * @param cat_it       Iterator to the categories
 * @param page_path_it Iterator to the current page path
 * @param article_i_it Iterator to the article index position
 * @return Line carriage return required
 */
bool blogator::output::generic::CategoryLister::writeHtmlBlock(
    blogator::dto::Page &page,
    const std::string   &indent,
    const std::string   &block_name,
    const dto::Index::ListIndexPaths_t::const_iterator &cat_it,
    const dto::Index::PagePaths_t::const_iterator      &page_path_it,
    std::vector<size_t>::const_iterator                &article_i_it )
{
    if( block_name == "page-name" ) {
        page._out << cat_it->first;
        return false;

    } else if( block_name == "breadcrumb" ) {
        page._out  << "\n";
        writeBreadcrumb( page, indent + "\t", _breadcrumb_parents, cat_it->first );
        return true;

    } else if( block_name == "page-nav" ) {
        page._out  << "\n";
        writePageNavDiv( page, indent + "\t", cat_it->second.file_names, page_path_it );
        return true;

    } else if( block_name == "index-entries" ) {
        page._out  << "\n";

        while( article_i_it != cat_it->second.article_indices.cend() &&
               _entry_counter < _options->_index.items_per_page )
        {
            const auto &article  = _articles.at( *article_i_it );
            auto abs_path        = page._abs_path.parent_path();
            auto rel_path        = ( _options->_paths.posts_dir / article._paths.out_html ).lexically_relative( abs_path );

            writeIndexEntry( page, indent, article, rel_path );

            ++article_i_it;
            ++_entry_counter;
        }

        return true;

    } else {
        _display.error(
            "[output::generic::CategoryLister::writeHtmlBlock(..)] "
            "HTML Div class '" + block_name + "' not recognised."
        );
    }

    return false;
}

/**
 * Write an index entry for an article to an index page
 * @param page               Target file
 * @param indent             Space to place before the output lines (i.e.: html indent)
 * @param article            Article DTO
 * @param post_href_path     Relative HTML post file path
 */
void blogator::output::generic::CategoryLister::writeIndexEntry(
    dto::Page &page,
    const std::string  &indent,
    const dto::Article &article,
    const std::filesystem::path &post_href_path ) const
{
    if( article._is_featured && !_options->_index.featured_css_class.empty() )
        page._out << indent << "<a class=\"" << _options->_index.featured_css_class << "\" href=\"" << post_href_path.string() << "\">\n";
    else
        page._out << indent << "<a href=\"" << post_href_path.string() << "\">\n";

    if( article._cust_index_entry ) {
        auto custom_entry_maker = generic::EntryWriter( _options, article._cust_index_entry );
        custom_entry_maker.write( page, indent, article );

    } else {
        _entry_maker.write( page, indent, article );
    }

    page._out << indent << "</a>" << std::endl;
}