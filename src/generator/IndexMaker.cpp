#include "IndexMaker.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "../fs/fs.h"
#include "../html/html.h"
#include "../exception/file_access_failure.h"

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
    _options( std::move( global_options ) )
{}

/**
 * Initialize HTML index pages generation
 * @param tag_index    Master Index of tags to articles
 * @return Success
 */
bool blogator::generator::IndexMaker::init()
{
    auto index_write_positions    = dto::Template::getConsecutiveWritePositions( _templates->_index.div_write_pos );
    auto tag_list_write_positions = dto::Template::getConsecutiveWritePositions( _templates->_tag_list.div_write_pos );
    //TODO check insert points exists then throw proper error is none found
    auto index_byDate_ok  = generateDateIndexPages( index_write_positions );
    auto index_byTag_ok   = generateTagIndexPages( index_write_positions );
    auto index_TagList_ok = writeTagListPage( tag_list_write_positions );

    return ( index_byDate_ok && index_byTag_ok && index_TagList_ok );
}

/**
 * Generates 'by date' index pages
 * @param insert_points Consecutive insert points in the index template
 * @return Success
 * @throws std::invalid_argument when page path could not be generated for index by date
 */
bool blogator::generator::IndexMaker::generateDateIndexPages( const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    try {
        auto article_it   = _master_index->_articles.cbegin();
        auto page_path_it = _master_index->_indices.byDate.page_file_names.cbegin();

        while( article_it != _master_index->_articles.cend() ) {
            if( page_path_it == _master_index->_indices.byDate.page_file_names.cend() )
                throw std::invalid_argument(
                    "Page paths found for the index pages are not sufficient for the number of articles."
                );

            auto abs_file_path = _options->_paths.index_dir / _options->_paths.index_sub_dirs.by_date / *page_path_it;

            if( std::filesystem::exists( abs_file_path ) )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' already exists. Possible files with duplicate names in source folder structure."
                );

            auto page_out = std::ofstream( abs_file_path );

            if( !page_out.is_open() )
                throw exception::file_access_failure(
                    "File '" + abs_file_path.string() + "' could not be opened for writing."
                );

            writeDateIndexPage( page_out, page_path_it, article_it, insert_points );

            page_out.close();
            ++page_path_it;
        }

        return true;
    } catch( exception::file_access_failure &e ) {
        std::cerr << "[generator::IndexMaker::generateDateIndexPages(..)] " << e.what() << std::endl;
    }

    return false;
}

/**
 * Generates the master tag list index page
 * @param insert_points Consecutive insert points in the index list template
 * @return
 */
bool blogator::generator::IndexMaker::writeTagListPage( const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    for( auto e : insert_points )
        std::cout << "found " << e.second << " at (" << e.first.line << ", " << e.first.col << ")" << std::endl;
    //TODO index list page writer
    return false;
}

/**
 * Generates 'by tag' index pages
 * @param insert_points Consecutive insert points in the index template
 * @return Success
 * @throws std::invalid_argument when page path could not be generated for index by date
 */
bool blogator::generator::IndexMaker::generateTagIndexPages( const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    try {
        auto tag_it = _master_index->_indices.byTag.tags.cbegin();
        while( tag_it != _master_index->_indices.byTag.tags.cend() ) {
            const auto abs_dir_path = _options->_paths.index_dir / _options->_paths.index_sub_dirs.by_tag;
            auto article_i_it = tag_it->second.article_indices.cbegin();
            auto page_path_it = tag_it->second.file_names.cbegin();

            while( article_i_it != tag_it->second.article_indices.cend() ) {
                if( page_path_it == tag_it->second.file_names.cend() )
                    throw std::invalid_argument(
                        "Page paths found for the index pages are not sufficient for the number of articles."
                    );

                auto abs_file_path = abs_dir_path / *page_path_it;

                if( std::filesystem::exists( abs_file_path ) )
                    throw exception::file_access_failure(
                        "File '" + abs_file_path.string() + "' already exists."
                    );

                auto page_out = std::ofstream( abs_file_path );

                if( !page_out.is_open() )
                    throw exception::file_access_failure(
                        "File '" + abs_file_path.string() + "' could not be opened for writing."
                    );

                writeTagIndexPage( page_out, tag_it, page_path_it, article_i_it, insert_points );

                page_out.close();
                ++page_path_it;
            }
            ++tag_it;
        }

        return true;

    } catch( exception::file_access_failure &e ) {
        std::cerr << "[generator::IndexMaker::generateTagIndexPages(..)] " << e.what() << std::endl;
    }

    return false;
}

/**
 * Writes a page of entries
 * @param page          Page file
 * @param page_it       Iterator to current page path
 * @param article_it    Iterator to the current article to be processed
 * @param insert_points Index template insertion point locations
 */
