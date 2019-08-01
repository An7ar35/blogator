#include <utility>

#include "Landing.h"
#include <iostream>
#include <fstream>

#include "../../html/html.h"
#include "../../exception/file_access_failure.h"
#include "../../exception/failed_expectation.h"
#include "../../cli/MsgInterface.h"
#include "../../fs/fs.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::output::page::Landing::Landing( std::shared_ptr<const dto::Index>     master_index,
                                          std::shared_ptr<const dto::Templates> templates,
                                          std::shared_ptr<const dto::Options>   global_options ) :
    Page( std::move( master_index ),
               std::move( templates ),
               std::move( global_options ) ),
    _entry_maker( generic::EntryWriter( _options, _templates->_landing_entry ) ),
    _breadcrumb_parents( BreadCrumb_t() )
{}

/**
 * Initialize HTML landing page generation
 * @return Success
 * @throws exception::failed_expectation when insert points could not be found in the landing page template
 */
bool blogator::output::page::Landing::init() const {
    _display.begin( "Generating landing page", 1, "..." );

    if( _templates->_landing->block_write_pos.empty() ||  _templates->_landing_entry->block_write_pos.empty() ) {
        throw exception::failed_expectation(
            "Missing insertion points in one or more of the landing page templates "
            "(Found: page=" + std::to_string( _templates->_landing->block_write_pos.size() ) +
            ", entry=" + std::to_string( _templates->_landing_entry->block_write_pos.size() ) + ")."
        );
    }

    try {
        generateLandingPage();
    } catch( std::exception &e ) {
        _display.error( e.what() );
        return false;
    }

    _display.progress( "DONE" );
    return true;
}

/**
 * Generates the landing page
 * @throws std::invalid_argument when page path could not be generated for landing page
 */
void blogator::output::page::Landing::generateLandingPage() const {
    using dto::Page;
    using dto::Line;
    using dto::Template;

    Page page = { _options->_paths.root_dir / _options->_filenames.landing,
                  std::ofstream( _options->_paths.root_dir / _options->_filenames.landing ) };

    if( !page._out.is_open() )
        throw exception::file_access_failure(
            "File '" + page._abs_path.string() + "' could not be opened for writing."
        );

    page._out << _options->getSoftwareSignatureStr() << "\n";

    Template::WritePosIterators insert_it = { _templates->_landing->block_write_pos.cbegin(),
                                              _templates->_landing->path_write_pos.cbegin() };

    Line line = { _templates->_landing->html->_lines.cbegin(), 0 };

    while( line._it != _templates->_landing->html->_lines.cend() ) {
        writeTemplateLine( page, line, insert_it );
        ++line;
    }

    page._out.close();
}

/**
 * Inserts any paths/blocks that are on a line
 * @param page      Output page DTO
 * @param line      Line DTO
 * @param insert_it Iterators to the current block and path insertion point
 */
void blogator::output::page::Landing::writeTemplateLine( dto::Page       &page,
                                                         const dto::Line &line,
                                                         dto::Template::WritePosIterators &insert_it ) const
{
    auto hasBlock = [&]() { return ( insert_it.block != _templates->_landing->block_write_pos.cend() && insert_it.block->first.line == line._num ); };
    auto hasPath  = [&]() { return ( insert_it.path  != _templates->_landing->path_write_pos.cend()  && insert_it.path->first.line  == line._num ); };

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
            writeHtmlBlock( page, indent + "\t", insert_it.block->second );
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
        writeHtmlBlock( page, indent + "\t", insert_it.block->second );
        page._out << indent;

        col = insert_it.block->first.col;
        ++insert_it.block;
    }

    while( hasPath() ) { //just path(s) left to insert
        page._out << line._it->substr( col, insert_it.path->first.col - col )
                  << fs::adaptRelPath( _index->_paths.templates.landing, page._abs_path,
                                       insert_it.path->second.string() ).string();

        col = insert_it.path->first.col;
        ++insert_it.path;
    }

    page._out << line._it->substr( col ) << "\n";
}

/**
 * Writes the relevant HTML block at the given position
 * @param page        Output file
 * @param indent      Space to place before the output lines (i.e.: html indent)
 * @param block_class Block's class name
 */
void blogator::output::page::Landing::writeHtmlBlock( dto::Page &page,
                                                      const std::string &indent,
                                                      const std::string &block_class ) const
{
    if( block_class == "breadcrumb" ) {
        writeBreadcrumb( page, indent, _breadcrumb_parents, _options->_breadcrumb.start );
    } else if( block_class == "newest-posts" ) {
        writeNewestPosts( page, indent );
    } else if( block_class == "top-tags" ) {
        writeTopTags( page._out, indent );
    } else if( block_class == "top-authors" ) {
        writeTopAuthors( page._out, indent );
    } else if( block_class == "featured-posts" ) {
        writeFeatured( page, indent );
    } else {
        _display.error(
            "[output::page::Landing::writeHtmlBlock(..)] "
            "HTML Div class '" + block_class + "' not recognised."
        );
    }
}

