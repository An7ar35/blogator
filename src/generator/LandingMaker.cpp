#include "LandingMaker.h"
#include <iostream>
#include <fstream>

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
blogator::generator::LandingMaker::LandingMaker( std::shared_ptr<const dto::Index>    master_index,
                                                 std::shared_ptr<const dto::Template> templates,
                                                 std::shared_ptr<const dto::Options>  global_options ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) )
{}

/**
 * Initialize HTML landing page generation
 * @return Success
 * @throws exception::failed_expectation when insert points could not be found in the landing page template
 */
bool blogator::generator::LandingMaker::init() const {
    auto &display              = cli::MsgInterface::getInstance();
    display.begin( "Generating landing page", 1, "..." );

    auto page_write_positions  = dto::Template::getConsecutiveWritePositions( _templates->_start.div_write_pos );
    auto entry_write_positions = dto::Template::getConsecutiveWritePositions( _templates->_start_entry.div_write_pos );

    if( page_write_positions.empty() || entry_write_positions.empty() ) {
        throw exception::failed_expectation(
            "Missing insertion points in one or more of the landing page templates "
            "(Found: page=" + std::to_string( page_write_positions.size() ) +
            ", entry=" + std::to_string( entry_write_positions.size() ) + ")."
        );
    }

    try {
        generateLandingPage( page_write_positions, entry_write_positions );
    } catch( std::exception &e ) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    display.progress( "DONE" );
    return true;
}

/**
 * Generates the landing page
 * @param page_insert_points  Consecutive landing page template insert points
 * @param entry_insert_points Consecutive landing page entry template insert points
 * @throws std::invalid_argument when page path could not be generated for landing page
 */
