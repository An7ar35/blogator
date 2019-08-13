#include "html.h"
#include "../cli/MsgInterface.h"

#include <filesystem>
#include <iostream>

/**
 * Creates a stylesheet link for a html <head>
 * @param css_path Path of the css file
 * @return Formatted link as a string
 */
std::string blogator::html::createStylesheetLink( const std::filesystem::path &css_path ) {
    return "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"" + css_path.string() + "\"/>";
}

/**
 * Creates a stylesheet link for a html <head>
 * @param css_path Absolute path of the css file
 * @param out_dir  Path of the output directory
 * @return Formatted link as a string
 */
std::string blogator::html::createStylesheetLink( const std::filesystem::path &css_loc, const std::filesystem::path &out_dir ) {
    auto css_path = css_loc.parent_path().lexically_relative( out_dir.parent_path() ) / css_loc.filename();
    return createStylesheetLink( css_path.string() );
}


/**
 * Creates a hyperlink tag
 * @param href      Link path/address
 * @param link_text Text to use in between <a></a> tags
 * @param css_class CSS class to use (optional)
 * @return Formatted html hyperlink tag
 */
std::string blogator::html::createHyperlink( const std::filesystem::path &href,
                                             const std::string &link_text,
                                             const std::string &css_class )
{
    std::stringstream ss;

    ss << "<a ";
    if( !css_class.empty() )
        ss << "class=\"" << css_class << "\" ";
    ss << "href=\"" << href.string() << "\">" << link_text << "</a>";

    return ss.str();
}

/**
 * Creates a hyperlink tag
 * @param href      Link path/address
 * @param link_text Text to use in between <a></a> tags
 * @param css_class CSS class to use (optional)
 * @param rel       'rel' attribute
 * @return Formatted html hyperlink tag
 */
std::string blogator::html::createHyperlink( const std::filesystem::path &href,
                                             const std::string &link_text,
                                             const std::string &css_class,
                                             const std::string &rel )
{
    std::stringstream ss;

    ss << "<a ";
    if( !css_class.empty() )
        ss << "class=\"" << css_class << "\" ";
    if( !rel.empty() )
        ss << "rel=\"" << rel << "\" ";
    ss << "href=\"" << href.string() << "\">" << link_text << "</a>";

    return ss.str();
}

/**
 * Create a time tag
 * @param date_stamp DateStamp DTO
 * @param months     Month string lookup map
 * @param css_class  CSS class to use (optional)
 * @return Formatted time tag
 */
std::string blogator::html::createDateTime( const blogator::dto::DateStamp & date_stamp,
                                            const dto::Options::MonthStringLookup_t &months,
                                            const std::string & css_class )
{
    std::stringstream ss;

    ss << "<time ";
    if( !css_class.empty() )
        ss << "class=\"" << css_class << "\" ";
    ss << "datetime=\"" << date_stamp._year << "-" << date_stamp._month << "-" << date_stamp._day << "\">"
       << date_stamp._day << " " << months.at( date_stamp._month ) << " " << date_stamp._year
       << "</time>";

    return ss.str();
}

/**
 * Creates a breadcrumb HTML list
 * @param breadcrumb Breadcrumb
 * @return Breadcrumb's HTML
 */
blogator::dto::HTML blogator::html::createBreadcrumb( const blogator::html::BreadCrumb_t &breadcrumb ) {
    auto html = dto::HTML();
    auto it   = breadcrumb.cbegin();
    auto last = std::prev( breadcrumb.cend() );

    html._lines.emplace_back( "<ul>" );
    while( it != breadcrumb.cend() ) {
        std::stringstream ss;
        if( it != last )
            ss << "<li>" << html::createHyperlink( it->first, it->second.string() ) << "</li>";
        else
            ss << "<li>" << it->first << "</li>";
        html._lines.emplace_back( ss.str() );
        ++it;
    }
    html._lines.emplace_back( "</ul>" );

    return html;
}

/**
 * Generates the HTML for the By-Date section of the index pane
 * @param master_index Index DTO
 * @param options      Options DTO
 * @return IndexDateTree DTO
 */
std::unique_ptr<blogator::dto::IndexDateTree>
    blogator::html::generateIndexDateTreeHTML( const blogator::dto::Index & master_index,
                                               const blogator::dto::Options & options )
{
    auto tree = std::make_unique<dto::IndexDateTree>();

    auto article = master_index._articles.cbegin();
    if( article != master_index._articles.cend() ) {
        size_t curr_i    = 0; //article position in master index
        auto   curr_date = article->_datestamp;

        html::writer::openTree( *tree );
        html::writer::openYearNode( article->_datestamp, *tree );
        html::writer::openMonthNode( article->_datestamp, options._months, *tree );

        while( article != master_index._articles.cend() ) {

            if( article->_datestamp._year != curr_date._year ) {
                html::writer::closeMonthNode( *tree );
                html::writer::closeYearNode( *tree );
                html::writer::openYearNode( article->_datestamp, *tree );
                html::writer::openMonthNode( article->_datestamp, options._months, *tree );

            } else if( article->_datestamp._month != curr_date._month ) {
                html::writer::closeMonthNode( *tree );
                html::writer::openMonthNode( article->_datestamp, options._months, *tree );
            }

            html::writer::addArticleLeaf( *article, curr_i, *tree );

            curr_date = article->_datestamp;
            ++curr_i;
            ++article;
        }

        html::writer::closeMonthNode( *tree );
        html::writer::closeYearNode( *tree );
        html::writer::closeTree( *tree );
    }

    return std::move( tree );
}

/**
 * Generates the HTML for the By-Tag section of the index pane
 * @param master_index Index DTO
 * @param options      Options DTO
 * @return IndexTagTree DTO
 */
std::unique_ptr<blogator::dto::IndexTagTree>
    blogator::html::generateIndexTagTreeHTML( const blogator::dto::Index   &master_index,
                                              const blogator::dto::Options &options )
{
    auto &display = cli::MsgInterface::getInstance();

    auto tree = std::make_unique<dto::IndexTagTree>();

    html::writer::openTree( *tree );

    for( const auto &tag : master_index._indices.byTag.cats ) {
        html::writer::openTagNode( tag.first, *tree );

        for( auto article_i : tag.second.article_indices ) {
            try {
                html::writer::addArticleLeaf( master_index._articles.at( article_i ),
                                              article_i,
                                              *tree );

            } catch( std::out_of_range &e ) {
                std::stringstream ss;
                ss << "Could not find article in i=" << article_i
                   << " referenced in tag '" << tag.first << "'. Skipping.";
                display.error( ss.str() );
            }
        }

        html::writer::closeTagNode( *tree );
    }

    html::writer::closeTree( *tree );

    return std::move( tree );
}