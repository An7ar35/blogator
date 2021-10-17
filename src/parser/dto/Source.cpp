#include "Source.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param in Input stream
 * @param path Filepath of source
 */
Source::Source( std::istream &in, std::filesystem::path path ) :
    _stream( in ),
    _path( std::move( path ) ),
    _format( encoding::Format_e::UNKNOWN ),
    _confidence( encoding::Confidence_e::TENTATIVE )
{}

/**
 * Constructor
 * @param in Input stream
 * @param path Filepath of source
 * @param format Known source format (with certainty)
 */
Source::Source( std::istream &in, std::filesystem::path path, encoding::Format_e format ) :
    _stream( in ),
    _path( std::move( path ) ),
    _format( format ),
    _confidence( encoding::Confidence_e::CERTAIN )
{}

/**
 * Gets the stream
 * @return Stream
 */
std::istream &Source::stream() {
    return _stream;
}

/**
 * Gets the source path
 * @return Source path
 */
const std::filesystem::path &Source::path() const {
    return _path;
}

/**
 * Gets the source format
 * @return Source format
 */
encoding::Format_e Source::format() const {
    return _format;
}

/**
 * Gets the confidence of the source format
 * @return Confidence
 */
encoding::Confidence_e Source::confidence() const {
    return _confidence;
}

/**
 * Sets the source format
 * @param format Format
 */
void Source::setFormat( encoding::Format_e format ) {
    _format = format;
}

/**
 * Sets the confidence of the source format
 * @param confidence Confidence
 */
void Source::setConfidence( encoding::Confidence_e confidence ) {
    _confidence = confidence;
}
