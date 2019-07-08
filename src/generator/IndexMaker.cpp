#include "IndexMaker.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "../fs/fs.h"
#include "../html/html.h"
#include "../exception/file_access_failure.h"
#include "../exception/failed_expectation.h"
#include "../cli/MsgInterface.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::generator::IndexMaker::IndexMaker( std::shared_ptr<const dto::Index>    master_index,
                                             std::shared_ptr<const dto::Template> templates,
                                             std::shared_ptr<const dto::Options>  global_options ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) ),
    _display( cli::MsgInterface::getInstance() )
{
    _total_jobs = _master_index->_indices.byDate.page_count
                  + _master_index->_indices.byTag.page_count
                  + _master_index->_indices.byAuthor.page_count;
}

/**
 * Initialize HTML index pages generation
 * @return Success
 * @throws exception::failed_expectation when insert points could not be found in 1 or more of the index templates
 */
bool blogator::generator::IndexMaker::init() {
    try {
        auto index_write_positions       = dto::Template::getConsecutiveWritePositions( _templates->_index.div_write_pos );
        auto index_entry_write_positions = dto::Template::getConsecutiveWritePositions( _templates->_index_entry.div_write_pos );

        if( index_write_positions.empty() || index_entry_write_positions.empty() ) {
            throw exception::failed_expectation(
                "Missing insertion points in one or more index templates "
                "(Found: index page=" + std::to_string( index_write_positions.size() ) +
                ", index entry=" + std::to_string( index_entry_write_positions.size() ) + ")."
            );
        }

        generateDateIndexPages( index_write_positions, index_entry_write_positions );

        //Tag list index
        if( _options->_index.index_by_tag ) {
            auto tag_list_write_positions = dto::Template::getConsecutiveWritePositions( _templates->_tag_list.div_write_pos );

            if( tag_list_write_positions.empty() )
                throw exception::failed_expectation( "Missing insertion points in the tag list template." );

            generateTagIndexPages( index_write_positions, index_entry_write_positions );
            writeTagListPage( tag_list_write_positions, index_entry_write_positions );
        }

        //Author list index
        if( _options->_index.index_by_author ) {
            auto author_list_write_positions = dto::Template::getConsecutiveWritePositions( _templates->_author_list.div_write_pos );

            if( author_list_write_positions.empty() )
                throw exception::failed_expectation( "Missing insertion points in the author list template." );

            generateAuthorIndexPages( index_write_positions, index_entry_write_positions );
            writeAuthorListPage( author_list_write_positions, index_entry_write_positions );
        }

        _display.progress( "DONE" );

    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

/**
 * Generates 'by date' index pages
 * @param page_insert_points Consecutive insert points in the index template
 * @param item_insert_points Item template insertion point locations
 * @return Success
 * @throws std::invalid_argument          when page path could not be generated for index by date
 * @throws exception::file_access_failure when access issues where encountered
 */
void blogator::generator::IndexMaker::generateDateIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                              const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const
{
    auto &display     = cli::MsgInterface::getInstance();
    auto article_it   = _master_index->_articles.cbegin();
    auto page_path_it = _master_index->_indices.byDate.page_file_names.cbegin();

    while( article_it != _master_index->_articles.cend() ) {
        if( article_it == _master_index->_articles.cbegin() )
            _display.begin( "Generating indices", _total_jobs, _options->_folders.index.by_date.filename() / article_it->_paths.out_html );
        else
            _display.progress( _options->_folders.index.by_date.filename() / article_it->_paths.out_html );

        if( page_path_it == _master_index->_indices.byDate.page_file_names.cend() )
            throw std::invalid_argument(
                "Page paths found for the index pages are not sufficient for the number of articles."
            );

        auto abs_file_path = _options->_paths.index_date_dir / *page_path_it;

        if( std::filesystem::exists( abs_file_path ) )
            throw exception::file_access_failure(
                "File '" + abs_file_path.string() + "' already exists. Possible files with duplicate names in source folder structure."
            );

        auto page_out = std::ofstream( abs_file_path );

        if( !page_out.is_open() )
            throw exception::file_access_failure(
                "File '" + abs_file_path.string() + "' could not be opened for writing."
            );

        writeDateIndexPage( page_out, page_path_it, article_it, page_insert_points, item_insert_points );

        page_out.close();
        ++page_path_it;
    }
}

/**
 * Generates the master tag list index page
 * @param page_insert_points Consecutive insert points in the index list template
 * @param item_insert_points Item template insertion point locations
 * @throws exception::file_access_failure when access issues where encountered
 */
void blogator::generator::IndexMaker::writeTagListPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                        const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const
{
    auto abs_file_path = _options->_paths.index_tag_dir / _templates->_tag_list.file_name;
    auto page_out      = std::ofstream( abs_file_path );

    if( !page_out.is_open() )
        throw exception::file_access_failure(
            "File '" + abs_file_path.string() + "' could not be opened for writing."
        );

    _display.progress( _options->_folders.index.by_tag.filename() / _templates->_tag_list.file_name );

    page_out << _options->BLOGATOR_SIGNATURE << "\n";

    size_t curr_src_line = 0;
    auto   insert_point  = page_insert_points.cbegin();

    for( const auto &template_line : _templates->_tag_list.html->_lines ) {
        if( insert_point != page_insert_points.cend() && insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );
            page_out << template_line.substr( 0, insert_point->first.col ) << std::endl;

            if( insert_point->second == "breadcrumb" ) {
                writeIndexListBreadcrumb( page_out, indent + "\t", _options->_breadcrumb.by_tag );

            } else if( insert_point->second == "tag-list" ) {
                writeTagList( page_out, indent + "\t" );

            } else if( insert_point->second == "tag-list-hierarchy" ) {
                writeTagListHierarchy( page_out, indent + "\t" );

            } else {
                std::cerr << "[generator::IndexMaker::writeTagListPage(..)] "
                          << "HTML Div class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            page_out << indent << template_line.substr( insert_point->first.col ) << std::endl;
            ++insert_point;

        } else {
            page_out << template_line << "\n";
        }

        ++curr_src_line;
    }

    page_out.close();
}

/**
 * Generates the master author list index page
 * @param page_insert_points Consecutive insert points in the index list template
 * @param item_insert_points Item template insertion point locations
 * @throws exception::file_access_failure when access issues where encountered
 */
void blogator::generator::IndexMaker::writeAuthorListPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                           const dto::Template::ConsecutiveWritePositions_t &item_insert_points )
{
    auto abs_file_path = _options->_paths.index_author_dir / _templates->_author_list.file_name;
    auto page_out      = std::ofstream( abs_file_path );

    if( !page_out.is_open() )
        throw exception::file_access_failure(
            "File '" + abs_file_path.string() + "' could not be opened for writing."
        );

    _display.progress( _options->_folders.index.by_author.filename() / _templates->_author_list.file_name );

    page_out << _options->BLOGATOR_SIGNATURE << "\n";

    size_t curr_src_line = 0;
    auto   insert_point  = page_insert_points.cbegin();

    for( const auto &template_line : _templates->_author_list.html->_lines ) {
        if( insert_point != page_insert_points.cend() && insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );
            page_out << template_line.substr( 0, insert_point->first.col ) << std::endl;

            if( insert_point->second == "breadcrumb" ) {
                writeIndexListBreadcrumb( page_out, indent + "\t", _options->_breadcrumb.by_author );

            } else if( insert_point->second == "author-list" ) {
                writeAuthorList( page_out, indent + "\t" );

            } else {
                std::cerr << "[generator::IndexMaker::writeAuthorListPage(..)] "
                          << "HTML Div class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            page_out << indent << template_line.substr( insert_point->first.col ) << std::endl;
            ++insert_point;

        } else {
            page_out << template_line << "\n";
        }

        ++curr_src_line;
    }

    page_out.close();
}

/**
 * Generates 'by tag' index pages
 * @param page_insert_points Consecutive insert points in the index template
 * @param item_insert_points Item template insertion point locations
 * @throws std::invalid_argument          when page path could not be generated for tag index
 * @throws exception::file_access_failure when access issues where encountered
 */
void blogator::generator::IndexMaker::generateTagIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                             const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const
{
    auto tag_it = _master_index->_indices.byTag.tags.cbegin();
    while( tag_it != _master_index->_indices.byTag.tags.cend() ) {
        auto article_i_it = tag_it->second.article_indices.cbegin();
        auto page_path_it = tag_it->second.file_names.cbegin();

        while( article_i_it != tag_it->second.article_indices.cend() ) {
            if( page_path_it == tag_it->second.file_names.cend() )
                throw std::invalid_argument(
                    "Page paths found for the index pages are not sufficient for the number of articles."
                );

            auto abs_file_path = _options->_paths.index_tag_dir / *page_path_it;

            _display.progress( _options->_folders.index.by_tag.filename() / *page_path_it );

            if( std::filesystem::exists( abs_file_path ) )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' already exists."
                );

            auto page_out = std::ofstream( abs_file_path );

            if( !page_out.is_open() )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' could not be opened for writing."
                );

            ParentPage parent_page = { _options->_breadcrumb.by_tag, _templates->_tag_list.file_name };
            writeCategoryIndexPage( page_out, parent_page, tag_it, page_path_it, article_i_it,
                                    page_insert_points, item_insert_points );

            page_out.close();
            ++page_path_it;
        }

        ++tag_it;
    }
}

