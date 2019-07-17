#include "EntryWriter.h"

#include <iostream>
#include <fstream>

#include "../../fs/fs.h"
#include "../../html/reader/reader.h"
#include "../../dto/Line.h"
#include "../../html/html.h"

/**
 * Constructor
 * @param entry_template Template DTO for the entry
 * @param months         Lookup map for the months strings
 */
blogator::output::generic::EntryWriter::EntryWriter( std::shared_ptr<const dto::Template> entry_template,
                                                     const dto::Options::MonthStringLookup_t &months ) :
    _display( cli::MsgInterface::getInstance() ),
    _template( std::move( entry_template ) ),
    _months( months )
{}

/**
 * Write an entry for an Article to a page
 * @param page    Output Page DTO
 * @param indent  Space to place before the output lines (i.e.: html indent)
 * @param article Article DTO
 */
void blogator::output::generic::EntryWriter::write( blogator::dto::Page &page,
                                                    const std::string   &indent,
                                                    const dto::Article  &article ) const
{
    dto::Template::WritePosIterators insert_points = { _template->block_write_pos.cbegin(), _template->path_write_pos.cbegin() };
    dto::Line                        line          = { _template->html->_lines.cbegin(), 0 };

    while( line._it != _template->html->_lines.cend() ) {
        writeTemplateLine( page, indent + "\t", line, article, insert_points );
        ++line;
    }
}

/**
 * Gets the Template's type used for makign the entries
 * @return Template::Type
 */
blogator::dto::Template::Type blogator::output::generic::EntryWriter::getTemplateType() const {
    return _template->type;
}

/**
 * Inserts any paths/blocks that are on a line
 * @param page       Output page DTO
 * @param line       Line DTO
 * @param article    Article DTO for the entry
 * @param insert_its Iterators to the block/path insertion points for the template
 */
void blogator::output::generic::EntryWriter::writeTemplateLine( dto::Page &page,
                                                                const std::string &indent,
                                                                const dto::Line &line,
                                                                const dto::Article &article,
                                                                dto::Template::WritePosIterators &insert_its ) const
{
    auto hasBlock = [&]() { return ( insert_its.block != _template->block_write_pos.cend() && insert_its.block->first.line == line._num ); };
    auto hasPath  = [&]() { return ( insert_its.path  != _template->path_write_pos.cend()  && insert_its.path->first.line  == line._num ); };

    std::string::size_type col = 0;
    const auto sub_indent   = html::reader::getIndent( *line._it );
    const auto total_indent = indent + sub_indent;

    //EARLY RETURN: when there's nothing to add/edit on the line
    if( !hasBlock() && !hasPath() ) {
        page._out << indent << *line._it << "\n";
        return;
    }

    while( hasBlock() && hasPath() ) {
        if( insert_its.block->first.col < insert_its.path->first.col ) { //do the block first
            page._out << indent << line._it->substr( col, insert_its.block->first.col - col ) << "\n";
            writeHtmlBlock( page, total_indent +"\t", insert_its.block->second, article );
            page._out << total_indent;

            col = insert_its.block->first.col;
            ++insert_its.block;

        } else { //do the path first
            page._out << line._it->substr( col, insert_its.path->first.col - col )
                      << fs::adaptRelPath( _template->src, page._abs_path, insert_its.path->second.string() ).string();

            col =  insert_its.path->first.col;
            ++insert_its.path;
        }
    }

    while( hasBlock() ) { //just block(s) left to insert
        page._out << indent << line._it->substr( col, insert_its.block->first.col - col ) << "\n";
        writeHtmlBlock( page, total_indent + "\t", insert_its.block->second, article );
        page._out << total_indent;

        col = insert_its.block->first.col;
        ++insert_its.block;
    }

    while( hasPath() ) { //just path(s) left to insert
        page._out << line._it->substr( col, insert_its.path->first.col - col )
                  << fs::adaptRelPath( _template->src, page._abs_path, insert_its.path->second.string() ).string();

        col =  insert_its.path->first.col;
        ++insert_its.path;
    }

    page._out << line._it->substr( col ) << std::endl;
}

/**
 * Write the HTML block to the page
 * @param page       Output page DTO
 * @param indent     Space to place before the output lines (i.e.: html indent)
 * @param block_name HTML block's name (i.e.: the tag's class)
 * @param article    Article DTO
 */
void blogator::output::generic::EntryWriter::writeHtmlBlock( dto::Page &page,
                                                             const std::string  &indent,
                                                             const std::string  &block_name,
                                                             const dto::Article &article ) const
{
    if( block_name == "post-number" ) {
        page._out << indent << article._number << "\n";

    } else if( block_name == "heading" ) {
        page._out << indent << article._heading << "\n";

    } else if( block_name == "authors" ) {
        for( const auto & author : article._authors ) {
            page._out << indent
                      << "<span class=\"author\">" << author << "</span>\n";
        }

    } else if( block_name == "tags" ) {
        for( const auto & tag : article._tags ) {
            page._out << indent
                      << "<span class=\"tag\">" << tag << "</span>\n";
        }

    } else if( block_name == "date-stamp" ) {
        page._out << indent
                  << html::createDateTime( article._datestamp, _months ) << "\n";

    } else if( block_name == "summary" ) { //TODO
        _display.error(
            "[output::generic::EntryWriter::writeHtmlBlock(..)] "
            "//TODO: implement 'summary' insertion point"
        );
    } else {
        _display.error(
            "[output::generic::EntryWriter::writeHtmlBlock(..)] "
            "HTML element class '" + block_name + "' not recognised."
        );
    }
}