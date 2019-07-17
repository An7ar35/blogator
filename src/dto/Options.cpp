#include "Options.h"
#include "../cli/MsgInterface.h"

const char* blogator::dto::Options::BLOGATOR_NAME    = "Blogator";
const char* blogator::dto::Options::BLOGATOR_VERSION = "0.1a";
const char* blogator::dto::Options::BLOGATOR_URL     = "https://an7ar35.bitbucket.io"; //TODO update when a proper 'Blogator' page is created

/**
 * Constructor
 */
blogator::dto::Options::Options() :
    RUN_TIMESTAMP( std::chrono::system_clock::now() ),
    BLOGATOR_SIGNATURE(
        "<!-- Generated with " + std::string( BLOGATOR_NAME ) + " " + std::string( BLOGATOR_VERSION ) + " (" + std::string( BLOGATOR_URL ) + ") -->"
    )
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
        datestamp  = std::make_unique<DateStamp>( local->tm_year + 1900, local->tm_mon, local->tm_mday );
    }

    return *datestamp;
}