void blogator::generator::IndexMaker::writeDateIndexPage( std::ofstream &page,
                                                          const dto::Index::PagePaths_t::const_iterator &path_it,
                                                          dto::Index::Articles_t::const_iterator &article_it,
                                                          const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    size_t entry_counter = 0;
    size_t curr_src_line = 0;
    auto   insert_point  = insert_points.cbegin();
    auto   &paths        = _master_index->_indices.byDate.page_file_names;

    for( const auto &template_line : _templates->_index.html->_lines ) {
        if( insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );
            page << template_line.substr( 0, insert_point->first.col ) << std::endl;

            if( insert_point->second == "breadcrumb" ) {
                writeDateIndexBreadcrumb( page, indent + "\t", std::distance( paths.cbegin(), path_it ) + 1 );

            } else if( insert_point->second == "page-nav" ) {
                writePageNavDiv( page, indent + "\t", paths, path_it );

            } else if( insert_point->second == "index-entries" ) {
                while( article_it != _master_index->_articles.cend() && entry_counter < _options->_index.items_per_page ) {
                    auto abs_path = _options->_paths.index_dir / _options->_paths.index_sub_dirs.by_date / path_it->parent_path();
                    auto rel_path = ( _options->_paths.posts_dir / article_it->_paths.out_html ).lexically_relative( abs_path );

                    writeIndexEntry( page, indent + "\t", _templates->_months, *article_it, rel_path );

                    ++article_it;
                    ++entry_counter;
                }

                page << indent << template_line.substr( insert_point->first.col ) << std::endl;

            } else {
                std::cerr << "[generator::IndexMaker::writeDateIndexPage(..)] "
                          << "HTML Div class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            ++insert_point;

        } else {
            page << template_line << "\n";
        }

        ++curr_src_line;
    }
}

/**
 * Writes a page of entries
 * @param page            Page file
 * @param tag_it          Iterator to the current tag
 * @param page_path_it    Iterator to the current page path
 * @param article_i_it    Index position of the current Article to be processed
 * @param insert_points   Index template insertion point locations
 */
void blogator::generator::IndexMaker::writeTagIndexPage( std::ofstream &page,
                                                         const dto::Index::TagIndexPaths_t::const_iterator &tag_it,
                                                         const dto::Index::PagePaths_t::const_iterator &page_path_it,
                                                         std::vector<size_t>::const_iterator &article_i_it,
                                                         const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    size_t entry_counter   = 0;
    size_t curr_src_line   = 0;
    auto   insert_point    = insert_points.cbegin();
    auto  &article_indices = tag_it->second.article_indices;

    for( const auto &template_line : _templates->_index.html->_lines ) {
        if( insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );

            page << template_line.substr( 0, insert_point->first.col ) << std::endl;
            if( insert_point->second == "breadcrumb" ) {
                writeTagIndexBreadcrumb( page, indent + "\t", tag_it->first );

            } else if( insert_point->second == "page-nav" ) {
                writePageNavDiv( page, indent + "\t", tag_it->second.file_names, page_path_it );

            } else if( insert_point->second == "index-entries" ) {

                while( article_i_it != tag_it->second.article_indices.cend() &&
                       entry_counter < _options->_index.items_per_page )
                {
                    auto abs_path = _options->_paths.index_dir / _options->_paths.index_sub_dirs.by_date / page_path_it->parent_path();
                    const auto &article  = _master_index->_articles.at( *article_i_it );
                    auto rel_path = ( _options->_paths.posts_dir / article._paths.out_html ).lexically_relative( abs_path );

                    writeIndexEntry( page, indent + "\t", _templates->_months, article, rel_path );

                    ++article_i_it;
                    ++entry_counter;
                }

                page << indent << template_line.substr( insert_point->first.col ) << std::endl;

            } else {
                std::cerr << "[generator::IndexMaker::writeTagIndexPage(..)] "
                          << "HTML Div class '" << insert_point->second << "' not recognised."
                          << std::endl;
            }

            ++insert_point;

        } else {
            page << template_line << "\n";
        }

        ++curr_src_line;
    }
}

/**
 * Write an index entry for an article to an index page
 * @param file           Target file
 * @param indent         Space to place before the output lines (i.e.: html indent)
 * @param months         Month string lookup map
 * @param article        Article DTO
 * @param post_href_path Relative HTML post file path
 */
