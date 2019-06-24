#include "html.h"

#include <filesystem>

/**
 * Creates a stylesheet link for a html <head>
 * @param css_path Path of the css file
 * @return Formatted link as a string
 */
std::string blogator::html::createStylesheetLink( const std::filesystem::path &css_loc, const std::filesystem::path &out_dir ) {
    auto css_path = css_loc.parent_path().lexically_relative( out_dir.parent_path() ) / css_loc.filename();
    return "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"" + css_path.string() + "\"/>";
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