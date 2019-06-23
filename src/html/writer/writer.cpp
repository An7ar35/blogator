#include "writer.h"
#include <regex>
#include <sstream>

/**
 * Opens a new <ol class="tree"></ol> tree
 * @param date_tree IndexPane::DateTree container
 */
void blogator::html::writer::openTree( blogator::dto::IndexDateTree &date_tree ) {
    date_tree.html._lines.emplace_back( "<ol class=\"tree\">" );
}

/**
 * Closes the last <ol> tag of a tree
 * @param date_tree IndexPane::DateTree container
 */
void blogator::html::writer::closeTree( blogator::dto::IndexDateTree &date_tree ) {
    date_tree.html._lines.emplace_back( "</ol>" );
}

/**
 * Opens a new year (YYYY) node on a HTML ordered list (<ol>)
 * @param datestamp Article's DateStamp DTO
 * @param html      IndexPane::DateTree container
 */
void blogator::html::writer::openYearNode( const blogator::dto::DateStamp &datestamp,
                                           dto::IndexDateTree &date_tree )
{
    date_tree.date_line_map.emplace( std::make_pair( datestamp._year, date_tree.html._lines.size() ) );

    auto yyyy = std::to_string( datestamp._year );
    date_tree.html._lines.emplace_back( "\t<li> <label for=\"checkbox_" + yyyy + "\">" + yyyy + "</label>" +
                                        R"(<input type="checkbox" id="checkbox_)" + yyyy + "\"/>" );
    date_tree.html._lines.emplace_back( "\t\t<ol>" );
}

/**
 * Closes a YYYY tree node (year) on a HTML ordered list (<ol>)
 * @param html IndexPane::DateTree container
 */
void blogator::html::writer::closeYearNode( dto::IndexDateTree &date_tree ) {
    date_tree.html._lines.emplace_back( "\t\t</ol>" );
    date_tree.html._lines.emplace_back( "\t</li>" );
}

/**
 * Opens a new month (YYYYMM) node on a HTML ordered list (<ol>)
 * @param datestamp Article's DateStamp DTO
 * @param month_map Map of month number to strings
 * @param html      IndexPane::DateTree container
 */
void blogator::html::writer::openMonthNode( const blogator::dto::DateStamp &datestamp,
                                            const std::unordered_map<unsigned, std::string> &month_map,
                                            dto::IndexDateTree &date_tree )
{
    auto yyyymm = dto::DateStamp::concatYYYYMM( datestamp );
    date_tree.date_line_map.emplace( std::make_pair( yyyymm, date_tree.html._lines.size() ) );

    const auto &month   = month_map.at( datestamp._month );
    auto       yyyymm_s = std::to_string( yyyymm );
    date_tree.html._lines.emplace_back( "\t\t\t<li> <label for=\"checkbox_" + yyyymm_s + "\">" + month + "</label>" +
                                        R"(<input type="checkbox" id="checkbox_)" + yyyymm_s + "\"/>" );
    date_tree.html._lines.emplace_back( "\t\t\t\t<ol>" );
}

/**
 * Closes a YYYYMM tree node (month) on a HTML ordered list (<ol>)
 * @param html IndexPane::DateTree container
 */
void blogator::html::writer::closeMonthNode( dto::IndexDateTree &date_tree ) {
    date_tree.html._lines.emplace_back( "\t\t\t\t</ol>" );
    date_tree.html._lines.emplace_back( "\t\t\t</li>" );
}

/**
 * Adds an article leaf to the tree
 * @param article     Article DTO
 * @param article_pos Article position (i) in the master index
 * @param date_tree   IndexPane::DateTree container
 */
void blogator::html::writer::addArticleLeaf( const blogator::dto::Article &article,
                                             const size_t &article_pos,
                                             dto::IndexDateTree &date_tree )
{
    date_tree.article_line_map.emplace( std::make_pair( article_pos, date_tree.html._lines.size() ) );

    std::stringstream li;
    li << "\t\t\t\t\t<li class=\"\">"
       << std::setfill('0') << std::setw(2) << article._datestamp._day
       << " <a href=\"" << article._paths.out_html.string() << "\">"
       << article._heading << "</a></li>";
    date_tree.html._lines.emplace_back( li.str() );
}

/**
 * Opens a new <ol class="tree"></ol> tree
 * @param tag_tree IndexPane::TagTree container
 */
