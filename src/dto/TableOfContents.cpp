#include "TableOfContents.h"

#include <regex>
#include <stack>

/**
 * Heading formatted printer helper function
 * @return Heading as a formatted string
 */
std::string blogator::dto::TableOfContents::Heading::print() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

/**
 * Heading ID printer helper function
 * @param os Output stream
 * @return Heading ID as a formatted string
 */
std::string blogator::dto::TableOfContents::Heading::printID() const {
    std::stringstream ss;
    ss << "heading_";
    for( auto it = numbering.cbegin(); it != numbering.cend(); ++it ) {
        ss << *it;
        if( it != std::prev( numbering.cend() ) )
            ss << "_";
    }
    return ss.str();
}

/**
 * Gets the numbering depth for the heading
 * @return Numbering depth
 */
int blogator::dto::TableOfContents::Heading::depth() const {
    return numbering.size();
}

/**
 * Equality comparison operator
 * @param rhs Heading to compare to
 * @return Equality state
 */
bool blogator::dto::TableOfContents::Heading::operator ==( const TableOfContents::Heading &rhs ) const {
    if( level == rhs.level && str == rhs.str && numbering.size() == rhs.numbering.size() ) {
        for( auto it1 = numbering.cbegin(), it2 = rhs.numbering.cbegin(); it1 != numbering.cend(); ++it1, ++it2 ) {
            if( *it1 != *it2 )
                return false;
        }
        return true;
    }
    return false;
}

/**
 * Negative equality comparison operator
 * @param rhs Heading to compare to
 * @return Not-equal state
 */
bool blogator::dto::TableOfContents::Heading::operator !=( const TableOfContents::Heading &rhs ) const {
    return !( *this == rhs );
}

/**
 * Constructor
 */
blogator::dto::TableOfContents::TableOfContents() :
    _min_level( 0 ),
    _max_level( MAX_HEADING_LEVEL ),
    _root_level( MAX_HEADING_LEVEL ),
    _leaf_level( 0 )
{}

/**
 * Constructor
 * @param max_levels Maximum number of heading levels for the ToC
 * @param offset     Heading level offset
 * @throws std::invalid_argument when offset exceeds the highest heading level available
 */
blogator::dto::TableOfContents::TableOfContents( int max_levels, int offset ) :
    _root_level( MAX_HEADING_LEVEL ),
    _leaf_level( 0 )
{
    _min_level = offset + 1;
    auto max_level = max_levels + offset;
    _max_level = ( max_level > MAX_HEADING_LEVEL ? MAX_HEADING_LEVEL : max_level );

    if( _max_level < _min_level ) {
        std::stringstream ss;
        ss << "[blogator::dto::TableOfContents::setProperties( " << max_levels << ", " << offset << " )] "
           << "Offset exceeds the max heading level available (<h6></h6>).";
        throw std::invalid_argument( ss.str() );
    }
}

/**
 * Find a single qualifying heading on a given line and add it to the Table of Contents
 * @param line_number Line number from the HTML source
 * @param line        Line of HTML from source
 */
void blogator::dto::TableOfContents::findHeading( const size_t &line_number, const std::string &line )
{
    auto s = R"(^(.*?<\s*h([1|2|3|4|5|6]){1})\s*>(.*)(?:</h\2){1}>)"; //grp#2: heading level, grp#3: heading str
    auto rx = std::regex( s );
    auto it = std::sregex_iterator( line.begin(), line.end(), rx );

    while( it != std::sregex_iterator() ) {
        try {
            auto level_i = stoi( it->str( 2 ) );

            if( level_i >= _min_level && level_i <= _max_level ) {
                if( level_i < _root_level )
                    _root_level = level_i;
                if( level_i > _leaf_level )
                    _leaf_level = level_i;

                auto level_e = static_cast<TableOfContents::HeadingLevel>( level_i );

                _heading_insert_pos.insert(
                    { dto::InsertPosition( line_number, it->str( 1 ).length() ), Heading( level_e, it->str( 3 ) ) }
                );
            }

        } catch( std::invalid_argument &e ) {
            std::stringstream ss;
            ss << "[blogator::dto::TableOfContents::findHeading( " << line_number << ", \"" << line << "\" )] "
               << "Could not convert heading level '" << it->str( 2 ) << "' to an integer.";
            throw std::invalid_argument( ss.str() );
        }
        ++it;
    }
}

