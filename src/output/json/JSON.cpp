#include "JSON.h"

#include <fstream>
#include "../../exception/file_access_failure.h"
#include "../../fs/fs.h"

/**
 * Escapes a string for JSON output
 * WARNING: will not work on UTF-8 characters that are larger than the C++/Linux char size
 * @param str String to escape
 * @return Escaped string
 */
std::string blogator::output::json::escapeStr( const std::string &str ) {
    //RFC-4627: https://www.ietf.org/rfc/rfc4627.txt
    std::stringstream ss;

    for( auto c : str ) {
        switch( c ) {
            case 0x08: ss << R"(\b)"; break; //BS
            case 0x09: ss << R"(\t)"; break; //HT
            case 0x0A: ss << R"(\n)"; break; //LF
            case 0x0C: ss << R"(\f)"; break; //FF
            case 0x0D: ss << R"(\r)"; break; //CR
            case 0x22: ss << R"(\")"; break; //quotation mark
            case 0x5C: ss << R"(\\)"; break; //reverse solidus
            default: ss << c;
        }
    }

    return ss.str();
}

/**
 * Constructor
 * @param master_index   Master index
 * @param global_options Global blogator options
 */
blogator::output::json::JSON::JSON( std::shared_ptr<const dto::Index> master_index,
                                    std::shared_ptr<const dto::Options> global_options ) :
    _master_index( std::move( master_index ) ),
    _options( std::move( global_options ) ),
    _json_item_processed( 0 ),
    _json_item_count( 0 ),
    _display( cli::MsgInterface::getInstance() )

{
    _json_item_count += _master_index->_articles.size();
    _json_item_count += ( _options->_index.index_by_tag    ? _master_index->_indices.byTag.cats.size()    : 0 );
    _json_item_count += ( _options->_index.index_by_author ? _master_index->_indices.byAuthor.cats.size() : 0 );
    _json_item_count += ( _options->_index.index_by_year   ? _master_index->_indices.byYear.cats.size()   : 0 );
}

/**
 * Initialise JSON index creator
 * @return Errorless success
 */
bool blogator::output::json::JSON::init() {
    if( !_options->_json_index.generate )
        return true;

    try {
        auto path = _options->_paths.index_dir / _options->_json_index.file_name;

        if( std::filesystem::exists( path ) )
            throw exception::file_access_failure(
                "Old JSON index file was not purged: " + path.string()
            );

        std::ofstream out( path );

        if( !out.is_open() )
            throw exception::file_access_failure(
                "Could not open the JSON index's output file: " + path.string()
            );

        _display.begin( "Generating JSON index", _json_item_count + 1, "Articles" );

        out << "{ ";

        writeArticles( out );

        if( _options->_index.index_by_tag ) {
            out << ", ";
            writeTags( out );
        }
        if( _options->_index.index_by_author ) {
            out << ", ";
            writeAuthors( out );
        }

        if( _options->_index.index_by_year ) {
            out << ", ";
            writeYears( out );
        }

        if( !_options->_json_index.append_paths.empty() ) {

            for( const auto &src_rel_path : _options->_json_index.append_paths ) {
                out << ", ";

                _display.debug( "Appending external JSON content to JSON Index file: " + src_rel_path );

                auto src_abs_path = _options->_paths.root_dir / src_rel_path;
                appendJSON( src_abs_path, out );
            }
        }

        out << " }";

        out.flush();
        out.close();

    } catch( std::exception &e ) {
        _display.error( e.what() );
        return false;
    }

    _display.progress( "DONE" );
    return true;
}

/**
 * Write out the article index section to file
 * @param file JSON file output
 */
void blogator::output::json::JSON::writeArticles( std::ofstream &file ) {
    //"articles" = [ { "title": "", "authors": [ "" ], "tags": [ "" ], "headings": [ "" ], "href": "" } ]

    file << R"("articles": [ )";

    for( auto article_it = _master_index->_articles.cbegin(); article_it != _master_index->_articles.cend(); ++article_it ) {

        _display.progress( "Articles" );

        file << "{ "
             << R"("title": ")" << json::escapeStr( article_it->_heading ) << "\", "
             << R"("date": ")" << article_it->_datestamp << "\", "
             << R"("authors": [ )";

        for( auto it = article_it->_authors.cbegin(); it != article_it->_authors.cend(); ++it ) {
            if( it != std::prev( article_it->_authors.cend() ) )
                file << "\"" << json::escapeStr( *it ) << "\", ";
            else
                file << "\"" << json::escapeStr( *it ) << "\" ], ";
        }

        file << R"("tags": [ )";

        for( auto it = article_it->_tags.cbegin(); it != article_it->_tags.cend(); ++it ) {
            if( it != std::prev( article_it->_tags.cend() ) )
                file << "\"" << json::escapeStr( *it ) << "\", ";
            else
                file << "\"" << json::escapeStr( *it ) << "\" ], ";
        }

        file << R"("headings": [ )";
        if( article_it->_toc ) { //has ToC auto generated
            for( auto it = article_it->_toc->headings().cbegin(); it != article_it->_toc->headings().cend(); ++it ) {
                if( it != std::prev( article_it->_toc->headings().cend() ) )
                    file << "\"" << json::escapeStr( it->second.str ) << "\", ";
                else
                    file << "\"" << json::escapeStr( it->second.str ) << "\" ";
            }
        }

        file << R"(], "href": ")"
             << json::escapeStr( _options->_folders.posts.root / article_it->_paths.out_html )
             << R"(" })";

        if( article_it != std::prev( _master_index->_articles.cend() ) )
            file << ", ";

        ++_json_item_processed;
    }

    file << " ]";

}

