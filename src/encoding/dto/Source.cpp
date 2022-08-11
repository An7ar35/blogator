#include "Source.h"

using namespace blogator::encoding;

/**
 * Constructor
 * @param in Input stream
 * @param path Filepath of source
 */
Source::Source( std::istream &in, std::filesystem::path path ) :
    _stream( in ),
    _path( std::move( path ) ),
    _format( specs::Format::UNKNOWN ),
    _confidence( specs::Confidence::TENTATIVE ),
    _position( TextPos() )
{}

/**
 * Constructor
 * @param in Input byte stream
 * @param path Filepath of source
 * @param format Known source format (with certainty)
 */
Source::Source( std::istream &in, std::filesystem::path path, specs::Format format ) :
    _stream( in ),
    _path( std::move( path ) ),
    _format( format ),
    _confidence( specs::Confidence::CERTAIN ),
    _position( TextPos() )
{}

/**
 * Gets the stream (will calls peek() to update the stream state)
 * @return Stream
 */
std::istream & Source::stream() {
    _stream.peek();
    return _stream;
}

/**
 * Gets the position tracker
 * @return Position
 */
blogator::TextPos & Source::position() {
    return _position;
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
specs::Format Source::format() const {
    return _format;
}

/**
 * Gets the confidence of the source format
 * @return Confidence
 */
specs::Confidence Source::confidence() const {
    return _confidence;
}

/**
 * Sets the source format
 * @param format Format
 */
void Source::setFormat( specs::Format format ) {
    _format = format;
}

/**
 * Sets the confidence of the source format
 * @param confidence Confidence
 */
void Source::setConfidence( specs::Confidence confidence ) {
    _confidence = confidence;
}