/**
 * Finds a qualifying ToC insert block on a given line and add it to the list of insertion points for ToC blocks
 * @param line_number Line number from the HTML source
 * @param line        Line of HTML from source
 * @param css_class   CSS class used to denote a ToC insertion block in the HTML source
 */
void blogator::dto::TableOfContents::findTocBlock( const size_t &line_number,
                                                   const std::string &line,
                                                   const std::string &css_class )
{
    auto s  = R"(^(.*<\s*[a-zA-Z\d]+\s*class\s*=\s*"[a-zA-Z-_\s]*)" + css_class + R"((?:\s[a-zA-Z-_\s]*"|")>\s*)(?:</[a-zA-Z\d]+>))";
    auto rx = std::regex( s );
    auto it = std::sregex_iterator( line.begin(), line.end(), rx );

    while( it != std::sregex_iterator() ) {
        _toc_insert_pos.emplace_back( dto::InsertPosition( line_number, it->str( 1 ).length() ) );
        ++it;
    }
}

/**
 * Generates all the numbering and level information for all the headings
 */
void blogator::dto::TableOfContents::finaliseToC() {
    generateNumbering( _heading_insert_pos );
}


/**
 * Gets the lowest heading level allowed in the ToC
 * @return Lowest heading level
 */
blogator::dto::TableOfContents::HeadingLevel blogator::dto::TableOfContents::minHeadingLevel() const {
    return static_cast<HeadingLevel>( _min_level );
}

/**
 * Gets the highest heading level allowed in the ToC
 * @return Highest heading level
 */
blogator::dto::TableOfContents::HeadingLevel blogator::dto::TableOfContents::maxHeadingLevel() const {
    return static_cast<HeadingLevel>( _max_level );
}

/**
 * Gets the root heading level (lowest) of the current list of headings in the ToC
 * @return ToC root heading level
 */
blogator::dto::TableOfContents::HeadingLevel blogator::dto::TableOfContents::rootHeadingLevel() const {
    return static_cast<HeadingLevel>( _root_level );
}

/**
 * Gets the leaf heading level (highest) of the current list of headings in the ToC
 * @return ToC outer-leaf heading level
 */
blogator::dto::TableOfContents::HeadingLevel blogator::dto::TableOfContents::leafHeadingLevel() const {
    return static_cast<HeadingLevel>( _leaf_level );
}

/**
 * Gets the headings for the ToC
 * @return R/O access to the heading of the ToC
 */
const std::map<blogator::dto::InsertPosition, blogator::dto::TableOfContents::Heading> &blogator::dto::TableOfContents::headings() const {
    return _heading_insert_pos;
}

/**
 * Gets the insertion location for any table of contents block found in the HTML source
 * @return R/O access to the ToC insertion point(s) in the HTML source
 */
const std::list<blogator::dto::InsertPosition> &blogator::dto::TableOfContents::tocPositions() const {
    return _toc_insert_pos;
}

/**
 * Gets the state of existence of a Table of Content insertion location
 * @return ToC insertion position existence
 */
bool blogator::dto::TableOfContents::tocBlockExists() const {
    return !_toc_insert_pos.empty();
}

/**
 * Generates the numbering scheme for the headings
 * @param headings Collection of headings in order
 */
void blogator::dto::TableOfContents::generateNumbering( std::map<InsertPosition, Heading> &headings ) const {
    auto total_depth = static_cast<int>( leafHeadingLevel() ) - static_cast<int>( rootHeadingLevel() ) + 1;
    auto numbering   = std::vector<int>( total_depth, 0 );
    int  last_depth  = 0;

    for( auto &h : headings ) {
        auto depth = static_cast<int>( h.second.level ) - static_cast<int>( rootHeadingLevel() );

        if( depth < last_depth )
            for( auto it = std::next( numbering.begin(), depth + 1 ); it != numbering.end(); ++it )
                *it = 0;

        ++numbering.at( depth );

        for( int i = 0; i <= depth; ++i )
            h.second.numbering.emplace_back( numbering.at( i ) );

        last_depth = depth;
    }
}