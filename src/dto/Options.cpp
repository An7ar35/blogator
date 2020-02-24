#include "Options.h"
#include "../cli/MsgInterface.h"

/**
 * Constructor
 * @param info Software information
 */
blogator::dto::Options::Options() :
    RUN_TIMESTAMP( std::chrono::system_clock::now() )
{}

/**
 * Sets the root path and generates all absolute paths from it
 * @param root_path Root directory path
 */
void blogator::dto::Options::setupAbsolutePaths( const std::filesystem::path & root_path ) {
    auto &display = cli::MsgInterface::getInstance();
    display.debug( "Working directory ...: " + root_path.string() );

    _paths.root_dir         = root_path;
    _paths.template_dir     = _paths.root_dir / _folders.templates.root;
    _paths.source_dir       = _paths.root_dir / _folders.source.root;
    _paths.posts_dir        = _paths.root_dir / _folders.posts.root;
    _paths.css_dir          = _paths.root_dir / _folders.css.root;
    _paths.index_dir        = _paths.root_dir / _folders.index.root;
    _paths.index_date_dir   = _paths.root_dir / _folders.index.by_date;
    _paths.index_year_dir   = _paths.root_dir / _folders.index.by_year;
    _paths.index_tag_dir    = _paths.root_dir / _folders.index.by_tag;
    _paths.index_author_dir = _paths.root_dir / _folders.index.by_author;
}

/**
 * Sets the temporary directory path
 * @param temp_path Temp directory
 */
void blogator::dto::Options::setTempPath( const std::filesystem::path &temp_path ) {
    auto &display = cli::MsgInterface::getInstance();
    display.debug( "Temporary directory .: " + temp_path.string() );

    _paths.temp_dir = temp_path;
}

/**
 * Gets the software's name string
 * @return Software name
 */
std::string blogator::dto::Options::getSoftwareNameStr() const {
    return "Blogator";
}

/**
 * Gets the software's version string
 * @return Software version
 */
std::string blogator::dto::Options::getSoftwareVersionStr() const {
    return blogator::cmake::VERSION;
}

/**
 * Gets the software's signature line for injecting into generated HTML files
 * @return Signature string
 */
std::string blogator::dto::Options::getSoftwareSignatureStr() const {
    using blogator::cmake::VERSION;
    using blogator::cmake::URL;
    static const auto str = "<!-- Generated with Blogator " + VERSION + " (" + URL + ") -->";
    return str;
}

/**
 * Gets the program's run time-stamp
 * @return Timestamp as a RFC 822 compliant string
 */
std::string blogator::dto::Options::getRunTimeStamp() const {
    auto now = std::chrono::system_clock::to_time_t( RUN_TIMESTAMP );
    std::stringstream ss;
    ss << std::put_time( std::localtime( &now ), "%a, %d %b %Y %T %z" ); //RFC 822
    return ss.str();
}

/**
 * Gets the program's runtime date-stamp (dd/mm/yyyy)
 * @return Reference to DateStamp DTO
 */
const blogator::dto::DateStamp & blogator::dto::Options::getRuntimeDateStamp() const {
    static std::unique_ptr<DateStamp> datestamp;

    if( !datestamp ) {
        auto now   = std::chrono::system_clock::to_time_t( RUN_TIMESTAMP );
        auto local = std::localtime( &now );
        datestamp  = std::make_unique<DateStamp>( local->tm_year + 1900, local->tm_mon + 1, local->tm_mday );
    }

    return *datestamp;
}