#include "helper.h"

#include <fstream>
#include <iostream>

/**
 * JSON-ify a collection of HTML5 tokens into a stream
 * @param os Output stream
 * @param tokens Tokens
 * @return Output stream
 */
std::ostream &blogator::tests::jsonifyHtml5Tokens( std::ostream &os, const std::vector<std::unique_ptr<blogator::parser::token::html5::HTML5Tk>> &tokens ) {
    os << "[";

    for( auto it = tokens.cbegin(); it != tokens.cend(); ++ it ) {
        os << *(*it);
        if( std::next( it ) != tokens.cend() ) {
            os << ", ";
        }
    }

    os << "]";
    return os;
}

/**
 * JSON-ify a collection of HTML5 error objects into a stream
 * @param os Output stream
 * @param err Error objects
 * @return Output stream
 */
std::ostream & blogator::tests::jsonifyHtml5Errors( std::ostream & os, const std::vector<blogator::parser::logging::ErrorObject> &err ) {
    os << "[";

    for( auto it = err.cbegin(); it != err.cend(); ++ it ) {
        auto code = it->error();
        auto pos  = it->textpos();
        std::replace( code.begin(), code.end(), ' ', '-' );
        os << R"({ "code": ")" << code << R"(", "line": )" << pos.line << ",  \"col\": " << pos.col << " }";
        if( std::next( it ) != err.cend() ) {
            os << ", ";
        }
    }

    os << "]";
    return os;
}

/**
 * Gets all '.test' files found in a directory path
 * @param test_dir Directory path
 * @return Collection of test file paths
 */
std::vector<std::filesystem::path> blogator::tests::getTestFiles( const std::filesystem::path &test_dir ) {
    auto test_files = std::vector<std::filesystem::path>();

    for (const std::filesystem::path & file : std::filesystem::directory_iterator( test_dir ) ) {
        if( file.extension() == ".test" ) {
            test_files.push_back( file );
        }
    }

    return test_files;
}

/**
 * Loads a json file into a JSON object
 * @param path Source filepath
 * @return JSON
 */
nlohmann::json blogator::tests::loadJSON( const std::filesystem::path &path ) {
    std::cout << "Loading test file: " << path << std::endl;
    std::fstream file;
    file.open( path, std::ios::in );
    auto json = nlohmann::json();
    file >> json;
    file.close();
    return json;
}

/**
 * Loads and add all tests from found files
 * @param test_dir Directory path
 * @return Collection of { test, source path }
 */
std::vector<std::pair<nlohmann::json, std::filesystem::path>> blogator::tests::loadJSONTests( const std::filesystem::path &test_dir ) {
    std::vector<std::pair<nlohmann::json, std::filesystem::path>> test_collection;

    auto test_files = getTestFiles( test_dir );

    for( const auto & file : test_files ) {
        auto source = loadJSON( file );

        if( !source.contains( "tests" ) ) {
            std::cerr << "Can't find root key 'tests' in source json file: " << file << std::endl;
        } else {
            std::cout << "Adding tests from: " << file << std::endl;

            for( const auto & test : source.at( "tests" ) ) {
                test_collection.emplace_back( std::make_pair( test, file ) );
            }
        }
    }

    return test_collection;
}