void blogator::generator::LandingMaker::generateLandingPage( const dto::Template::ConsecutiveWritePositions_t &page_insert_points,
                                                             const dto::Template::ConsecutiveWritePositions_t &entry_insert_points ) const
{
    auto abs_file_path = _options->_paths.root_dir / _templates->_start.file_name;
    auto page_out      = std::ofstream( abs_file_path );

    if( !page_out.is_open() )
        throw exception::file_access_failure(
            "File '" + abs_file_path.string() + "' could not be opened for writing."
        );

    page_out << _options->BLOGATOR_SIGNATURE << "\n";

    size_t curr_src_line = 0;
    auto   insert_point  = page_insert_points.cbegin();

    for( const auto &template_line : _templates->_start.html->_lines ) {
        if( insert_point != page_insert_points.cend() && insert_point->first.line == curr_src_line ) {
            const auto indent = html::reader::getIndent( template_line );
            page_out << template_line.substr( 0, insert_point->first.col ) << std::endl;

            if( insert_point->second == "breadcrumb" ) {
                writeBreadcrumb( page_out, indent + "\t" );
            } else if( insert_point->second == "newest-posts" ) {
                writeNewestPosts( page_out, indent + "\t", entry_insert_points );
            } else if( insert_point->second == "top-tags" ) {
                writeTopTags( page_out, indent + "\t" );
            } else if( insert_point->second == "top-authors" ) {
                writeTopAuthors( page_out, indent + "\t" );
            } else if( insert_point->second == "featured-posts" ) {
                writeFeatured( page_out, indent + "\t", entry_insert_points );
            } else {
                std::cerr << "[generator::LandingMaker::generateLandingPage(..)] "
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
 * Writes the breadcrumb html to the landing page file
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::LandingMaker::writeBreadcrumb( std::ofstream &page,
                                                         const std::string &indent ) const
{
    page << indent << "<ul>\n"
         << indent << "\t<li>" << _options->_breadcrumb.start << "</li>\n"
         << indent << "</ul>\n";
}

/**
 * Write the top tags (as defined in the landing page options) to the landing page
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::LandingMaker::writeTopTags( std::ofstream &page,
                                                      const std::string &indent ) const
{
    page << indent << "<ul>\n";

    for( const auto &tag : _master_index->_indices.byTag.top_tags ) {
        try {
            const auto       &specs     = _master_index->_indices.byTag.tags.at( tag );
            const std::string str       = tag + " (" + std::to_string( specs.article_indices.size() ) + ')';
            const auto       &file_name = specs.file_names.at( 0 );
            const auto        rel_path  = _options->_paths.index_tag_dir.lexically_relative( _options->_paths.root_dir );

            page << indent << "\t<li>"
                 << html::createHyperlink( rel_path / file_name , str )
                 << "</li>\n";

        } catch( std::out_of_range &e ) {
            if( _master_index->_indices.byTag.tags.find( tag ) == _master_index->_indices.byTag.tags.end() )
                std::cerr << "[generator::LandingMaker::writeTopTags(..)] "
                          << "Tag '" << tag << "' does not seem to exist in the master index."
                          << std::endl;
            else
                std::cerr << "[generator::LandingMaker::writeTopTags(..)] "
                          << "Tag '" << tag << "' doesn't not have any index page path(s) in its specifications."
                          << std::endl;
        }
    }

    page << indent << "</ul>\n";
}

/**
 * Write the top authors (as defined in the landing page options) to the landing page
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::generator::LandingMaker::writeTopAuthors( std::ofstream &page,
                                                         const std::string &indent ) const
{
    page << indent << "<ul>\n";

    for( const auto &author : _master_index->_indices.byAuthor.top_authors ) {
        try {
            const auto       &specs     = _master_index->_indices.byAuthor.authors.at( author );
            const std::string str       = author + " (" + std::to_string( specs.article_indices.size() ) + ')';
            const auto       &file_name = specs.file_names.at( 0 );
            const auto        rel_path  = _options->_paths.index_author_dir.lexically_relative( _options->_paths.root_dir );

            page << indent << "\t<li>"
                 << html::createHyperlink( rel_path / file_name , str )
                 << "</li>\n";

        } catch( std::out_of_range &e ) {
            if( _master_index->_indices.byTag.tags.find( author ) == _master_index->_indices.byTag.tags.end() )
                std::cerr << "[generator::LandingMaker::writeTopAuthors(..)] "
                          << "Author '" << author << "' does not seem to exist in the master index."
                          << std::endl;
            else
                std::cerr << "[generator::LandingMaker::writeTopAuthors(..)] "
                          << "Author '" << author << "' doesn't not have any index page path(s) in its specifications."
                          << std::endl;
        }
    }

    page << indent << "</ul>\n";
}

/**
 * Writes the newest posts entries (as defined in the landing page options) to the landing page
 * @param page                Target file
 * @param indent              Space to place before the output line (i.e.: html indent)
 * @param entry_insert_points Consecutive landing page entry insertion points
 */
void blogator::generator::LandingMaker::writeNewestPosts( std::ofstream &page,
                                                          const std::string &indent,
                                                          const dto::Template::ConsecutiveWritePositions_t &entry_insert_points ) const
{
    size_t i          = 0;
    auto   article_it = _master_index->_articles.cbegin();

    while( article_it != _master_index->_articles.cend() && i < _options->_landing_page.most_recent ) {
        auto href = _options->_folders.posts.root / article_it->_paths.out_html;
        page << indent << "<a href=\"" << href.string() << "\">\n";

        writeArticleEntry( page, indent + "\t", *article_it, *_templates->_start_entry.html, entry_insert_points );

        page << indent << "</a>" << std::endl;

        ++article_it;
        ++i;
    }
}

/**
 * Write the featured articles (as defined in the landing page options) to the landing page
 * @param page                Target file
 * @param indent              Space to place before the output line (i.e.: html indent)
 * @param entry_insert_points Consecutive landing page entry insertion points
 */
void blogator::generator::LandingMaker::writeFeatured( std::ofstream &page,
                                                       const std::string &indent,
                                                       const dto::Template::ConsecutiveWritePositions_t &entry_insert_points ) const
{
    for( const auto &article : _master_index->_featured ) {
        auto href = _options->_folders.posts.root / article._paths.out_html;
        page << indent << "<a href=\"" << href.string() << "\">\n";

        writeArticleEntry( page, indent + "\t", article, *_templates->_start_entry.html, entry_insert_points );

        page << indent << "</a>" << std::endl;
    }
}

/**
 * Writes an article entry based on a given html/insert-points template
 * @param page          Target file
 * @param indent        Space to place before the output line (i.e.: html indent)
 * @param article       Article DTO
 * @param src_html      Template source HTML
 * @param insert_points Consecutive landing page entry insertion points
 */
void blogator::generator::LandingMaker::writeArticleEntry( std::ofstream & page,
                                                           const std::string &indent,
                                                           const dto::Article &article,
                                                           const dto::HTML &src_html,
                                                           const dto::Template::ConsecutiveWritePositions_t & insert_points ) const
{
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
                for( const auto & author : article._authors ) {
                    page << indent << sub_indent
                         << "\t<span class=\"author\">" << author << "</span>\n";
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
                std::cerr << "[generator::LandingMaker::writeArticleEntry(..)] "
                          << "TODO: implement 'summary' insertion point"
                          << std::endl;

            } else {
                std::cerr << "[generator::LandingMaker::writeArticleEntry(..)] "
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