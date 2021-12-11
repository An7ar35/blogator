#include "LogFormatter.h"

using namespace blogator::logger::formatter;

/**
 * Constructor
 * @param type Formatter type
 */
LogFormatter::LogFormatter( LogFormatterType type ) :
    _type( type )
{}

/**
 * Gets the formatter type
 * @return LogFormatterType enum
 */
LogFormatterType LogFormatter::type() const {
    return _type;
}
