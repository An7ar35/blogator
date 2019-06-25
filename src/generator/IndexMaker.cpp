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
 * @param global_options Global blogator options
 */
blogator::generator::IndexMaker::IndexMaker( std::shared_ptr<dto::Options> global_options ) :
    _options( std::move( global_options ) ),
    _by_date_page_paths( std::make_unique<PagePaths_t>() )
{}

/**
 * Initialize HTML index pages generation
 * @param master_index Master index of all posts/articles
 * @param templates    Master templates
 * @param tag_index    Master Index of tags to articles
 * @return Success
 */
bool blogator::generator::IndexMaker::init( const blogator::dto::Index &master_index,
                                            const blogator::dto::Template &templates,
                                            const blogator::dto::Index::TagIndexMap_t &tag_index )
{
    size_t page_count = ( master_index._articles.size() / _options->_index.items_per_page )
                      + ( ( master_index._articles.size() % _options->_index.items_per_page ) > 0 );

    for( size_t p = 0; p < page_count; ++p ) {
        std::stringstream ss;
        ss << "page" << p << ".html";
        _by_date_page_paths->emplace_back(
            _options->_paths.index_dir / _options->_paths.index_sub_dirs.by_date / ss.str()
        );
    }

    auto consecutive_div_pos = dto::Template::getConsecutiveWritePositions( templates._index.div_write_pos );

    auto index_byDate_ok = generateDateIndexPages( master_index, templates, consecutive_div_pos );

    return ( index_byDate_ok );
}

/**
 * Generates 'by date' index pages
 * @param master_index  Master index
 * @param templates     Templates
 * @param insert_points Consecutive insert points in the index template
 * @throws exception::file_access_failure
 * @throws std::invalid_argument when page path could not be generated for index by date
 */
bool blogator::generator::IndexMaker::generateDateIndexPages( const dto::Index &master_index,
                                                              const dto::Template &templates,
                                                              const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    try {
        auto article_it   = master_index._articles.cbegin();
        auto page_path_it = _by_date_page_paths->cbegin();
        while( article_it != master_index._articles.cend() ) {

            if( page_path_it == _by_date_page_paths->cend() )
                throw std::invalid_argument( "Page paths found for the index pages are not sufficient for the number of articles." );

            if( std::filesystem::exists( *page_path_it ) )
                throw exception::file_access_failure(
                    "File '" + page_path_it->string() + "' already exists. Possible files with duplicate names in source folder structure."
                );

            auto page_out = std::ofstream( *page_path_it );

            if( !page_out.is_open() )
                throw exception::file_access_failure(
                    "File '" + page_path_it->string() + "' could not be opened for writing."
                );

            writeDateIndexPage( page_out, page_path_it,
                                templates, master_index._articles, article_it,
                                insert_points );

            page_out.close();
            ++page_path_it;
        }

        return true;
    } catch( exception::file_access_failure &e ) {
        std::cerr << e.what() << std::endl;
    }

    return false;
}

/**
 * Writes a page of entries
 * @param page          Page file
 * @param page_path     Page file path iterator
 * @param templates     Templates
 * @param articles      Articles
 * @param article_it    Iterator to the current article to be processed
 * @param insert_points Index template insertion point locations
 */
