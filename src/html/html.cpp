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