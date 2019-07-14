#include "MsgDisplay.h"
#include <iostream>
#include <unistd.h>
#include <eadlib/cli/colour.h>

/**
 * Constructor
 */
blogator::cli::MsgDisplay::MsgDisplay() :
    _progress_bar( BAR_WIDTH, CHAR_EMPTY ),
    _old_cerr_buffer( std::cerr.rdbuf( _err_buffer.rdbuf() ) )
{}

/**
 * Destructor
 */
blogator::cli::MsgDisplay::~MsgDisplay() {
    using eadlib::cli::BGColour;
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
    using eadlib::cli::format;
    std::cout << "\033[2B" << "\n"
              << format<FGColour::RED, Decoration::BOLD>( _err_buffer.str() )
              << std::endl;
    std::cerr.rdbuf( _old_cerr_buffer );
}

/**
 * Begins a new process
 * @param process   Process name
 * @param next_step Next step's name in the process's workload
 */
void blogator::cli::MsgDisplay::newProcess( const std::string & process, const std::string &next_step ) {
    _curr_process = process;
    _curr_step    = next_step;
    std::cout << std::endl;
    update( _curr_step, 0 );
}

/**
 * Update the progress of the current workload step
 * @param progress Fractional progress of the workload
 */
void blogator::cli::MsgDisplay::update( const double &progress ) {
    updateDisplay( progress );
}

/**
 * Update the progress of the current workload process
 * @param next_step Description of the next step in the workload
 * @param progress  Fractional progress of the workload
 */
void blogator::cli::MsgDisplay::update( const std::string &next_step, const double &progress ) {
    _curr_step = next_step;
    updateDisplay( progress );
}


void blogator::cli::MsgDisplay::done() {
    std::cout << "\033[2B";
}

void blogator::cli::MsgDisplay::log( const std::string & msg ) {
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
    _err_buffer << eadlib::cli::format<FGColour::MAGENTA, Decoration::BOLD>( "[LOG] " )
                << eadlib::cli::format<FGColour::MAGENTA, Decoration::FAINT>( msg ) << "\33[31;1m\n";
}

void blogator::cli::MsgDisplay::message( const std::string & msg ) {
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
    _err_buffer << eadlib::cli::format<FGColour::CYAN, Decoration::BOLD>( "[MESSAGE] " )
                << eadlib::cli::format<FGColour::CYAN>( msg ) << "\33[31;1m\n";
}

void blogator::cli::MsgDisplay::warning( const std::string & msg ) {
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
    _err_buffer << eadlib::cli::format<FGColour::YELLOW, Decoration::BOLD>( "[WARNING] " )
                << eadlib::cli::format<FGColour::YELLOW, Decoration::BOLD>( msg ) << "\33[31;1m\n";
}

void blogator::cli::MsgDisplay::error( const std::string & msg ) {
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
    _err_buffer << eadlib::cli::format<FGColour::RED, Decoration::BOLD>( "[ERROR] " )
                << eadlib::cli::format<FGColour::RED, Decoration::BOLD>( msg ) << "\33[31;1m\n";
}

void blogator::cli::MsgDisplay::flushErrorsToDisplay() {
    using eadlib::cli::BGColour;
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
    std::cout << _err_buffer.str() << std::endl;
}

/**
 * Writes the description/progress bar to the output
 * @param progress Fractional progress of the workload
 */
void blogator::cli::MsgDisplay::updateDisplay( const double & progress ) {
    using eadlib::cli::BGColour;
    using eadlib::cli::FGColour;
    using eadlib::cli::Decoration;
//    sleep( 1 ); //TODO remove (debug only)
    std::cout << "\r> " << _curr_process << ": "
              << eadlib::cli::format<FGColour::BLUE, Decoration::BOLD>( _curr_step )
              << "\033[K\n";
    //Graphic progress
    if( BAR_WIDTH > 0 ) {
        auto position = (size_t) (progress * BAR_WIDTH);
        std::stringstream ss;
        ss << "[";
        for( size_t i = 1; i < position; i++ ) {
            ss << "=";
        }
        if( position < BAR_WIDTH ) {
            ss << "|";
            for( size_t i = position; i < BAR_WIDTH - 1; i++ ) {
                ss << CHAR_EMPTY;
            }
        }
        ss << "]";
        std::cout << ss.str() << "";
    }
    //Numeric progress
    if( (unsigned)(progress * 100) == 100 ) {
        std::cout << "100";
        if( PRECISION > 0 ) {
            std::cout << ".";
            for( size_t i = 0; i < PRECISION; i++ ) {
                std::cout << "0";
            }
        }
        std::cout << "\% \b\n";
    } else {
        if( progress < 0.1 ) {
            std::cout << " ";
        }
        if( progress < 1 ) {
            std::cout << " ";
        }
        std::cout << std::fixed;
        std::cout << std::setprecision( PRECISION ) << progress * 100 << "\% \b\033[K";
    }
    std::cout << "\033[1A";
    std::cout << std::flush;
}