void blogator::generator::IndexMaker::writeDateIndexPage( std::ofstream & page,
                                                          const PagePaths_t::const_iterator &page_path_it,
                                                          const dto::Template &templates,
                                                          const dto::Index::Articles_t &articles,
                                                          dto::Index::Articles_t::const_iterator &article_it,
                                                          const dto::Template::ConsecutiveDivWritePositions_t &insert_points ) const
{
    size_t entry_counter = 0;
    size_t curr_src_line = 0;
    auto   insert_point  = insert_points.cbegin();

    for( const auto &template_line : templates._index.html->_lines ) {
        if( insert_point->first.line == curr_src_line ) {
            const auto char_it = find_if( template_line.begin(),
                                          template_line.end(),
                                          []( char c ) { return !isspace( c ); } );
            const auto spaces  = template_line.substr( 0, char_it - template_line.begin() );

            page << template_line.substr( 0, insert_point->first.col ) << std::endl;

            if( insert_point->second == "index_entries" ) {

                while( article_it != articles.cend() && entry_counter < _options->_index.items_per_page ) {
                    auto rel_path = ( _options->_paths.posts_dir / article_it->_paths.out_html ).lexically_relative( page_path_it->parent_path() );
                    writeIndexEntry( page, spaces + "\t", templates._months, *article_it, rel_path );

                    ++article_it;
                    ++entry_counter;
                }

                page << spaces << template_line.substr( insert_point->first.col ) << std::endl;

            } else if( insert_point->second == "page_nav" ) {
                writePageNavDiv( page, spaces + "\t", *_by_date_page_paths, page_path_it );
            } else {
                std::cerr << "Tag HTML Div class '" << insert_point->second << "' not recognised." << std::endl;
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
 * @param fore_space     Space to place before the output lines (i.e.: html indent)
 * @param months         Month string lookup map
 * @param article        Article DTO
 * @param post_href_path Relative HTML post file path
 */
void blogator::generator::IndexMaker::writeIndexEntry( std::ofstream &file,
                                                       const std::string &fore_space,
                                                       const blogator::dto::Template::MonthStringLookup_t &months,
                                                       const blogator::dto::Article &article,
                                                       const std::filesystem::path &post_href_path ) const
{
    /**
     * [LAMBDA] Write a standard entry to an index page
     */
    auto writeStandardEntry = [&]() {
        file << fore_space << "\t<span class=\"index-entry-heading\">" << article._heading << "</span>\n"
             << fore_space << "\t" << html::createDateTime( article._datestamp, months ) << "\n"
             << fore_space << "\t<div class=\"index-entry-tags\">\n";
        for( const auto &tag : article._tags ) {
            file << fore_space << "\t\t<span class=\"tag\">" << tag << "</span>\n";
        }
        file << fore_space << "\t</div>\n";
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
            file << fore_space << "\t" << line << "\n";
        }

        in.close();
    };


    file << fore_space << "<a href=\"" << post_href_path.string() << "\">\n";

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

    file << fore_space << "</a>" << std::endl;
}

/**
 * Writes the page navigation html to a file
 * @param file         Target file
 * @param fore_space   Space to place before the output line (i.e.: html indent)
 * @param page_path_it Iterator to the current page path where the navigation is being written to
 */
void blogator::generator::IndexMaker::writePageNavDiv( std::ofstream &file,
                                                       const std::string &fore_space,
                                                       const PagePaths_t &page_paths,
                                                       const IndexMaker::PagePaths_t::const_iterator &page_path_it ) const
{
    const bool   is_first    = page_path_it == page_paths.cbegin();
    const bool   is_last     = page_path_it == std::prev( page_paths.cend() );
    const size_t page_number = ( page_path_it - page_paths.cbegin() ) + 1;

    file << fore_space << "\t"
         << ( is_first
              ? html::createHyperlink( *page_paths.cbegin(), _options->_page_nav.first, "disabled" )
              : html::createHyperlink( *page_paths.cbegin(), _options->_page_nav.first ) )
         << ( is_first
              ? html::createHyperlink( *page_paths.cbegin(), _options->_page_nav.backwards, "disabled" )
              : html::createHyperlink( *std::prev( page_path_it ), _options->_page_nav.backwards, "disabled" ) )
         << std::to_string( page_number ) << _options->_page_nav.separator << std::to_string( page_paths.size() )
         << ( is_last
              ? html::createHyperlink( *std::prev( page_paths.cend() ), _options->_page_nav.forward, "disabled" )
              : html::createHyperlink( *std::next(  page_path_it ), _options->_page_nav.forward ) )
         << ( is_last
              ? html::createHyperlink( *std::prev( page_paths.cend() ), _options->_page_nav.last, "disabled" )
              : html::createHyperlink( *std::prev( page_paths.cend() ), _options->_page_nav.last ) )
         << std::endl;
}