void blogator::generator::IndexMaker::writeIndexEntry( std::ofstream &file,
                                                       const std::string &indent,
                                                       const blogator::dto::Template::MonthStringLookup_t &months,
                                                       const blogator::dto::Article &article,
                                                       const std::filesystem::path &post_href_path ) const
{
    /**
     * [LAMBDA] Write a standard entry to an index page
     */
    auto writeStandardEntry = [&]() {
        file << indent << "\t<span class=\"index-entry-heading\">" << article._heading << "</span>\n"
             << indent << "\t" << html::createDateTime( article._datestamp, months ) << "\n"
             << indent << "\t<div class=\"index-entry-tags\">\n";
        for( const auto &tag : article._tags ) {
            file << indent << "\t\t<span class=\"tag\">" << tag << "</span>\n";
        }
        file << indent << "\t</div>\n";
    };

    /**
     * [LAMBDA] Write a custom entry to an index page
     * @throws exception::file_access_failure when custom index entry source could not be opened
     */
    auto writeCustomEntry = [&]( const std::filesystem::path &cust_entry_path ) {
        std::string   line;
        std::ifstream in( cust_entry_path );

        if( !in.is_open() )
            throw exception::file_access_failure( "Could not open: " + cust_entry_path.string() );

        while( getline( in, line ) ) {
            file << indent << "\t" << line << "\n";
        }

        in.close();
    };


    file << indent << "<a href=\"" << post_href_path.string() << "\">\n";

    if( article._paths.entry_html.empty() ) {
        writeStandardEntry();
    } else {
        try {
            writeCustomEntry( article._paths.entry_html );
        } catch( exception::file_access_failure &e ) {
            std::cerr << e.what() << std::endl;
            writeStandardEntry();
        }
    }

    file << indent << "</a>" << std::endl;
}

/**
 * Writes the page navigation html to a file
 * @param file    Target file
 * @param indent  Space to place before the output line (i.e.: html indent)
 * @param paths   Collection of page paths
 * @param path_it Iterator to the current page path where the navigation is being written to
 */
void blogator::generator::IndexMaker::writePageNavDiv( std::ofstream &file,
                                                       const std::string &indent,
                                                       const dto::Index::PagePaths_t &paths,
                                                       const dto::Index::PagePaths_t::const_iterator &path_it ) const
{
    const bool   is_first    = path_it == paths.cbegin();
    const bool   is_last     = path_it == std::prev( paths.cend() );
    const size_t page_number = ( path_it - paths.cbegin() ) + 1;

    file << indent << "\t"
         << ( is_first
              ? html::createHyperlink( *paths.cbegin(), _options->_page_nav.first, "disabled" )
              : html::createHyperlink( *paths.cbegin(), _options->_page_nav.first ) )
         << ( is_first
              ? html::createHyperlink( *paths.cbegin(), _options->_page_nav.backwards, "disabled" )
              : html::createHyperlink( *std::prev( path_it ), _options->_page_nav.backwards, "disabled" ) )
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
 * Writes the breadcrumb html to a index 'by date' file
 * @param file        Target file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param page_number Page number
 */
void blogator::generator::IndexMaker::writeDateIndexBreadcrumb( std::ofstream &file,
                                                                const std::string &indent,
                                                                const size_t &page_number ) const
{
    file << indent << "\t<ul>\n"
         << indent << "\t\t<li>" << html::createHyperlink( "../../index.html", _options->_breadcrumb.start ) << "</li>\n"
         << indent << "\t\t<li>" << html::createHyperlink( "0.html", _options->_breadcrumb.by_date ) << "</li>\n"
         << indent << "\t\t<li>" << _options->_breadcrumb.page << page_number << "</li>\n"
         << indent << "\t</ul>\n";
}

/**
 * Writes the breadcrumb html to a index 'by tag' file
 * @param file        Target file
 * @param indent      Space to place before the output line (i.e.: html indent)
 * @param tag         Tag string
 */
void blogator::generator::IndexMaker::writeTagIndexBreadcrumb( std::ofstream &file,
                                                               const std::string &indent,
                                                               const std::string &tag ) const
{
    file << indent << "\t<ul>\n"
         << indent << "\t\t<li>" << html::createHyperlink( "../../index.html", _options->_breadcrumb.start ) << "</li>\n"
         << indent << "\t\t<li>" << html::createHyperlink( "tags.html", _options->_breadcrumb.by_tag ) << "</li>\n"
         << indent << "\t\t<li>" << tag << "</li>\n"
         << indent << "\t</ul>\n";
}

/**
 * Writes the breadcrumb html to the tag list index file
 * @param file        Target file
 * @param indent      Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::IndexMaker::writeTagListBreadcrumb( std::ofstream &file,
                                                              const std::string &indent ) const
{
    file << indent << "\t<ul>\n"
         << indent << "\t\t<li>" << html::createHyperlink( "../../index.html", _options->_breadcrumb.start ) << "</li>\n"
         << indent << "\t\t<li>" << _options->_breadcrumb.by_tag << "</li>\n"
         << indent << "\t</ul>\n";
}


