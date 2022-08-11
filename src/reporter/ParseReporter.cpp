#include "ParseReporter.h"

#include <iostream> //TODO temp/remove
#include <utility>

#include "../logger/Logger.h"

using namespace blogator::reporter;

ParseReporter ParseReporter::_instance = ParseReporter();

/**
 * Destructor
 */
ParseReporter::~ParseReporter() {
    ParseReporter::flush();
}

/**
 * Sets the buffering (turning it off will dispatch error items to all callbacks as they come)
 * @param flag On: true (default), Off: false
 */
void ParseReporter::setBuffering( bool flag ) {
    LOG_DEBUG(
        "[reporter::ParseReporter::setBuffering( ", ( flag ? "true" : "false" ), " )] "
        "Buffering turned ", ( flag ? "ON" : "OFF" )
    );

    ParseReporter::_instance._buffering = flag;
}

/**
 * [THREAD-SAFE] Attaches the primary output callback where ErrorObjects can be sent to
 * @param cb Callback method to add as primary
 */
void ParseReporter::attachOutputCallback( ParseReporter::OutputCallback_f cb ) {
    ParseReporter::_instance.attachPrimaryOutputCb( std::move( cb ) );
}

/**
 * [THREAD-SAFE] Detaches the primary output callback only
 */
void ParseReporter::detachOutputCallback() {
    ParseReporter::_instance.detachPrimaryOutputCb();
}

/**
 * [THREAD-SAFE] Adds a named output callback
 * @param name Name
 * @param cb Output callback
 * @return Success
 */
bool ParseReporter::appendOutputCallback( const std::string &name, ParseReporter::OutputCallback_f cb ) {
    return ParseReporter::_instance.appendOutputCb( name, std::move( cb ) );
}

/**
 * [THREAD-SAFE] Removes a named output callback
 * @param name Name
 * @return Success
 */
bool ParseReporter::detachOutputCallback( const std::string &name ) {
    return ParseReporter::_instance.removeOutputCb( name );
}

/**
 * [THREAD-SAFE] Logs a message
 * @param err_obj Error object
 */
void ParseReporter::log( ReporterObject && err_obj  ) {
    ParseReporter::_instance.logErrorObject( std::move( err_obj ) );
}

/**
 * [THREAD-SAFE] Logs a message
 * @param src Source filepath
 * @param ctx Error Context enum
 * @param err_code Error code within the context
 * @param position Text position
 */
void ParseReporter::log( std::filesystem::path src, reporter::Context ctx, int err_code, TextPos position ) {
    ParseReporter::_instance.logErrorObject( ReporterObject( std::move( src ), ctx, err_code, position ) );
}

/**
 * [THREAD-SAFE] Logs a message
 * @param src Source filepath
 * @param ctx Error Context enum
 * @param err_code Error code within the context
 * @param txt Optional text to append to the error message
 * @param position Text position
 */
void ParseReporter::log( std::filesystem::path src, reporter::Context ctx, int err_code, std::string txt, TextPos position ) {
    ParseReporter::_instance.logErrorObject( ReporterObject( std::move( src ), ctx, err_code, position, std::move( txt ) ) );
}

/**
 * [THREAD-SAFE] Flushes every error message pools
 */
void ParseReporter::flush() {
    ParseReporter::_instance.flushPools();
}

/**
 * [THREAD-SAFE] Flushes all error messages mapped to a given file path
 * @param path File path
 */
void ParseReporter::flush( const std::filesystem::path &path ) {
    ParseReporter::_instance.flushPool( path );
}

/**
 * [PRIVATE/THREAD-SAFE] Attaches a primary output callback
 * @param cb Output callback
 */
void ParseReporter::attachPrimaryOutputCb( ParseReporter::OutputCallback_f cb ) {
    std::lock_guard<std::mutex> guard( _mutex );
    _output_cb = std::move( cb );
}

/**
 * [PRIVATE/THREAD-SAFE] Detaches the primary output callback
 */
void ParseReporter::detachPrimaryOutputCb() {
    std::lock_guard<std::mutex> guard( _mutex );
    _output_cb = nullptr;
}

/**
 * [PRIVATE/THREAD-SAFE] Adds a named output callback
 * @param name Name
 * @param cb Output callback
 * @return Success
 */
bool ParseReporter::appendOutputCb( const std::string &name, ParseReporter::OutputCallback_f cb ) {
    std::lock_guard<std::mutex> guard( _mutex );
    return _outputs.try_emplace( name, std::move( cb ) ).second;
}

/**
 * [PRIVATE/THREAD-SAFE] Removes a named output callback
 * @param name Name
 * @return Success
 */
bool ParseReporter::removeOutputCb( const std::string &name ) {
    std::lock_guard<std::mutex> guard( _mutex );
    return ( _outputs.erase( name ) > 0 );
}

/**
 * [PRIVATE/THREAD-SAFE] Logs an error
 * @param err Error object
 */
void ParseReporter::logErrorObject( ReporterObject &&err ) {
    std::lock_guard<std::mutex> guard( _mutex );

    if( _buffering ) {
        auto result = _pools.try_emplace( err.filepath(), ErrorPool_t() );
        auto &pool = result.first->second;

        pool.emplace_back( std::move( err ) );

    } else {
        dispatch( err );
    }
}

/**
 * [PRIVATE/THREAD-SAFE] Flushes an error pool
 * @param path Source path (key) of pool
 */
void ParseReporter::flushPool( const std::filesystem::path &path ) {
    std::lock_guard<std::mutex> guard( _mutex );

    auto it = _pools.find( path );

    if( it != _pools.end() ) {
        LOG_DEBUG( "[reporter::ParseReporter::flushPool( ", path, " )] Flushing error pool (", it->second.size(), " items)" );

        for( const auto & err : it->second ) {
            dispatch( err );
        }

        _pools.erase( it );
    }
}

/**
 * [PRIVATE/THREAD-SAFE] Flushes all error pools buffered
 */
void ParseReporter::flushPools() {
    std::lock_guard<std::mutex> guard( _mutex );

    for( const auto & pool : _pools ) {
        LOG_DEBUG( "[reporter::ParseReporter::flushPools()] Flushing error pool for: ", pool.first, " (", pool.second.size(), " items)" );

        for( const auto & err : pool.second ) {
            dispatch( err );
        }
    }

    _pools.clear();
}

/**
 * [PRIVATE] Dispatches an error
 * @param err Error object
 */
void ParseReporter::dispatch( const ReporterObject &err ) {
    if( _output_cb ) {
        _output_cb( err );
    }

    for( auto & [str, cb] : _outputs ) {
        cb( err );
    }

    //TODO output file -> passed via global config?

    std::cout << "{\n"
              << "\tsource: " << err.filepath() << "\n"
              << "\tposition: " << err.position() << "\n"
              << "\tcontext: " << err.context() << "\n"
              << "\terror: " << err.error() << "\n"
              << "\tdetailed: " << err.detailed() << "\n"
              << "}\n";
}

//TODO map of sources -> ordered collection of errors
//     this would need a call to flush the source's errors (if any) when the parsing/dom builder thread finishes (on EOF?)