void blogator::html::writer::openTree( blogator::dto::IndexTagTree &tag_tree ) {
    tag_tree.html._lines.emplace_back( "<ol class=\"tree\">" );
}

/**
 * Closes the last <ol> tag of a tree
 * @param tag_tree IndexPane::TagTree container
 */
void blogator::html::writer::closeTree( blogator::dto::IndexTagTree &tag_tree ) {
    tag_tree.html._lines.emplace_back( "</ol>" );
}

/**
 * Opens a new tag node on a HTML ordered list (<ol>)
 * @param tag      Tag string
 * @param tag_tree IndexPane::TagTree container
 */
void blogator::html::writer::openTagNode( const std::string & tag,
                                          blogator::dto::IndexTagTree &tag_tree )
{
    tag_tree.tag_line_map.emplace( std::make_pair( tag, tag_tree.html._lines.size() ) );

    auto id_number = std::to_string( tag_tree.tag_line_map.size() );

    tag_tree.html._lines.emplace_back( "\t<li> <label for=\"checkbox_t" + id_number + "\">" + tag + "</label>" +
                                       R"(<input type="checkbox" id="checkbox_t)" + id_number + "\"/>" );
    tag_tree.html._lines.emplace_back( "\t\t<ol>" );
}

/**
 * Closes a tag tree node on a HTML ordered list (<ol>)
 * @param tag_tree IndexPane::TagTree container
 */
void blogator::html::writer::closeTagNode( blogator::dto::IndexTagTree &tag_tree ) {
    tag_tree.html._lines.emplace_back( "\t\t</ol>" );
    tag_tree.html._lines.emplace_back( "\t</li>" );
}

/**
 * Adds an article leaf to the tree
 * @param article     Article DTO
 * @param article_pos Article position (i) in the master index
 * @param date_tree   IndexPane::DateTree container
 */
void blogator::html::writer::addArticleLeaf( const blogator::dto::Article &article,
                                             const size_t &article_pos,
                                             blogator::dto::IndexTagTree &tag_tree )
{
    auto it = tag_tree.articles_line_map.find( article_pos );
    if( it == tag_tree.articles_line_map.end() ) {
        tag_tree.articles_line_map.emplace( std::make_pair( article_pos, std::vector<size_t>( { tag_tree.html._lines.size() } ) ) );
    } else {
        it->second.emplace_back( tag_tree.html._lines.size() );
    }

    std::stringstream li;
    li << "\t\t\t<li class=\"\">"
       << "<a href=\"" << article._paths.out_html.string() << "\">" << article._heading << "</a>"
       << "</li>";
    tag_tree.html._lines.emplace_back( li.str() );
}

/**
 * Inserts a state string into the first input checkbox tag found on a line
 * @param line  Line to find <input type="checkbox"...> tag in
 * @param state State string to add to the html input tag
 * @return Edited line if tag is found, otherwise the original line
 */
std::string blogator::html::writer::setInputCheckboxState( const std::string &line,
                                                           const std::string &state )
{
    const static auto tag = std::string( "<input type=\"checkbox\"" );
    const static auto rx  = std::regex( tag );

    auto it = std::sregex_iterator( line.begin(), line.end(), rx );

    if( it != std::sregex_iterator() ) {
        std::stringstream ss;
        ss << line.substr( 0, it->position() + tag.length() )
           << ' ' << state << ' '
           << line.substr( it->position()  + tag.length() );
        return  ss.str();
    }

    return line;
}

/**
 * Inserts a css class name into the first hyperlink tag found on a line
 * @param line      Line to find the <a href=".."> tag in
 * @param css_class CSS class string to insert into the tag
 * @return Edited line if tag is found, otherwise the original line
 */
std::string blogator::html::writer::setHyperlinkClass( const std::string &line,
                                                       const std::string &css_class )
{
    const static auto tag = std::string( "<a " );
    const static auto rx  = std::regex( tag );

    auto it = std::sregex_iterator( line.begin(), line.end(), rx );

    if( it != std::sregex_iterator() ) {
        std::stringstream ss;
        ss << line.substr( 0, it->position() + tag.length() )
           << "class=\"" << css_class << "\" "
           << line.substr( it->position()  + tag.length() );
        return  ss.str();
    }

    return line;
}