/**
 * Generates 'by author' index pages
 * @param page_insert_points Consecutive insert points in the index template
 * @param item_insert_points Item template insertion point locations
 * @throws std::invalid_argument          when page path could not be generated for author index
 * @throws exception::file_access_failure when access issues where encountered
 */
void blogator::generator::IndexMaker::generateAuthorIndexPages( const dto::Template::ConsecutiveWritePositions_t &page_insert_points, //TODO
                                                                const dto::Template::ConsecutiveWritePositions_t &item_insert_points )
{
    auto author_it = _master_index->_indices.byAuthor.authors.cbegin();
    while( author_it != _master_index->_indices.byAuthor.authors.cend() ) {
        auto article_i_it = author_it->second.article_indices.cbegin();
        auto page_path_it = author_it->second.file_names.cbegin();

        while( article_i_it != author_it->second.article_indices.cend() ) {
            if( page_path_it == author_it->second.file_names.cend() )
                throw std::invalid_argument(
                    "Page paths found for the index pages are not sufficient for the number of articles."
                );

            auto abs_file_path = _options->_paths.index_author_dir / *page_path_it;

            _display.progress( _options->_folders.index.by_author.filename() / *page_path_it );

            if( std::filesystem::exists( abs_file_path ) )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' already exists."
                );

            auto page_out = std::ofstream( abs_file_path );

            if( !page_out.is_open() )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' could not be opened for writing."
                );

            ParentPage parent_page = { _options->_breadcrumb.by_author, _templates->_author_list.file_name };
            writeCategoryIndexPage( page_out, parent_page, author_it, page_path_it, article_i_it,
                                    page_insert_points, item_insert_points );

            page_out.close();
            ++page_path_it;
        }
        ++author_it;
    }
}

