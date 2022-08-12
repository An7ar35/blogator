#include "helpers.h"

#include <iostream>
#include <fstream>

#include "../../src/unicode/utf8.h"
#include "../../src/encoding/dto/Source.h"
#include "../../src/encoding/Transcode.h"

/**
 * Loads a json file into a JSON object
 * @param path Source filepath
 * @return JSON
 */
nlohmann::json test_harness::loadJSONFromFile( const std::filesystem::path &path ) {
    std::cout << "Loading test file: " << path << std::endl;
    std::fstream file;
    file.open( path, std::ios::in );
    auto json = nlohmann::json();
    file >> json;
    file.close();
    return json;
}

/**
 * Gets all '.*' files found in a directory path
 * @param test_dir Directory path
 * @param file_ext File extension of the test files to look for
 * @return Collection of test file paths
 */
std::set<std::filesystem::path> test_harness::getTestFiles( const std::filesystem::path &test_dir, const std::string & file_ext ) {
    auto test_files = std::set<std::filesystem::path>();

    for( const std::filesystem::path & file : std::filesystem::directory_iterator( test_dir ) ) {
        if( file.extension() == file_ext && file.stem() != "package" ) { //avoids `package.json` file that sometimes lives in the root of some git repos
            test_files.emplace( file );
        }
    }

    return test_files;
}

/**
 * Uses Blogator's transcoder to convert the source U8 test input into U32
 * @param raw  Original raw UTF-8 text
 * @param path Source path
 * @return Processed/checked UTF-32 text
 * @throws std::runtime_error when transcoding fails
 */
blogator::U32Text test_harness::transcodeInput( const std::string &raw, const std::filesystem::path &path ) {
    std::vector<char32_t> out;
    std::stringstream     ss;

    ss << raw;

    auto source = blogator::encoding::Source( ss, path, blogator::encoding::specs::Format::UTF8 );

    if( !blogator::encoding::Transcode::convert( source, out ) ) {
        throw std::runtime_error( "Failed to transcode u8 source data to u32" );
    }

    return { path, out };
}

/**
 * Prints a UTF-32 character buffer to a UTF-8 stream
 * @param os Output stream
 * @param u32text U32 text
 */
void test_harness::printU32Buffer( std::ostream &os, blogator::U32Text & u32text ) {
    while( !u32text.reachedEnd() ) {
        blogator::unicode::utf8::convert( os, u32text.character() );
        u32text.nextChar();
    }

    u32text.reset();
}