/**
 * Write out the tag index section to file
 * @param file JSON file output
 */
void blogator::output::json::JSON::writeTags( std::ofstream &file ) {
    //"tags": [ { name: "", href: "" } ]

    file << R"("tags": [ )";

    for( auto tag_it = _master_index->_indices.byTag.cats.cbegin(); tag_it != _master_index->_indices.byTag.cats.cend(); ++tag_it ) {

        _display.progress( "Tags" );

        file << R"({ "name": ")"
             << json::escapeStr( tag_it->first )
             << R"(", "href": ")"
             << json::escapeStr( _options->_folders.index.by_tag / tag_it->second.file_names.front() )
             << R"(" })";

        if( tag_it != std::prev( _master_index->_indices.byTag.cats.cend() ) )
            file << ", ";

        ++_json_item_processed;
    }

    file << " ]";
}

/**
 * Write out the author index section to file
 * @param file JSON file output
 */
void blogator::output::json::JSON::writeAuthors( std::ofstream &file ) {
    //"authors": [ { "name": "", "href": "" } ]

    file << R"("authors": [ )";

    for( auto author_it = _master_index->_indices.byAuthor.cats.cbegin(); author_it != _master_index->_indices.byAuthor.cats.cend(); ++author_it ) {

        _display.progress( "Authors" );

        file << R"({ "name": ")"
             << json::escapeStr( author_it->first )
             << R"(", "href": )"
             << json::escapeStr( _options->_folders.index.by_author / author_it->second.file_names.front() )
             << R"(" })";

        if( author_it != std::prev( _master_index->_indices.byAuthor.cats.cend() ) )
            file << ", ";

        ++_json_item_processed;
    }

    file << " ]";
}

/**
 * Write out the year index section to file
 * @param file JSON file output
 */
void blogator::output::json::JSON::writeYears( std::ofstream &file ) {
    //"years": [ { "year": "", "href": "" } ]

    file << R"("years": [ )";

    for( auto year_it = _master_index->_indices.byYear.cats.cbegin(); year_it != _master_index->_indices.byYear.cats.cend(); ++year_it ) {

        _display.progress( "Years" );

        file << R"({ "year": ")"
             << json::escapeStr( year_it->first )
             << R"(", "href": ")"
             << json::escapeStr( _options->_folders.index.by_year / year_it->second.file_names.front() )
             << R"(" })";

        if( year_it != std::prev( _master_index->_indices.byYear.cats.cend() ) )
            file << ", ";

        ++_json_item_processed;
    }

    file << " ]";
}

/**
 * Appends the content of a third party JSON file to the Blogator JSON index file
 * Note: assumes the source contains valid JSON
 * @param src_path Source JSON file path whose content is to be appended to the output JSON
 * @param file     Output JSON
 * @throws exception::file_access_failure when source JSON file could not be opened
 */
void blogator::output::json::JSON::appendJSON( const std::filesystem::path &src_path, std::ofstream &file ) {
    std::ifstream     src( src_path.string() );
    std::stringstream ss;

    if( src.is_open() ) {
        std::string line;

        while( getline( src, line ) ) {
            ss << line;
        }

        src.close();

        std::string cache = ss.str();

        if( cache.empty() ) {
            _display.warning( "JSON file to append is empty: " + src_path.string() );
            return; //EARLY-RETURN
        }

        std::string::size_type start = 0;
        std::string::size_type end   = cache.length() - 1;

        //move to the first character at start and end
        while( start < cache.length() && ( isspace( cache.at( start ) ) ) )
            ++start;

        while( end > 0 && ( isspace( cache.at( end ) ) ) )
            --end;

        //get rid of the first encountered '{' and '}' on either end
        if( start < cache.length() && cache.at( start ) == '{' )
            ++start;

        if( end > 0 && cache.at( end ) == '}' )
            --end;

        if( start >= end ) {
            _display.error( "JSON file looks malformed: " + src_path.string() );
            return; //EARLY-RETURN
        }

        std::stringstream temp;
        temp << "start: '" << start << "', end: '" << end << "'.";
        _display.debug( temp.str() );

        for( ; start <= end; ++start )
            file << cache.at( start );

    } else {
        throw exception::file_access_failure( "Could not open: " + src_path.string() );
    }
}