/**
 * Writes a page of entries
 * @param page               Page file
 * @param page_it            Iterator to current page path
 * @param article_it         Iterator to the current article to be processed
 * @param page_insert_points Index template insertion point locations
 * @param item_insert_points Item template insertion point locations
 */
void blogator::generator::IndexMaker::writeDateIndexPage( std::ofstream &page,
                                                          const dto::Index::PagePaths_t::const_iterator &path_it,
                                                          dto::Index::Articles_t::const_iterator &article_it,
                                                          const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                          const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const
{
    page << _options->BLOGATOR_SIGNATURE << "\n";

    size_t entry_counter = 0;
    size_t curr_src_line = 0;
    auto   insert_point  = page_insert_points.cbegin();
    auto   &paths        = _master_index->_indices.byDate.page_file_names;

    for( const auto &template_line : _templates->_index.html->_lines ) {
        if( insert_point != page_insert_points.cend() && insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );
            page << template_line.substr( 0, insert_point->first.col ) << std::endl;

            if( insert_point->second == "breadcrumb" ) {
                auto page_num = _options->_breadcrumb.page + std::to_string( std::distance( paths.cbegin(), path_it ) + 1 );
                writeIndexPageBreadcrumb( page, indent + "\t", _options->_breadcrumb.by_date, "0.html", page_num );

            } else if( insert_point->second == "page-nav" ) {
                writePageNavDiv( page, indent + "\t", paths, path_it );

            } else if( insert_point->second == "index-entries" ) {
                while( article_it != _master_index->_articles.cend() && entry_counter < _options->_index.items_per_page ) {
                    auto abs_path = _options->_paths.index_date_dir / path_it->parent_path();
                    auto rel_path = ( _options->_paths.posts_dir / article_it->_paths.out_html ).lexically_relative( abs_path );

                    writeIndexEntry( page, indent + "\t", *article_it, rel_path, item_insert_points );

                    ++article_it;
                    ++entry_counter;
                }

            } else {
                std::cerr << "[generator::IndexMaker::writeDateIndexPage(..)] "
                          << "HTML Div class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            page << indent << template_line.substr( insert_point->first.col ) << std::endl;
            ++insert_point;

        } else {
            page << template_line << "\n";
        }

        ++curr_src_line;
    }
}

/**
 * Writes a page of entries
 * @param page               Page file
 * @param parent             Parent page info in the blog's structure
 * @param cat_it             Iterator to the current category
 * @param page_path_it       Iterator to the current page path
 * @param article_i_it       Index position of the current Article to be processed
 * @param page_insert_points Index template insertion point locations
 * @param item_insert_points Item template insertion point locations
 */
void blogator::generator::IndexMaker::writeCategoryIndexPage( std::ofstream &page,
                                                              const ParentPage &parent,
                                                              const dto::Index::ListIndexPaths_t::const_iterator &cat_it,
                                                              const dto::Index::PagePaths_t::const_iterator &page_path_it,
                                                              std::vector<size_t>::const_iterator &article_i_it,
                                                              const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                              const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const
{
    page << _options->BLOGATOR_SIGNATURE << "\n";

    size_t entry_counter   = 0;
    size_t curr_src_line   = 0;
    auto   insert_point    = page_insert_points.cbegin();
    auto  &article_indices = cat_it->second.article_indices;

    for( const auto &template_line : _templates->_index.html->_lines ) {
        if( insert_point != page_insert_points.cend() && insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );

            page << template_line.substr( 0, insert_point->first.col ) << std::endl;
            if( insert_point->second == "breadcrumb" ) {
                writeIndexPageBreadcrumb( page, indent + "\t", parent._name, parent._path, cat_it->first );

            } else if( insert_point->second == "page-nav" ) {
                writePageNavDiv( page, indent + "\t", cat_it->second.file_names, page_path_it );

            } else if( insert_point->second == "index-entries" ) {

                while( article_i_it != cat_it->second.article_indices.cend() &&
                       entry_counter < _options->_index.items_per_page )
                {
                    auto abs_path = _options->_paths.index_date_dir / page_path_it->parent_path();
                    const auto &article  = _master_index->_articles.at( *article_i_it );
                    auto rel_path = ( _options->_paths.posts_dir / article._paths.out_html ).lexically_relative( abs_path );

                    writeIndexEntry( page, indent + "\t", article, rel_path, item_insert_points );

                    ++article_i_it;
                    ++entry_counter;
                }

            } else {
                std::cerr << "[generator::IndexMaker::writeCategoryIndexPage(..)] "
                          << "HTML Div class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            page << indent << template_line.substr( insert_point->first.col ) << std::endl;
            ++insert_point;

        } else {
            page << template_line << "\n";
        }

        ++curr_src_line;
    }
}

/**
 * Write an index entry for an article to an index page
 * @param page               Target file
 * @param indent             Space to place before the output lines (i.e.: html indent)
 * @param article            Article DTO
 * @param post_href_path     Relative HTML post file path
 * @param item_insert_points Item template insertion point locations
 */
void blogator::generator::IndexMaker::writeIndexEntry( std::ofstream &page,
                                                       const std::string &indent,
                                                       const blogator::dto::Article &article,
                                                       const std::filesystem::path &post_href_path,
                                                       const dto::Template::ConsecutiveWritePositions_t &item_insert_points ) const
{
    page << indent << "<a href=\"" << post_href_path.string() << "\">\n";

    if( article._paths.entry_html.empty() ) {
        writeTemplateIndexEntry( page, indent + "\t", article, *_templates->_index_entry.html, item_insert_points );

    } else {
        try {
            auto entry = dto::Template::IndexEntry();
            entry.html = fs::importHTML( article._paths.entry_html );

            html::reader::findInsertPositions( *entry.html, entry.div_write_pos );
            auto write_positions = dto::Template::getConsecutiveWritePositions( entry.div_write_pos );

            writeTemplateIndexEntry( page, indent + "\t", article, *entry.html, write_positions );

        } catch( exception::file_access_failure &e ) {
            std::cerr << e.what() << std::endl;
            writeTemplateIndexEntry( page, indent + "\t", article, *_templates->_index_entry.html, item_insert_points );
        }
    }

    page << indent << "</a>" << std::endl;
}

/**
 * Write an entry based on the template to an index page
 * @param page          Target file
 * @param indent        Space to place before the output lines (i.e.: html indent)
 * @param article       Article DTO
 * @param src_html      Source HTMl for template
 * @param insert_points Item template insertion point locations
 */
void blogator::generator::IndexMaker::writeTemplateIndexEntry( std::ofstream &page,
                                                               const std::string &indent,
                                                               const dto::Article &article,
                                                               const dto::HTML &src_html,
                                                               const dto::Template::ConsecutiveWritePositions_t &insert_points ) const {
    size_t curr_src_line = 0;
    auto   insert_point  = insert_points.cbegin();

    for( const auto & template_line : src_html._lines ) {
        if( insert_point != insert_points.cend() && insert_point->first.line == curr_src_line ) {
            const auto sub_indent = html::reader::getIndent( template_line );

            page << indent << template_line.substr( 0, insert_point->first.col ) << "\n";

            if( insert_point->second == "post-number" ) {
                page << indent << sub_indent << "\t" << article._number << "\n";

            } else if( insert_point->second == "heading" ) {
                page << indent << sub_indent << "\t" << article._heading << "\n";

            } else if( insert_point->second == "authors" ) {
                page << indent << sub_indent << "\t";
                size_t i = 1;
                for( const auto & author : article._authors ) {
                    page << author;
                    if( i < article._authors.size() )
                        page << ", ";
                    ++i;
                }

            } else if( insert_point->second == "tags" ) {
                for( const auto & tag : article._tags ) {
                    page << indent << sub_indent
                         << "\t<span class=\"tag\">" << tag << "</span>\n";
                }

            } else if( insert_point->second == "date-stamp" ) {
                page << indent << sub_indent << "\t"
                     << html::createDateTime( article._datestamp, _options->_months ) << "\n";

            } else if( insert_point->second == "summary" ) { //TODO
                std::cerr << "[generator::IndexMaker::writeTemplateIndexEntry(..)] "
                          << "TODO: implement 'summary' insertion point"
                          << std::endl;

            } else {
                std::cerr << "[generator::IndexMaker::writeIndexEntry(..)] "
                          << "HTML element class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            page << indent << sub_indent
                 << template_line.substr( insert_point->first.col )
                 << std::endl;

            ++insert_point;

        } else {
            page << indent << template_line << "\n";
        }

        ++curr_src_line;
    }
}

/**
 * Writes the page navigation html to a file
 * @param page    Target file
 * @param indent  Space to place before the output line (i.e.: html indent)
 * @param paths   Collection of page paths
 * @param path_it Iterator to the current page path where the navigation is being written to
 */
void blogator::generator::IndexMaker::writePageNavDiv( std::ofstream &page,
                                                       const std::string &indent,
                                                       const dto::Index::PagePaths_t &paths,
                                                       const dto::Index::PagePaths_t::const_iterator &path_it ) const
{
    const bool   is_first    = path_it == paths.cbegin();
    const bool   is_last     = path_it == std::prev( paths.cend() );
    const size_t page_number = ( path_it - paths.cbegin() ) + 1;

    page << indent
         << ( is_first
              ? html::createHyperlink( *paths.cbegin(), _options->_page_nav.first, "disabled" )
              : html::createHyperlink( *paths.cbegin(), _options->_page_nav.first ) )
         << ( is_first
              ? html::createHyperlink( *paths.cbegin(), _options->_page_nav.backward, "disabled" )
              : html::createHyperlink( *std::prev( path_it ), _options->_page_nav.backward, "disabled" ) )
         << std::to_string( page_number ) << _options->_page_nav.separator << std::to_string( paths.size() )
         << ( is_last
              ? html::createHyperlink( *std::prev( paths.cend() ), _options->_page_nav.forward, "disabled" )
              : html::createHyperlink( *std::next( path_it ), _options->_page_nav.forward ) )
         << ( is_last
              ? html::createHyperlink( *std::prev( paths.cend() ), _options->_page_nav.last, "disabled" )
              : html::createHyperlink( *std::prev( paths.cend() ), _options->_page_nav.last ) )
         << std::endl;
}

/**
 * Writes a complete flat-level tag list
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::IndexMaker::writeTagList( std::ofstream &page, const std::string &indent ) const {
    if( !_master_index->_indices.byTag.tags.empty() ) {
        auto tag_it = _master_index->_indices.byTag.tags.cbegin();
        page << indent << "<ul>\n";

        while( tag_it != _master_index->_indices.byTag.tags.cend() ) {
            page << indent << "\t<li>"
                 << html::createHyperlink( tag_it->second.file_names.front(), tag_it->first ) << "</li>\n";
            ++tag_it;
        }

        page << indent << "</ul>" << std::endl;
    }
}

/**
 * Writes complete tag list categorised alphabetically
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::IndexMaker::writeTagListHierarchy( std::ofstream &page,
                                                             const std::string &indent ) const
{
    if( !_master_index->_indices.byTag.tags.empty() ) {
        auto tag_it = _master_index->_indices.byTag.tags.cbegin();
        page << indent << "<ul>\n";

        while( tag_it != _master_index->_indices.byTag.tags.cend() ) {
            const char sublist_letter = std::tolower( tag_it->first.at( 0 ) );

            page << indent << "\t<li>" << char( std::toupper( sublist_letter ) ) << "\n"
                 << indent << "\t\t<ul>\n";

            while( tag_it != _master_index->_indices.byTag.tags.cend() &&
                   char( std::tolower( sublist_letter ) ) == std::tolower( tag_it->first.at( 0 )  ) )
            {
                page << indent << "\t\t\t<li>"
                     << html::createHyperlink( tag_it->second.file_names.front(), tag_it->first ) << "</li>\n";
                ++tag_it;
            }

            page << indent << "\t\t</ul>\n"
                 << indent << "\t</li>\n";
        }

        page << indent << "</ul>" << std::endl;
    }
}

/**
 * Writes a complete flat-level author list
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::IndexMaker::writeAuthorList( std::ofstream &page,
                                                       const std::string &indent ) const
{
    if( !_master_index->_indices.byAuthor.authors.empty() ) {
        auto tag_it = _master_index->_indices.byAuthor.authors.cbegin();
        page << indent << "<ul>\n";

        while( tag_it != _master_index->_indices.byAuthor.authors.cend() ) {
            page << indent << "\t<li>"
                 << html::createHyperlink( tag_it->second.file_names.front(), tag_it->first ) << "</li>\n";
            ++tag_it;
        }

        page << indent << "</ul>" << std::endl;
    }
}

/**
 * Writes the breadcrumb html to an index category file
 * @param page        Target file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param parent_name Name of parent page
 * @param parent_path Path of parent page
 * @param page_name   Name of current page
 */
void blogator::generator::IndexMaker::writeIndexPageBreadcrumb( std::ofstream &page,
                                                                const std::string &indent,
                                                                const std::string &parent_name,
                                                                const std::filesystem::path &parent_path,
                                                                const std::string &page_name ) const
{
    const auto  landing_page = std::filesystem::path( "../.." / _templates->_start.file_name );

    page << indent << "<ul>\n"
         << indent << "\t<li>" << html::createHyperlink( landing_page, _options->_breadcrumb.start ) << "</li>\n"
         << indent << "\t<li>" << html::createHyperlink( parent_path, parent_name ) << "</li>\n"
         << indent << "\t<li>" << page_name << "</li>\n"
         << indent << "</ul>\n";
}

/**
 * Writes the breadcrumb html to any index lists (categories) file
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::IndexMaker::writeIndexListBreadcrumb( std::ofstream &page,
                                                                const std::string &indent,
                                                                const std::string &list_name ) const
{
    const auto landing_page = std::filesystem::path( "../.." / _templates->_start.file_name );

    page << indent << "<ul>\n"
         << indent << "\t<li>" << html::createHyperlink( landing_page, _options->_breadcrumb.start ) << "</li>\n"
         << indent << "\t<li>" << list_name << "</li>\n"
         << indent << "</ul>\n";
}