/**
 * Write the top tags (as defined in the landing page options) to the landing page
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::Landing::writeTopTags( std::ofstream     &page,
                                                    const std::string &indent ) const
{
    page << indent << "<ul>\n";

    for( const auto &tag : _index->_indices.byTag.top ) {
        try {
            const auto       &specs     = _index->_indices.byTag.cats.at( tag );
            const std::string str       = tag + " (" + std::to_string( specs.article_indices.size() ) + ')';
            const auto       &file_name = specs.file_names.at( 0 );
            const auto        rel_path  = _options->_paths.index_tag_dir.lexically_relative( _options->_paths.root_dir );

            page << indent << "\t<li>"
                 << html::createHyperlink( rel_path / file_name , str )
                 << "</li>\n";

        } catch( std::out_of_range &e ) {
            if( _index->_indices.byTag.cats.find( tag ) == _index->_indices.byTag.cats.end() )
                _display.error(
                    "[output::page::Landing::writeTopTags(..)] "
                    "Tag '" + tag + "' does not seem to exist in the master index."
                );
            else
                _display.error(
                    "[output::page::Landing::writeTopTags(..)] "
                    "Tag '" + tag + "' doesn't not have any index page path(s) in its specifications."
                );
        }
    }

    page << indent << "</ul>\n";
}

/**
 * Write the top authors (as defined in the landing page options) to the landing page
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::Landing::writeTopAuthors( std::ofstream     &page,
                                                       const std::string &indent ) const
{
    page << indent << "<ul>\n";

    for( const auto &author : _index->_indices.byAuthor.top ) {
        try {
            const auto       &specs     = _index->_indices.byAuthor.cats.at( author );
            const std::string str       = author + " (" + std::to_string( specs.article_indices.size() ) + ')';
            const auto       &file_name = specs.file_names.at( 0 );
            const auto        rel_path  = _options->_paths.index_author_dir.lexically_relative( _options->_paths.root_dir );

            page << indent << "\t<li>"
                 << html::createHyperlink( rel_path / file_name , str )
                 << "</li>\n";

        } catch( std::out_of_range &e ) {
            if( _index->_indices.byTag.cats.find( author ) == _index->_indices.byTag.cats.end() )
                _display.error(
                    "[output::page::Landing::writeTopAuthors(..)] "
                    "Author '" + author + "' does not seem to exist in the master index."
                );
            else
                _display.error(
                    "[output::page::Landing::writeTopAuthors(..)] "
                    "Author '" + author + "' doesn't not have any index page path(s) in its specifications."
                );
        }
    }

    page << indent << "</ul>\n";
}

/**
 * Writes the newest posts entries (as defined in the landing page options) to the landing page
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::Landing::writeNewestPosts( dto::Page &page, const std::string &indent ) const {
    /**
     * [LAMBDA] When duplicates are not allowed on the landing page,
     *          check if an article is in the 'featured posts' list.
     */
    auto isFeatured = [&]( const dto::Article &article ) {
        if( !_options->_landing_page.duplicates && !_options->_landing_page.featured.empty() ) {
            const auto rel_path = article._paths.src_html.lexically_relative( _options->_paths.source_dir );
            return ( _options->_landing_page.featured.find( rel_path ) != _options->_landing_page.featured.cend() );
        }

        return false;
    };

    auto article_it = _index->_articles.cbegin();
    size_t i        = 0;

    while( article_it != _index->_articles.cend() && i < _options->_landing_page.most_recent ) {

        if( !isFeatured( *article_it ) ) {
            auto href = _options->_folders.posts.root / article_it->_paths.out_html;

            page._out << indent << "<a href=\"" << href.string() << "\">\n";
            writeEntry( page, indent, *article_it );
            page._out << indent << "</a>" << std::endl;

            ++i;
        }

        ++article_it;
    }
}

/**
 * Write the featured articles (as defined in the landing page options) to the landing page
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::Landing::writeFeatured( dto::Page &page, const std::string &indent ) const {
    for( const auto &article : _index->_featured ) {
        auto href = _options->_folders.posts.root / article._paths.out_html;

        page._out << indent << "<a href=\"" << href.string() << "\">\n";
        writeEntry( page, indent, article );
        page._out << indent << "</a>" << std::endl;
    }
}

/**
 * Writes the index entry for the landing page's section
 * @param page    Target file
 * @param indent  Space to place before the output line (i.e.: html indent)
 * @param article Article who's entry to write
 */
void blogator::output::page::Landing::writeEntry( blogator::dto::Page &page,
                                                  const std::string   &indent,
                                                  const dto::Article  &article ) const {

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
}
