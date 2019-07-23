#include "ChronoIndexLister.h"

#include "../../cli/MsgInterface.h"
#include "../../exception/file_access_failure.h"
#include "../../fs/fs.h"
#include "../../html/reader/reader.h"
#include "../../exception/failed_expectation.h"

/**
 * Constructor
 * @param global_options Global blogator options
 * @param articles       Articles
 * @param page_template  Page Template to use
 * @param entry_maker    EntryMaker instance to use
 */
blogator::output::generic::ChronoIndexLister::ChronoIndexLister( std::shared_ptr<const dto::Options> global_options,
                                                                 const dto::Index::Articles_t &articles,
                                                                 const dto::Template          &page_template,
                                                                 output::generic::EntryWriter entry_maker ) :
    Lister(
        std::move( global_options ),
        articles,
        page_template
    ),
    _entry_maker( std::move( entry_maker ) )
{}

/**
 * Writes the chronological index pages
 * @param chrono_index Chronological index information
 * @param breadcrumb   Parent Breadcrumb
 * @param dir_path     Absolute path to the output directory of the index files
 */
void blogator::output::generic::ChronoIndexLister::write(
    const dto::Index::Indices::ChronoIndex           &chrono_index,
    const output::abstract::Breadcrumb::BreadCrumb_t &breadcrumb,
    const std::filesystem::path                      &dir_path )
{
    auto &display = cli::MsgInterface::getInstance();

    _breadcrumb_parents = breadcrumb;
    _breadcrumb_parents.emplace_back( std::make_pair( _options->_breadcrumb.by_date, chrono_index.file_names.front() ) );

    auto article_it    = _articles.cbegin();
    auto page_path_it  = chrono_index.file_names.cbegin();

    while( article_it != _articles.cend() ) {
        _page_num     += 1;
        _entry_counter = 0;

        if( page_path_it == chrono_index.file_names.cend() )
            throw std::invalid_argument(
                "Page paths found for index pages are not sufficient for the number of articles. "
                "(dir: " + dir_path.filename().string() + ")"
            );

        auto abs_file_path = dir_path / *page_path_it;
        if( article_it != _articles.cbegin() )
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
            writeTemplateLine( page, line, chrono_index.file_names, page_path_it, article_it, insert_points );
            ++line;
        }

        page._out.close();
        ++page_path_it;
    }
}

/**
 * Inserts any paths/block that are on a line
 * @param page         Output page DTO
 * @param line         Line DTO
 * @param page_paths   List of pages for the index
 * @param page_path_it Current page path iterator
 * @param article_it   Current article iterator
 * @param insert_it    Iterators to the block/path insertion points for the template
 */
void blogator::output::generic::ChronoIndexLister::writeTemplateLine(
    dto::Page       &page,
    const dto::Line &line,
    const dto::Index::PagePaths_t                  &page_paths,
    const dto::Index::PagePaths_t::const_iterator  &page_path_it,
    dto::Index::Articles_t::const_iterator         &article_it,
    dto::Template::WritePosIterators               &insert_it  )
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
            writeHtmlBlock( page, indent, insert_it.block->second, page_paths, page_path_it, article_it );
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
        writeHtmlBlock( page, indent, insert_it.block->second, page_paths, page_path_it, article_it );
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
 * Writes the relevant HTML block at the given position
 * @param page         Output file
 * @param indent       Space to place before the output lines (i.e.: html indent)
 * @param block_name   Block's class name
 * @param page_paths   List of pages for the index
 * @param page_path_it Current page path iterator
 * @param article_it   Current article iterator
 */
void blogator::output::generic::ChronoIndexLister::writeHtmlBlock(
    dto::Page &page,
    const std::string  &indent,
    const std::string  &block_name,
    const dto::Index::PagePaths_t                  &page_paths,
    const dto::Index::PagePaths_t::const_iterator  &page_path_it,
    dto::Index::Articles_t::const_iterator         &article_it )
{
    if( block_name == "page-name" ) {
        page._out << _options->_breadcrumb.by_date;

    } else if( block_name == "breadcrumb" ) {
        auto page_desc = _options->_breadcrumb.page + std::to_string( _page_num );
        writeBreadcrumb( page, indent + "\t", _breadcrumb_parents, page_desc );

    } else if( block_name == "page-nav" ) {
        writePageNavDiv( page, indent + "\t", page_paths, page_path_it );

    } else if( block_name == "index-entries" ) {

        while( article_it != _articles.cend() && _entry_counter < _options->_index.items_per_page ) {
            auto abs_path        = page._abs_path.parent_path();
            auto rel_path        = ( _options->_paths.posts_dir / article_it->_paths.out_html ).lexically_relative( abs_path );

            writeIndexEntry( page, indent, *article_it, rel_path );

            ++article_it;
            ++_entry_counter;
        }

    } else {
        _display.error(
            "[output::generic::ChronoIndexLister::writeHtmlBlock(..)] "
            "HTML Div class '" + block_name + "' not recognised."
        );
    }
}

/**
 * Write an index entry for an article to an index page
 * @param page               Target file
 * @param indent             Space to place before the output lines (i.e.: html indent)
 * @param article            Article DTO
 * @param post_href_path     Relative HTML post file path
 */
void blogator::output::generic::ChronoIndexLister::writeIndexEntry(
    dto::Page &page,
    const std::string  &indent,
    const dto::Article &article,
    const std::filesystem::path &post_href_path ) const
{
    page._out << indent << "<a href=\"" << post_href_path.string() << "\">\n";

    if( article._paths.entry_html.empty() ) {
        _entry_maker.write( page, indent, article );

    } else {
        try {
            auto entry             = std::make_shared<dto::Template>( _entry_maker.getTemplateType() );
            entry->src             = article._paths.entry_html;
            entry->html            = fs::importHTML( entry->src );
            entry->block_write_pos = html::reader::getConsecutiveWritePositions( *entry->html, entry->block_classes );
            entry->path_write_pos  = dto::Templates::extractRelativePaths( *entry->html );

            fs::checkTemplateRelPaths( *entry );
            auto custom_entry_maker = generic::EntryWriter( _options, entry );

            custom_entry_maker.write( page, indent, article );

        } catch( exception::file_access_failure &e ) {
            _display.error( e.what() );
            _entry_maker.write( page, indent, article );
        }
    }

    page._out << indent << "</a>" << std::endl;
}