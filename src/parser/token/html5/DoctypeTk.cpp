#include "DoctypeTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
DoctypeTk::DoctypeTk( blogator::parser::TextPos position ) :
    HTML5Tk( specs::html5::TokenType::DOCTYPE, position ),
    _has_name( false ),
    _pid_flag( false ),
    _sid_flag( false ),
    _force_quirks( false )
{}

/**
 * Gets the name
 * @return Name string
 */
std::u32string DoctypeTk::name() const {
    return this->text();
}

/**
 * Gets the public identifier
 * @return Public identifier string
 */
std::u32string DoctypeTk::publicIdentifier() const {
    return _public_id;
}

/**
 * Gets the system identifier
 * @return System identifier string
 */
std::u32string DoctypeTk::systemIdentifier() const {
    return _system_id;
}

/**
 * Gets the name state
 * @return Not missing state
 */
bool DoctypeTk::hasName() const {
    return _has_name;
}

/**
 * Gets the public identifier state
 * @return Not missing state
 */
bool DoctypeTk::hasPID() const {
    return _pid_flag;
}

/**
 * Gets the system identifier state
 * @return Not missing state
 */
bool DoctypeTk::hasSID() const {
    return _sid_flag;
}

/**
 * Gets the 'force-quirks' flag state
 * @return Force-quirks flag state
 */
bool DoctypeTk::forceQuirks() const {
    return _force_quirks;
}

/**
 * Sets the name
 * @param name Name string
 */
void DoctypeTk::setName( std::u32string name ) {
    this->setText( std::move( name ) );
    _has_name = true;
}

/**
 * Sets the public identifier
 * @param pid Public identifier string
 */
void DoctypeTk::setPID( std::u32string pid ) {
    _public_id = std::move( pid );
    _pid_flag = true;
}

/**
 * Sets the system identifier
 * @param sid System identifier string
 */
void DoctypeTk::setSID( std::u32string sid ) {
    _system_id = std::move( sid );
    _sid_flag = true;
}

/**
 * Sets the 'force-quirks' flag
 * @param flag Flag state (default=true)
 */
void DoctypeTk::setForceQuirks( bool flag ) {
    _force_quirks = flag;
}

#ifdef TESTING

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void DoctypeTk::toStr( std::ostream &os ) const {
    os << R"(["DOCTYPE", )";
    if( hasName() ) {
        os << "\"";
        unicode::utf8::convert( os, name() );
        os << "\"";
    } else {
        os << "null";
    }
    os << ", ";
    if( hasPID() ) {
        os << "\"";
        unicode::utf8::convert( os, publicIdentifier() );
        os << "\"";
    } else {
        os << "null";
    }
    os << ", ";
    if( hasSID() ) {
        os << "\"";
        unicode::utf8::convert( os, systemIdentifier() );
        os << "\"";
    } else {
        os << "null";
    }
    os << ", " << ( forceQuirks() ?  "false" : "true" ) //bool values are swapped as html5lib_tests's flag checks 'correct' (i.e. !force-quirks)
       << " ]";
}

#else

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void DoctypeTk::toStr( std::ostream &os ) const {
    os << "html5::DoctypeTk={ name: ";
    if( hasName() ) {
        os << "\"";
        unicode::utf8::convert( os, name() );
        os << "\"";
    } else {
        os << "missing";
    }
    os << ", pid: ";
    if( hasPID() ) {
        os << "\"";
        unicode::utf8::convert( os, publicIdentifier() );
        os << "\"";
    } else {
        os << "missing";
    }
    os << ", sid: ";
    if( hasSID() ) {
        os << "\"";
        unicode::utf8::convert( os, systemIdentifier() );
        os << "\"";
    } else {
        os << "missing";
    }
    os << ", force-quirks: " << forceQuirks() << ", position: " << lineNum() << ":" << colPos() << " }";
}

#endif
