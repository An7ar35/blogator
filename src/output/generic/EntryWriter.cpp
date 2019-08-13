#include "EntryWriter.h"

#include <iostream>
#include <fstream>

#include "../../fs/fs.h"
#include "../../html/reader/reader.h"
#include "../../dto/Line.h"
#include "../../html/html.h"
#include "../../exception/file_access_failure.h"

/**
 * Constructor
 * @param global_options Global blogator options
 * @param entry_template Template DTO for the entry
 */
blogator::output::generic::EntryWriter::EntryWriter( std::shared_ptr<const dto::Options> global_options,
                                                     std::shared_ptr<const dto::Template> entry_template ) :
    _display( cli::MsgInterface::getInstance() ),
    _options( std::move( global_options ) ),
    _template( std::move( entry_template ) )
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
    bool  carriage_return   = true;

    //EARLY RETURN: when there's nothing to add/edit on the line
    if( !hasBlock() && !hasPath() ) {
        page._out << indent << *line._it << "\n";
        return;
    }

    while( hasBlock() && hasPath() ) {
        if( insert_its.block->first.col < insert_its.path->first.col ) { //do the block first
            if( carriage_return )
                page._out << indent;

            page._out << line._it->substr( col, insert_its.block->first.col - col );

            if( ( carriage_return = writeHtmlBlock( page, total_indent +"\t", insert_its.block->second, article ) ) )
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
        if( carriage_return )
            page._out << indent;

        page._out << line._it->substr( col, insert_its.block->first.col - col );

        if( ( carriage_return = writeHtmlBlock( page, total_indent + "\t", insert_its.block->second, article ) ) )
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
 * @return Line carriage return required
 */
bool blogator::output::generic::EntryWriter::writeHtmlBlock( dto::Page &page,
                                                             const std::string  &indent,
                                                             const std::string  &block_name,
                                                             const dto::Article &article ) const
{
    if( block_name == "post-number" ) {
        page._out << article._number;
        return false;

    } else if( block_name == "title" ) {
        page._out << article._heading;
        return false;

    } else if( block_name == "authors" ) {
        page._out  << "\n";
        for( const auto & author : article._authors ) {
            page._out << indent
                      << "<span class=\"author\">" << author << "</span>\n";
        }
        return true;

    } else if( block_name == "tags" ) {
        page._out << "\n";
        for( const auto & tag : article._tags ) {
            page._out << indent
                      << "<span class=\"tag\">" << tag << "</span>\n";
        }
        return true;

    } else if( block_name == "date-stamp" ) {
        page._out << html::createDateTime( article._datestamp, _options->_months );
        return false;

    } else if( block_name == "summary" ) {
        dto::Line line    = { article._summary.html._lines.cbegin(), 0 };
        auto      path_it = article._summary.path_write_pos.cbegin();

        page._out << "\n";
        while( line._it != article._summary.html._lines.cend() ) {
            page._out << indent << _options->_index.summary_pad_begin;
            writeSummaryLine( page, line, article, path_it );
            page._out << _options->_index.summary_pad_end << "\n";

            ++line;
        }

        return true;

    } else {
        _display.error(
            "[output::generic::EntryWriter::writeHtmlBlock(..)] "
            "HTML element class '" + block_name + "' not recognised."
        );
    }

    return false;
}

/**
 * Inserts any paths/blocks that are on a line
 * @param page    Output page DTO
 * @param line    Line DTO
 * @param article Article DTO for the entry
 * @param path_it Iterators to the path insertion points for summary
 */
void blogator::output::generic::EntryWriter::writeSummaryLine( dto::Page          &page,
                                                               const dto::Line    &line,
                                                               const dto::Article &article,
                                                               dto::ConsecutivePathPositions_t::const_iterator &path_it ) const
{
    auto hasPath  = [&]() { return ( path_it != article._summary.path_write_pos.cend() && path_it->first.line == line._num ); };

    std::string::size_type col = 0;

    //EARLY RETURN: when there's nothing to add/edit on the line
    if( !hasPath() ) {
        page._out << *line._it;
        return;
    }

    while( hasPath() ) {
        page._out << line._it->substr( col, path_it->first.col - col )
                  << fs::adaptRelPath( article._paths.src_html, page._abs_path, path_it->second.string() ).string();
        col =  path_it->first.col;
        ++path_it;
    }

    page._out << line._it->substr( col );
}
