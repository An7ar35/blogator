#include "MsgInterface.h"

/**
 * Gets the interface instance
 * @return Interface instance
 */
blogator::cli::MsgInterface & blogator::cli::MsgInterface::getInstance() {
    static MsgInterface instance;
    return instance;
}

/**
 * Constructor
 */
blogator::cli::MsgInterface::MsgInterface() :
    _display_out( std::make_unique<MsgDisplay>() )
{}

/**
 * Begins a workload progress not yet known step count
 * @param process_name Workload process name
 * @param next_step    First workload step's description
 */
void blogator::cli::MsgInterface::begin( const std::string & process_name,
                                         const std::string & next_step ) {
    if( _current_step < _total_steps )
        _display_out->done();

    _display_out->flushBuffer();
    _display_out->newProcess( process_name, next_step );
    _current_step = 0;
    _total_steps  = 0;
}

/**
 * Begins a workload progress
 * @param process_name Workload process name
 * @param steps        Number of steps in the workload
 * @param next_step    First workload step's description
 */
void blogator::cli::MsgInterface::begin( const std::string &process_name,
                                         const unsigned &steps,
                                         const std::string &next_step )
{
    if( _current_step < _total_steps )
        _display_out->done();

    _display_out->flushBuffer();
    _display_out->newProcess( process_name, next_step );
    _current_step = 0;
    _total_steps  = steps;
}

/**
 * Adds extra steps to the current progress
 * @param steps Workload steps to add
 */
void blogator::cli::MsgInterface::addSteps( const unsigned & steps ) {
    _total_steps += steps;
}

/**
 * Force-ends the progress
 * @param msg Step status message
 */
void blogator::cli::MsgInterface::end( const std::string &msg ) {
    _current_step = _total_steps;
    _display_out->update( msg, ( double ) _current_step / ( double ) _total_steps );
}

/**
 * Gets the current total of the progress
 * @return Progress as a fraction (0-1)
 */
double blogator::cli::MsgInterface::total() const {
    return (double) _current_step / (double) _total_steps;
}

/**
 * Gets the active state of the current progress
 * @return Active state
 */
bool blogator::cli::MsgInterface::isActive() const {
    return _current_step > 0 && _current_step <= _total_steps;
}

/**
 * Increments progress by 1
 */
void blogator::cli::MsgInterface::progress() {
    ++_current_step;
    _display_out->update( (double) _current_step / (double) _total_steps );
}

/**
 * Increments progress by 1 and changes the step description
 * @param next_step Step description
 */
void blogator::cli::MsgInterface::progress( const std::string &next_step ) {
    ++_current_step;
    _display_out->update( next_step, (double) _current_step / (double) _total_steps );
}

/**
 * Increments progress by n steps and changes the step description
 * @param next_step  Step description
 * @param step_count Increment amount
 */
void blogator::cli::MsgInterface::progress( const std::string & next_step, const unsigned & step_count ) {
    _current_step += step_count;
    _display_out->update( next_step, (double) _current_step / (double) _total_steps );
}


void blogator::cli::MsgInterface::resetLinePos() {
    _display_out->done();
}

void blogator::cli::MsgInterface::debug( const std::string & msg ) {
    _display_out->debug( msg );
}

void blogator::cli::MsgInterface::msg( const std::string & msg ) {
    _display_out->message( msg );
}

void blogator::cli::MsgInterface::msg( const std::string &msg,
                                       bool cond,
                                       const std::string &yes,
                                       const std::string &no )
{
    _display_out->message( msg, cond, yes, no );
}

void blogator::cli::MsgInterface::warning( const std::string & msg ) {
    _display_out->warning( msg );
}

void blogator::cli::MsgInterface::error( const std::string & msg ) {
    _display_out->error( msg );
}

void blogator::cli::MsgInterface::setShowDebug( bool state ) {
    _display_out->setDebug( state );
    _display_out->message( std::string( "Show debug messages: " ) + ( state ? "ON" : "OFF" ) );
}