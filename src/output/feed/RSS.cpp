#include <utility>
#include <iostream>
#include <fstream>

#include "RSS.h"
#include "../../exception/file_access_failure.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::output::feed::RSS::RSS( std::shared_ptr<const dto::Index>     master_index,
                                  std::shared_ptr<const dto::Templates> templates,
                                  std::shared_ptr<const dto::Options>   global_options ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) ),
    _display( cli::MsgInterface::getInstance() )
{
    _feed_item_count = _options->_rss.item_count > _master_index->_articles.size()
                       ? _master_index->_articles.size()
                       : _options->_rss.item_count;
}

/**
 * Initialise RSS feed creator
 * @return Errorless success
 */
bool blogator::output::feed::RSS::init() {
    if( !_options->_rss.generate )
        return true;

    try {
        auto path = _options->_paths.root_dir / _options->_rss.file_name;

        if( std::filesystem::exists( path ) )
            throw exception::file_access_failure(
                "Old RSS feed file was not purged: " + path.string()
            );

        std::ofstream out( path );

        if( !out.is_open() )
            throw exception::file_access_failure(
                "Could not open the RSS feed's output file: " + path.string()
            );

        writeHead( out );
        writeItems( out );
        writeFoot( out );

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
 * Writes the feed's header tags
 * @param file RSS feed file
 */
void blogator::output::feed::RSS::writeHead( std::ofstream &file ) const {
    _display.begin( "Generating RSS feed", _feed_item_count + 2, "header" );

    file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
         << "<rss version=\"2.0\">\n"
         << "\t<channel>\n"
         << "\t\t<title>" << _options->_rss.title << "</title>\n"
         << "\t\t<link>" << _options->_rss.root_url << "</link>\n"
         << "\t\t<image>\n"
         << "\t\t\t<title>" << _options->_rss.img_alt << "</title>\n";

    if( _options->_rss.root_url.back() != '/' )
        file << "\t\t\t<url>" << _options->_rss.root_url << "/" << _options->_rss.img_url << "</url>\n";
    else
        file << "\t\t\t<url>" << _options->_rss.root_url << _options->_rss.img_url << "</url>\n";

    file << "\t\t\t<url>" << _options->_rss.img_url.front() << "</url>\n"
         << "\t\t\t<link>" << _options->_rss.img_link << "</link>\n"
         << "\t\t\t<width>" << _options->_rss.img_width << "</width>\n"
         << "\t\t\t<height>" << _options->_rss.img_height << "</height>\n"
         << "\t\t\t<description>" << "" << "</description>\n"
         << "\t\t</image>\n"
         << "\t\t<img>" << _options->_rss.img_url << "</img>\n"
         << "\t\t<description>" << _options->_rss.description << "</description>\n"
         << "\t\t<copyright>" << _options->_rss.copyright << "</copyright>\n"
         << "\t\t<lastBuildDate>" << _options->getRunTimeStamp() << "</lastBuildDate>\n"
         << "\t\t<generator>" << _options->getSoftwareNameStr() << " " << _options->getSoftwareVersionStr() << "</generator>\n";
}

/**
 * Writes all article items into the feed
 * @param file RSS feed file
 */
void blogator::output::feed::RSS::writeItems( std::ofstream &file ) const {
    size_t item_count  = 0;
    auto   article_it  = _master_index->_articles.cbegin();

    while( article_it != _master_index->_articles.cend() &&
           item_count < _options->_rss.item_count )
    {
        _display.progress( "feed item " + std::to_string( item_count + 1 ) + "/" + std::to_string( _feed_item_count ) );

        auto rel_path = ( _options->_paths.posts_dir / article_it->_paths.out_html ).lexically_relative( _options->_paths.root_dir );

        file << "\t\t<item>\n"
             << "\t\t\t<title>" << article_it->_heading << "</title>\n"
             << "\t\t\t<link>" << _options->_rss.root_url << "/" << rel_path.string() << "</link>\n"
             << "\t\t\t<pubDate>" << article_it->_datestamp._day << " "
                                  << _options->_months.at( article_it->_datestamp._month ) << " "
                                  << article_it->_datestamp._year
                                  << "</pubDate>\n";

        for( const auto &tag : article_it->_tags ) {
            file << "\t\t\t<category>" << tag << "</category>\n";
        }

        file << "\t\t</item>\n";

        ++article_it;
        ++item_count;
    }
}

/**
 * Write the feed's footer closing tags
 * @param file RSS feed file
 */
void blogator::output::feed::RSS::writeFoot( std::ofstream & file ) const {
    _display.progress( "footer" );
    file << "\t</channel>\n"
         << "</rss>";
}