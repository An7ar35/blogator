#include "Indices.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "ByYearList.h"
#include "ByTagList.h"
#include "ByAuthorList.h"
#include "../generic/ChronoIndexLister.h"
#include "../abstract/IndexList.h"
#include "../../fs/fs.h"
#include "../../html/html.h"
#include "../../exception/file_access_failure.h"
#include "../../exception/failed_expectation.h"
#include "../../cli/MsgInterface.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::output::page::Indices::Indices( std::shared_ptr<const dto::Index>     master_index,
                                          std::shared_ptr<const dto::Templates> templates,
                                          std::shared_ptr<const dto::Options>   global_options ) :
    Page( std::move( master_index ),
          std::move( templates ),
          std::move( global_options )
    ),
    _entry_maker( generic::EntryWriter( _templates->_index_entry, _options->_months ) )
{
    _breadcrumb_parents = BreadCrumb_t( { { _options->_breadcrumb.start, "../../index.html" } } );

    _total_jobs = _index->_indices.chronological.page_count
                + _index->_indices.byYear.page_count
                + _index->_indices.byTag.page_count
                + _index->_indices.byAuthor.page_count;
}

/**
 * Initialize HTML index pages generation
 * @return Success
 * @throws exception::failed_expectation when insert points could not be found in 1 or more of the index templates
 */
bool blogator::output::page::Indices::init() const {
    try {
        if( _templates->_index->block_write_pos.empty() || _templates->_index_entry->block_write_pos.empty() ) {
            throw exception::failed_expectation(
                "Missing insertion points in one or more index templates "
                "(Found: index page=" + std::to_string( _templates->_index->block_write_pos.size() ) +
                ", index entry=" + std::to_string( _templates->_index_entry->block_write_pos.size() ) + ")."
            );
        }

        { //Chronological index ("by_date")
            if( _index->_indices.chronological.file_names.empty() )
                throw exception::failed_expectation(
                    "No prepared filenames were found in the chronological index "
                    "(" + std::to_string( _index->_indices.chronological.page_count ) + "pages "
                    "for " + std::to_string( _index->_articles.size() ) + " articles)."
                );

            const auto &first_file = *_index->_indices.chronological.file_names.cbegin();
            _display.begin( "Generating index pages", _total_jobs, _options->_paths.index_date_dir.filename() / first_file );

            auto entry_maker = generic::EntryWriter( _templates->_index_entry, _options->_months );
            auto index_maker = generic::ChronoIndexLister( _options, _index->_articles, *_templates->_index, entry_maker );

            auto breadcrumb = BreadCrumb_t( _breadcrumb_parents.cbegin(), _breadcrumb_parents.cend() );
            index_maker.write( _index->_indices.chronological, breadcrumb, _options->_paths.index_date_dir );
        }

        if( _options->_index.index_by_year ) { //Year list index ("by_year")
            if( _templates->_year_list->block_write_pos.empty() )
                throw exception::failed_expectation( "Missing insertion points in the tag list template." );

            _display.progress( _options->_folders.index.by_year.filename() / _options->_filenames.year_list );

            auto list_page_maker = ByYearList( _index, _options, *_templates->_year_list );
            auto list_page_path  = std::filesystem::path( _options->_paths.index_year_dir / _options->_filenames.year_list );
            auto entry_maker     = generic::EntryWriter( _templates->_index_entry, _options->_months );
            auto cat_page_maker  = generic::CategoryLister( _options, _index->_articles, *_templates->_index, entry_maker );

            auto breadcrumb      = BreadCrumb_t( _breadcrumb_parents.cbegin(), _breadcrumb_parents.cend() );
            breadcrumb.emplace_back( std::make_pair( _options->_breadcrumb.by_year, _options->_filenames.year_list ) );
            dto::Page page       = { list_page_path, std::ofstream( list_page_path ) };

            list_page_maker.write( page );
            cat_page_maker.write( _index->_indices.byYear, breadcrumb, _options->_paths.index_year_dir );
        }

        if( _options->_index.index_by_tag ) { //Tag list index ("by_tag")
            if( _templates->_tag_list->block_write_pos.empty() )
                throw exception::failed_expectation( "Missing insertion points in the tag list template." );

            _display.progress( _options->_folders.index.by_tag.filename() / _options->_filenames.tag_list );

            auto list_page_maker = ByTagList( _index, _options, *_templates->_tag_list );
            auto list_page_path  = std::filesystem::path( _options->_paths.index_tag_dir / _options->_filenames.tag_list );
            auto entry_maker     = generic::EntryWriter( _templates->_index_entry, _options->_months );
            auto cat_page_maker  = generic::CategoryLister( _options, _index->_articles, *_templates->_index, entry_maker );

            auto breadcrumb      = BreadCrumb_t( _breadcrumb_parents.cbegin(), _breadcrumb_parents.cend() );
            breadcrumb.emplace_back( std::make_pair( _options->_breadcrumb.by_tag, _options->_filenames.tag_list ) );
            dto::Page page       = { list_page_path, std::ofstream( list_page_path ) };

            list_page_maker.write( page );
            cat_page_maker.write( _index->_indices.byTag, breadcrumb, _options->_paths.index_tag_dir );
        }

        if( _options->_index.index_by_author ) { //Author list index ("by_author")
            if(  _templates->_author_list->block_write_pos.empty() )
                throw exception::failed_expectation( "Missing insertion points in the author list template." );

            _display.progress( _options->_folders.index.by_author.filename() / _options->_filenames.author_list );

            auto list_page_maker = ByAuthorList( _index, _options, *_templates->_author_list );
            auto list_page_path  = std::filesystem::path( _options->_paths.index_author_dir / _options->_filenames.author_list );
            auto entry_maker     = generic::EntryWriter( _templates->_index_entry, _options->_months );
            auto cat_page_maker  = generic::CategoryLister( _options, _index->_articles, *_templates->_index, entry_maker );

            auto breadcrumb      = BreadCrumb_t( _breadcrumb_parents.cbegin(), _breadcrumb_parents.cend() );
            breadcrumb.emplace_back( std::make_pair( _options->_breadcrumb.by_author, _options->_filenames.author_list ) );
            dto::Page page       = { list_page_path, std::ofstream( list_page_path ) };

            list_page_maker.write( page );
            cat_page_maker.write( _index->_indices.byAuthor, breadcrumb, _options->_paths.index_author_dir );
        }

        _display.progress( "DONE" );

    } catch( std::exception &e ) {
        _display.error( e.what() );
        return false;
    }

    return true;
}