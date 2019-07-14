#include "IndexList.h"

#include <utility>
#include <iostream>

#include "../../exception/file_access_failure.h"
#include "../../html/reader/reader.h"
#include "../../fs/fs.h"
#include "../../html/html.h"

/**
 *
 * @param global_options     Global blogator options
 * @param entry_template     Index list Template to use
 * @param breadcrumb         Parent breadcrumbs
 * @param page_name          Index list page name (for the breadcrumb)
 */
blogator::output::abstract::IndexList::IndexList( std::shared_ptr<const dto::Options> global_options,
                                                  const dto::Template &entry_template,
                                                  BreadCrumb_t        breadcrumb,
                                                  std::string         page_name ) :
    _options( std::move( global_options ) ),
    _template( entry_template ),
    _breadcrumb_parents( std::move( breadcrumb ) ),
    _breadcrumb_page_str( std::move( page_name ) )
{}

/**
 * Write a index list to a page
 * @param page    Output Page DTO
 */
void blogator::output::abstract::IndexList::write( dto::Page &page ) const {
    if( !page._out.is_open() )
        throw exception::file_access_failure(
            "File '" + page._abs_path.string() + "' could not be opened for writing."
        );

    page._out << _options->BLOGATOR_SIGNATURE << "\n";

    dto::Template::WritePosIterators insert_points = { _template.block_write_pos.cbegin(), _template.path_write_pos.cbegin() };
    dto::Line                        line          = { _template.html->_lines.cbegin(), 0 };

    while( line._it != _template.html->_lines.cend() ) {
        writeTemplateLine( page, line, insert_points );
        ++line;
    }

    page._out.close();
}

/**
 *
 * @param page
 * @param line
 * @param insert_it
 */
void blogator::output::abstract::IndexList::writeTemplateLine( dto::Page       &page,
                                                               const dto::Line &line,
                                                               dto::Template::WritePosIterators &insert_it ) const
{
    auto hasBlock = [&]() { return ( insert_it.block != _template.block_write_pos.cend() && insert_it.block->first.line == line._num ); };
    auto hasPath  = [&]() { return ( insert_it.path  != _template.path_write_pos.cend()  && insert_it.path->first.line  == line._num ); };

    std::string::size_type col = 0;
    const auto indent = html::reader::getIndent( *line._it );

    //EARLY RETURN: when there's nothing to add/edit on the line
    if( !hasBlock() && !hasPath() ) {
        page._out << *line._it << "\n";
        return;
    }

    while( hasBlock() && hasPath() ) {
        if( insert_it.block->first.col < insert_it.path->first.col ) { //do the block first
            page._out << line._it->substr( col, insert_it.block->first.col - col ) << "\n";
            writeHtmlBlock( page, indent, insert_it.block->second );
            page._out << indent;

            col = insert_it.block->first.col;
            ++insert_it.block;

        } else { //do the path first
            page._out << line._it->substr( col, insert_it.path->first.col - col )
                      << fs::adaptRelPath( _template.src, page._abs_path, insert_it.path->second.string() ).string();

            col = insert_it.path->first.col;
            ++insert_it.path;
        }
    }

    while( hasBlock() ) { //just block(s) left to insert
        page._out << line._it->substr( col, insert_it.block->first.col - col ) << "\n";
        writeHtmlBlock( page, indent, insert_it.block->second );
        page._out << indent;

        col = insert_it.block->first.col;
        ++insert_it.block;
    }

    while( hasPath() ) { //just path(s) left to insert
        page._out << line._it->substr( col, insert_it.path->first.col - col )
                  << fs::adaptRelPath( _template.src, page._abs_path,
                                       insert_it.path->second.string() ).string();

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
 */
void blogator::output::abstract::IndexList::writeHtmlBlock( blogator::dto::Page &page,
                                                           const std::string   &indent,
                                                           const std::string   &block_name ) const
{
    if( block_name == "breadcrumb" ) {
        writeBreadcrumb( page, indent + "\t", _breadcrumb_parents, _breadcrumb_page_str );

    } else if( block_name == "index-list-flat" ) {
        writeFlatList( page, indent + "\t" );

    } else if( block_name == "index-list-hierarchy") {
        writeHierarchyList( page, indent + "\t" );

    } else {
        std::cerr << "[output::abstract::IndexList::writeHtmlBlock(..)] "
                  << "HTML Div class '" << block_name << "' not recognised."
                  << std::endl;
    }
}