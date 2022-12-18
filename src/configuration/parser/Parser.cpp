#include "Parser.h"

#include "../../string/helpers.h"
#include "../../logger/Logger.h"
#include "../../exception/parsing_failure.h"
#include "../../unicode/utf8.h"
#include "../../unicode/utf32.h"
#include "../../reporter/ParseReporter.h"
#include "specs/ErrorCode.h"

using namespace blogator::configuration;
using           blogator::configuration::parser::specs::ErrorCode;

template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_KEY>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_KEYS>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::IN_KEY>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::AFTER_KEY>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_VALUES>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_VALUE>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::IN_NAMED_VALUE>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::AFTER_VALUE>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::AFTER_SCOPE>( const std::unique_ptr<token::ConfigTk> & token );
template<> void parser::Parser::processContent<parser::specs::InsertionMode::END_OF_FILE>( const std::unique_ptr<token::ConfigTk> & token );

/**
 * Constructor
 */
parser::Parser::Parser() :
    _errors( 0 )
{}

/**
   Initialises the parser to an initial state
 * @param path Source file path
 */
void parser::Parser::init( std::filesystem::path path ) {
    _src_path = std::move( path );
    _errors   = 0;
    _config = std::make_unique<Configuration>();
    setInsertionMode( InsertionMode_e::BEFORE_KEYS );
}

/**
 * Process token
 * @param token Configuration token
 * @throws blogator::exception::parsing_failure when token is dispatched after EOF is reached or parser is not initialised
 */
void parser::Parser::dispatch( std::unique_ptr<token::ConfigTk> token ) {
    if( !_config ) {
        throw exception::parsing_failure(
            "[configuration::parser::Parser::dispatch( std::unique_ptr<token::ConfigTk> )] "
            "Parser is un-initialised."
        );
    }

    if( !token ) {
        LOG_ERROR(
            "[configuration::parser::Parser::dispatch( std::unique_ptr<token::ConfigTk> )] "
            "NULL token passed - skipped."
        );

        return; //EARLY RETURN
    }

//    LOG_CRITICAL( "Mode : ", currentInsertionMode(), ": ", *token ); //TODO temp

    if( _curr_token ) {
        _prev_token = std::move( _curr_token );
    }

    _curr_token = std::move( token );

    switch( currentInsertionMode() ) {
        case specs::InsertionMode::BEFORE_KEYS   : { processContent<InsertionMode_e::BEFORE_KEYS>( currentToken() );    } break;
        case specs::InsertionMode::BEFORE_KEY    : { processContent<InsertionMode_e::BEFORE_KEY>( currentToken() );     } break;
        case specs::InsertionMode::IN_KEY        : { processContent<InsertionMode_e::IN_KEY>( currentToken() );         } break;
        case specs::InsertionMode::AFTER_KEY     : { processContent<InsertionMode_e::AFTER_KEY>( currentToken() );      } break;
        case specs::InsertionMode::BEFORE_VALUES : { processContent<InsertionMode_e::BEFORE_VALUES>( currentToken() );  } break;
        case specs::InsertionMode::BEFORE_VALUE  : { processContent<InsertionMode_e::BEFORE_VALUE>( currentToken() );   } break;
        case specs::InsertionMode::IN_NAMED_VALUE: { processContent<InsertionMode_e::IN_NAMED_VALUE>( currentToken() ); } break;
        case specs::InsertionMode::AFTER_VALUE   : { processContent<InsertionMode_e::AFTER_VALUE>( currentToken() );    } break;
        case specs::InsertionMode::AFTER_SCOPE   : { processContent<InsertionMode_e::AFTER_SCOPE>( currentToken() );    } break;

        case specs::InsertionMode::END_OF_FILE : {
            throw exception::parsing_failure(
                "[configuration::parser::Parser::dispatch( std::unique_ptr<token::ConfigTk> )] "
                "EOF had already been reached - content cannot be appended to current output in this state."
            );
        } break;

        default: {
            throw exception::parsing_failure(
                "[configuration::parser::Parser::dispatch( std::unique_ptr<token::ConfigTk> )] "
                "Unknown insertion mode: " + blogator::to_string( currentInsertionMode() )
            );
        } break;
    }
}

/**
 * Resets the parser
 * @return Configuration container object
 */
std::unique_ptr<Configuration> parser::Parser::reset() {
    if( currentInsertionMode() != InsertionMode_e::END_OF_FILE ) {
        LOG_WARNING( "[configuration::parser::Parser::reset()] Premature reset (EOF has not been reached)" );
        cleanupEOF();
    }

    _cache = {};
    return std::move( _config );
}

/**
 * Gets the number of errors raised for the current Configuration
 * @return Error count
 */
size_t parser::Parser::errors() const {
    return _errors;
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Configuration error code
 */
void parser::Parser::logError( blogator::TextPos position, int err_code ) {
    ++_errors;
    reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, position );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Configuration error code
 * @param str Description string
 * @param print_insertion_mode Flag to print the current insertion mode in the error description (default: true)
 */
void parser::Parser::logError( TextPos position, int err_code, const std::string & str, bool print_insertion_mode ) {
    ++_errors;
    if( print_insertion_mode ) {
        std::stringstream ss;
        ss << "{ '" << currentInsertionMode() << "' | " << str << " }";
        reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, ss.str(), position );
    } else {
        reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, str, position );
    }
}

/**
 * Sends a parse error to the parser log (format: "{ '$INSERTION_MODE' | $DESC_STRING }" )
 * @param position Position in source text
 * @param err_code Configuration error code
 * @param str Description string (UTF-32) to append to the insertion mode description
 * @param print_insertion_mode Flag to print the current insertion mode in the error description (default: true)
 */
void parser::Parser::logError( TextPos position, int err_code, const std::u32string & str, bool print_insertion_mode ) {
    logError( position, err_code, unicode::utf8::convert( str ), print_insertion_mode );
}

/**
 * Sends a parse error to the parser log
 * @param position Position in source text
 * @param err_code Configuration error code
 * @param token Token
 */
void parser::Parser::logError( TextPos position, int err_code, const token::ConfigTk & token ) {
    ++_errors;
    std::stringstream ss;

    ss << "{ '" << currentInsertionMode()  << "' | '" << token.type();

    if( token.type() != specs::TokenType::END_OF_FILE ) {
        ss << "': \"";
        unicode::utf8::convert( ss, token.text() );
        ss << "\"";
    } else {
        ss << "'";
    }

    ss << " }";

    reporter::ParseReporter::log( _src_path, reporter::Context::CONFIGURATION, err_code, ss.str(), position );
}

/**
 * Processes a token in the 'BEFORE_KEYS' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_KEYS>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE: [[fallthrough]];
        case specs::TokenType::KEY: {
            createScope();
            setInsertionMode( InsertionMode_e::BEFORE_KEY );
            processContent<InsertionMode_e::BEFORE_KEY>( token );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() == U":" ) {
                if( !_cache.scope.empty() ) { //special case as new `_cache.scope` item is not yet created
                    logError( token->position(), ErrorCode::ROOT_IN_SCOPED_KEY );
                } //else: root so ignore
            } else if( token->text() == U"=" ) {
                logError( token->position(), ErrorCode::UNSPECIFIED_KEY );
                setInsertionMode( InsertionMode_e::AFTER_KEY );
                processContent<InsertionMode_e::AFTER_KEY>( token );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"}" ) { //closing scope
                if( currBracket() == U"{" ) {
                    popBracket();
                    popLastScope();
                    setInsertionMode( InsertionMode_e::AFTER_SCOPE );
                } else {
                    logError( token->position(), ErrorCode::UNOPENED_BRACKET, *token );
                }
            } else if( token->text() == U"{" ) {
                logError( token->position(), ErrorCode::SYNTAX_ERROR, *token );
                pushBracket( token->text(), token->position() );
            } else {
                logError( token->position(), ErrorCode::SYNTAX_ERROR, *token );
            }
        } break;

        case specs::TokenType::END_OF_FILE: {
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;

        default: {
            logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'BEFORE_KEY' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_KEY>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE: [[fallthrough]];
        case specs::TokenType::KEY: {
            createScopedKey();
            setInsertionMode( InsertionMode_e::IN_KEY );
            processContent<InsertionMode_e::IN_KEY>( token );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() == U":" ) {
                if( !inRootScope() ) {
                    logError( token->position(), ErrorCode::ROOT_IN_SCOPED_KEY );
                } //else: root so ignore
            } else if( token->text() == U"=" ) {
                logError( token->position(), ErrorCode::UNSPECIFIED_KEY );
                setInsertionMode( InsertionMode_e::AFTER_KEY );
                processContent<InsertionMode_e::AFTER_KEY>( token );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"}" ) { //closing scope
                if( currBracket() == U"{" ) { //closing scope without a value
                    logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
                    addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
                    setInsertionMode( InsertionMode_e::AFTER_VALUE );
                    processContent<InsertionMode_e::AFTER_VALUE>( token );
                } else if( !closeBracketsDownTo( U"{", token->position() ) ) {
                    logError( token->position(), ErrorCode::UNOPENED_BRACKET, *token );
                }
            } else if( token->text() == U"{" ) {
                setInsertionMode( InsertionMode_e::AFTER_KEY );
                processContent<InsertionMode_e::AFTER_KEY>( token );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::END_OF_FILE: {
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;

        default: {
            logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'IN_KEY' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::IN_KEY>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE: {
            if( !Configuration::validate( token->text() ) ) {
                logError( token->position(), ErrorCode::INVALID_NAMESPACE_FORMAT, *token );
            }

            lastScopedKey().emplace_back( token->text() );
        } break;

        case specs::TokenType::KEY:        [[fallthrough]];
        case specs::TokenType::VALUE_NAME: {
            if( !Configuration::validate( token->text() ) ) {
                logError( token->position(), ErrorCode::INVALID_KEY_FORMAT, *token );
            }

            lastScopedKey().emplace_back( token->text() );
            setInsertionMode( InsertionMode_e::AFTER_KEY );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() != U":" ) {
                setInsertionMode( InsertionMode_e::AFTER_KEY );
                processContent<InsertionMode_e::AFTER_KEY>( token );
            } //else: ignore ':'
        } break;

        default: {
            setInsertionMode( InsertionMode_e::AFTER_KEY );
            processContent<InsertionMode_e::AFTER_KEY>( token );
        } break;
    }
}

/**
 * Processes a token in the 'AFTER_KEY' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::AFTER_KEY>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE:
        case specs::TokenType::KEY: {
            setInsertionMode( InsertionMode_e::BEFORE_KEY );
            processContent<InsertionMode_e::BEFORE_KEY>( token );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() == U"=" ) {
                setInsertionMode( InsertionMode_e::BEFORE_VALUES );
            } else if( token->text() == U":" ) {
                if( !inRootScope() ) {
                    logError( token->position(), ErrorCode::ROOT_IN_SCOPED_KEY, *token );
                }
                setInsertionMode( InsertionMode_e::BEFORE_KEY );
            } else { //dropped/ignored
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::PUNCTUATOR: {
            if( token->text() == U"," ) {
                setInsertionMode( InsertionMode_e::BEFORE_KEY );
            } else if( token->text() == U";" ) {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
                addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
                addToConfiguration( _cache.value );
                setInsertionMode( InsertionMode_e::BEFORE_KEY );
            } else { //dropped/ignored
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"{" ) {
                logError( token->position(), ErrorCode::MISSING_ASSIGNMENT_OPERATOR );
                setInsertionMode( InsertionMode_e::BEFORE_VALUES );
                processContent<InsertionMode_e::BEFORE_VALUES>( token );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::CHARACTER: {
            logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
        } break;

        case specs::TokenType::VALUE_NAME:
        case specs::TokenType::STRING_LITERAL:
        case specs::TokenType::INTEGER_LITERAL_10:
        case specs::TokenType::INTEGER_LITERAL_16:
        case specs::TokenType::INTEGER_LITERAL_FLT: [[fallthrough]];
        case specs::TokenType::BOOLEAN_LITERAL: {
            logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            setInsertionMode( InsertionMode_e::BEFORE_VALUES );
            processContent<InsertionMode_e::BEFORE_VALUES>( token );
        } break;

        case specs::TokenType::END_OF_FILE: {
            logError( token->position(), ErrorCode::EOF_IN_KEY, *token );
            addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
            addToConfiguration( _cache.value );
            setInsertionMode( InsertionMode_e::END_OF_FILE );
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;
    }
}

/**
 * Processes a token in the 'BEFORE_VALUES' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_VALUES>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::OPERATOR: {
            if( token->text() == U"[" ) { //array
                addValueToCache( Value::Type_e::ARRAY, token->text(), token->position() );
                pushBracket( token->text(), token->position() );
                setInsertionMode( InsertionMode_e::BEFORE_VALUE );

            } else if( token->text() == U":" ) {
                resetPendingNamedValueCache( token->position() );
                setInsertionMode( InsertionMode_e::IN_NAMED_VALUE );
                processContent < InsertionMode_e::IN_NAMED_VALUE > ( token );

            } else {
                logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            }
        } break;

        case specs::TokenType::PUNCTUATOR: {
            if( token->text() != U";" ) {
                logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            }

            addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
            processContent<InsertionMode_e::AFTER_VALUE>( token );
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"{" ) { //scope
                pushBracket( token->text(), token->position() );
                setInsertionMode( InsertionMode_e::BEFORE_KEYS );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::NAMESPACE:
        case specs::TokenType::VALUE_NAME:
        case specs::TokenType::STRING_LITERAL:
        case specs::TokenType::INTEGER_LITERAL_10:
        case specs::TokenType::INTEGER_LITERAL_16:
        case specs::TokenType::INTEGER_LITERAL_FLT:
        case specs::TokenType::BOOLEAN_LITERAL: {
            setInsertionMode( InsertionMode_e::BEFORE_VALUE );
            processContent<InsertionMode_e::BEFORE_VALUE>( token );
        } break;

        case specs::TokenType::END_OF_FILE: {
            logError( token->position(), ErrorCode::EOF_IN_VALUE );
            addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
            addToConfiguration( _cache.value );
            setInsertionMode( InsertionMode_e::END_OF_FILE );
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;

        default: {
            logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'BEFORE_VALUE' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::BEFORE_VALUE>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE: {
            resetPendingNamedValueCache( token->position() );
            setInsertionMode( InsertionMode_e::IN_NAMED_VALUE );
            processContent<InsertionMode_e::IN_NAMED_VALUE>( token );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() == U"[" ) { //array
                addValueToCache( Value::Type_e::ARRAY, token->text(), token->position() );
                pushBracket( token->text(), token->position() );

            } else if( token->text() == U"]" ) {
                if( currBracket() == U"[" ) { //empty array
                    setInsertionMode( InsertionMode_e::AFTER_VALUE );
                    processContent<InsertionMode_e::AFTER_VALUE>( token );
                } else {
                    if( closeBracketsDownTo( U"]", token->position() ) ) {
                        _cache.value.closeArray();
                    } else {
                        logError( token->position(), ErrorCode::UNOPENED_BRACKET, *token );
                    }
                }

            } else if( token->text() == U":" ) {
                resetPendingNamedValueCache( token->position() );
                setInsertionMode( InsertionMode_e::IN_NAMED_VALUE );
                processContent < InsertionMode_e::IN_NAMED_VALUE > ( token );

            } else {
                logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            }
        } break;

        case specs::TokenType::PUNCTUATOR: {
            logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
            processContent<InsertionMode_e::AFTER_VALUE>( token );
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"{" ) { //scope
                pushBracket( token->text(), token->position() );
                setInsertionMode( InsertionMode_e::BEFORE_KEYS );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::VALUE_NAME: {
            resetPendingNamedValueCache( token->position() );
            setInsertionMode( InsertionMode_e::IN_NAMED_VALUE );
            processContent<InsertionMode_e::IN_NAMED_VALUE>( token );
        } break;

        case specs::TokenType::STRING_LITERAL: {
            addValueToCache( Value::Type_e::STRING, token->text(), token->position() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
        } break;

        case specs::TokenType::INTEGER_LITERAL_10: [[fallthrough]];
        case specs::TokenType::INTEGER_LITERAL_16: {
            addValueToCache( Value::Type_e::INTEGER, token->text(), token->position() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
        } break;

        case specs::TokenType::INTEGER_LITERAL_FLT: {
            addValueToCache( Value::Type_e::FLOAT, token->text(), token->position() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
        } break;

        case specs::TokenType::BOOLEAN_LITERAL: {
            addValueToCache( Value::Type_e::BOOLEAN, token->text(), token->position() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
        } break;

        case specs::TokenType::END_OF_FILE: {
            logError( token->position(), ErrorCode::EOF_IN_VALUE );
            addToConfiguration( _cache.value );
            setInsertionMode( InsertionMode_e::END_OF_FILE );
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;

        default: { //i.e.: 'KEY' & 'CHARACTER'
            logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'IN_VALUE' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::IN_NAMED_VALUE>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE: {
            if( !Configuration::validate( token->text() ) ) {
                logError( token->position(), ErrorCode::INVALID_NAMESPACE_FORMAT, U"\"" + token->text() + U"\"" );
            }

            appendToPendingNamedValue( token->text() );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() != U":" ) {
                logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            } //else: drop ':'
        } break;

        case specs::TokenType::PUNCTUATOR: {
            if( token->text() == U";" ) {
                addValueToCache( Value::Type_e::VOID, token->text(), token->position() );
                setInsertionMode( InsertionMode_e::AFTER_VALUE );
                processContent<InsertionMode_e::AFTER_VALUE>( token );
            } else {
                logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            }
        } break;

        case specs::TokenType::KEY:        [[fallthrough]];
        case specs::TokenType::VALUE_NAME: {
            if( !Configuration::validate( token->text() ) ) {
                logError( token->position(), ErrorCode::INVALID_KEY_FORMAT, U"\"" + token->text() + U"\"" );
            }

            appendToPendingNamedValue( token->text() );
            addValueToCache( Value::Type_e::NAME, Configuration::convert( pendingNamedValue() ), pendingNamedValuePosition() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
        } break;

        case specs::TokenType::END_OF_FILE: {
            logError( token->position(), ErrorCode::EOF_IN_VALUE );
            addValueToCache( Value::Type_e::NAME, Configuration::convert( pendingNamedValue() ), pendingNamedValuePosition() );
            setInsertionMode( InsertionMode_e::AFTER_VALUE );
            processContent<InsertionMode_e::AFTER_VALUE>( token );
        } break;

        default: {
            logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'AFTER_VALUE' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::AFTER_VALUE>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE: [[fallthrough]];
        case specs::TokenType::KEY: {
            logError( token->position(), ErrorCode::MISSING_TERMINATOR );
            addToConfiguration( _cache.value );
            popLastScope();
            setInsertionMode( InsertionMode_e::BEFORE_KEYS );
            processContent<InsertionMode_e::BEFORE_KEYS>( token );
        } break;

        case specs::TokenType::OPERATOR: {
            if( token->text() == U"]" ) {
                if( currBracket() == U"[" ) { //close array
                    _cache.value.closeArray();
                    popBracket();
                } else if( !closeBracketsDownTo( U"]", token->position() ) ) {
                    logError( token->position(), ErrorCode::UNOPENED_BRACKET, *token );
                    _cache.value.convertCurrentContextToArray();
                }

            } else {
                logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
            }
        } break;

        case specs::TokenType::PUNCTUATOR: {
            if( token->text() == U";" ) {
                addToConfiguration( _cache.value );
                popLastScope();
                setInsertionMode( InsertionMode_e::BEFORE_KEYS );

                while( currBracket() == U"[" ) {
                    logError( token->position(),
                              ErrorCode::UNCLOSED_BRACKET,
                              ( "'" + unicode::utf8::convert( currBracket() ) + "' at " + currBracketPosition().str() ) );
                    popBracket();
                }

            } else if( token->text() == U"," ) { //i.e. inside array
                if( currBracket() != U"[" ) {
                    logError( token->position(), ErrorCode::MISSING_LIST_DECLARATION, *token );
                    _cache.value.convertCurrentContextToArray();
                }

                setInsertionMode( InsertionMode_e::BEFORE_VALUE );

            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"}" ) {
                if( currBracket() == U"{" ) { //close scope
                    logError( token->position(), ErrorCode::MISSING_TERMINATOR );
                    addToConfiguration( _cache.value );
                    popBracket();
                    popLastScope();
                    setInsertionMode( InsertionMode_e::AFTER_SCOPE );
                } else if( !closeBracketsDownTo( U"{", token->position() ) ) {
                    logError( token->position(), ErrorCode::UNOPENED_BRACKET, *token );
                }

            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::VALUE_NAME:
        case specs::TokenType::STRING_LITERAL:
        case specs::TokenType::INTEGER_LITERAL_10:
        case specs::TokenType::INTEGER_LITERAL_16:
        case specs::TokenType::INTEGER_LITERAL_FLT: [[fallthrough]];
        case specs::TokenType::BOOLEAN_LITERAL: {
            if( currBracket() == U"[" ) {
                logError( token->position(), ErrorCode::MISSING_LIST_SEPARATOR, *token );
            } else {
                logError( token->position(), ErrorCode::MISSING_LIST_DECLARATION, *token );
                _cache.value.convertCurrentContextToArray();
            }

            setInsertionMode( InsertionMode_e::BEFORE_VALUE );
            processContent<InsertionMode_e::BEFORE_VALUE>( token );
        } break;

        case specs::TokenType::END_OF_FILE: {
            addToConfiguration( _cache.value );
            popLastScope();
            setInsertionMode( InsertionMode_e::END_OF_FILE );
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;

        default: {
            logError( token->position(), ErrorCode::INVALID_VALUE_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'AFTER_SCOPE' insertion mode (i.e.: right after a scope closing '}');
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::AFTER_SCOPE>( const std::unique_ptr<token::ConfigTk> & token ) {
    switch( token->type() ) {
        case specs::TokenType::COMMENT: break; //dropped/ignored

        case specs::TokenType::NAMESPACE:
        case specs::TokenType::KEY: {
            if( match( previousToken(), U"}" ) ) {
                logError( token->position(), ErrorCode::MISSING_TERMINATOR ); //TODO if last tk != '}' ?
            }
            setInsertionMode( InsertionMode_e::BEFORE_KEYS );
            processContent<InsertionMode_e::BEFORE_KEYS>( token );
        } break;

        case specs::TokenType::PUNCTUATOR: {
            if( token->text() == U";" ) {
                setInsertionMode( InsertionMode_e::BEFORE_KEYS );
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::BRACKET: {
            if( token->text() == U"}" ) {
                if( currBracket() == U"{" ) {
                    logError( token->position(), ErrorCode::MISSING_TERMINATOR );
                    popBracket();
                    popLastScope();
                } else if( !closeBracketsDownTo( U"{", token->position() ) ) {
                    logError( token->position(), ErrorCode::UNOPENED_BRACKET, *token );
                }
            } else {
                logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
            }
        } break;

        case specs::TokenType::END_OF_FILE: {
            logError( token->position(), ErrorCode::MISSING_TERMINATOR );
            setInsertionMode( InsertionMode_e::END_OF_FILE );
            processContent<InsertionMode_e::END_OF_FILE>( token );
        } break;

        default: {
            logError( token->position(), ErrorCode::INVALID_CONFIG_FORMAT, *token );
        } break;
    }
}

/**
 * Processes a token in the 'END_OF_FILE' insertion mode
 * @param token ConfigTk token
 */
template<> void parser::Parser::processContent<parser::specs::InsertionMode::END_OF_FILE>( [[maybe_unused]] const std::unique_ptr<token::ConfigTk> & token ) {
    setInsertionMode( InsertionMode_e::END_OF_FILE );
    cleanupEOF();
}

/**
 * Gets current insertion mode
 * @return Insertion mode
 */
inline parser::Parser::InsertionMode_e parser::Parser::currentInsertionMode() const {
    return _insertion_mode;
}

/**
 * Sets the current insertion mode
 * @param mode Insertion mode
 */
inline void parser::Parser::setInsertionMode( parser::Parser::InsertionMode_e mode ) {
    _insertion_mode = mode;
}

/**
 * Gets the current ConfigTk
 * @return Pointer to current ConfigTk being processed
 */
inline const std::unique_ptr<parser::token::ConfigTk> & parser::Parser::currentToken() const {
    return _curr_token;
}

/**
 * Gets the previous ConfigTk
 * @return Pointer to the previous ConfigTk processed
 */
inline const std::unique_ptr<parser::token::ConfigTk> & parser::Parser::previousToken() const {
    return _prev_token;
}

/**
 * Checks the token type of a token matches a given type
 * @param tk Token
 * @param tk_type TokenType
 * @return Match state
 */
inline bool parser::Parser::match( const std::unique_ptr<token::ConfigTk> &tk, parser::specs::TokenType tk_type ) {
    return ( tk && tk->type() == tk_type );
}

/**
 * Checks the text of a token matches a given string
 * @param tk Token
 * @param txt String
 * @return Match state
 */
inline bool parser::Parser::match( const std::unique_ptr<token::ConfigTk> &tk, const std::u32string &txt ) {
    return ( tk && tk->text() == txt );
}

/**
 * Pushes a new scope
 * @return Newly created scope
 */
inline parser::Parser::Scope_t & parser::Parser::createScope() {
    return _cache.scope.emplace_back();
}

/**
 * Gets the current scope
 * @return Current scope (auto-created if none available)
 */
inline parser::Parser::Scope_t & parser::Parser::lastScope() {
    return ( _cache.scope.empty() ? _cache.scope.emplace_back() : _cache.scope.back() );
}

/**
 * Pops the last scope added
 */
inline void parser::Parser::popLastScope() {
    if( !_cache.scope.empty() ) {
        _cache.scope.pop_back();
    }
}

/**
 * Checks if currently in a scope
 * @return Scope existence state
 */
inline bool parser::Parser::hasScope() const {
    return !_cache.scope.empty();
}

/**
 * Checks if the state of the parser inside the root scope
 * @return Inside root scope state
 */
inline bool parser::Parser::inRootScope() const {
    if( !_cache.scope.empty() ) {
        for( auto it = _cache.scope.crbegin(); it != _cache.scope.crend(); ++it ) {
            if( !it->empty() ) {
                return ( std::next( it ) == _cache.scope.crend() ); //i.e.: is it the first list of keys in the current scopes collection
            }
        }
    }

    return true; //i.e.: there are no scopes, or they are all empty
}

/**
 * Creates a Namespaced Key entry in the last scope (scope auto-created if there are none)
 * @return Namespaced key entry (auto-created if none available)
 */
inline parser::Parser::Scope_t::value_type & parser::Parser::createScopedKey() {
    return ( hasScope() ? lastScope().emplace_back()
                        : createScope().emplace_back() );
}

/**
 * Gets the last created namespaced key entry in the last scope
 * @return Namespaced key entry (auto-created if none available)
 */
inline parser::Parser::Scope_t::value_type & parser::Parser::lastScopedKey() {
    if( !hasScope() ) {
        createScope();
    }

    return ( lastScope().empty() ? lastScope().emplace_back()
                                 : lastScope().back() );
}

/**
 * Adds value to cached value(s)
 * @param value_info Value information package
 */
inline void parser::Parser::addValueToCache( Value::Type_e value_type, const std::u32string & value_str, const TextPos & position ) {
    switch( value_type ) {
        case Value::Type_e::VOID: {
            if( _cache.value.initialised() ) {
                _cache.value.addValueToCurrentContext( Value(), position );
            } else {
                _cache.value = ValueCache( Value(), position );
            }
        } break;

        case Value::Type_e::NAME: {
            const auto * val = _config->find( pendingNamedValue() );

            if( val != nullptr && val->type() != Value::Type_e::NAME ) {
                if( _cache.value.initialised() ) {
                    _cache.value.addValueToCurrentContext( val->clone(), position );

                    if( val->type() == Value::Type_e::ARRAY ) {
                        _cache.value.closeArray();
                    }

                } else {
                    _cache.value = ValueCache( val->clone(), position );
                }

            } else {
                if( _cache.value.initialised() ) {
                    _cache.value.addValueToCurrentContext(
                        Value( Configuration::convert( pendingNamedValue() ), Value::IS_NAMED_VALUE ),
                        position
                    );

                } else {
                    _cache.value = ValueCache(
                        Value( Configuration::convert( pendingNamedValue() ), Value::IS_NAMED_VALUE ),
                        position
                    );
                }
            }
        } break;

        case Value::Type_e::BOOLEAN: {
            if( _cache.value.initialised() ) {
                _cache.value.addValueToCurrentContext(
                    Value( ( value_str == U"true" ) ),
                    position
                );

            } else {
                _cache.value = ValueCache(
                    Value( ( value_str == U"true" ) ),
                    position
                );
            }
        } break;

        case Value::Type_e::INTEGER: {
            try {
                if( _cache.value.initialised() ) {
                    _cache.value.addValueToCurrentContext(
                        Value( unicode::utf32::toLong( value_str, 0 ) ),
                        position
                    );

                } else {
                    _cache.value = ValueCache(
                        Value( unicode::utf32::toLong( value_str, 0 ) ),
                        position
                    );
                }

            } catch( const std::invalid_argument & e ) {
                throw exception::parsing_failure(
                    "[configuration::parser::Parser::addValueToCache( " + blogator::to_string( value_type ) + ", " + unicode::utf8::convert( value_str ) + ", " + position.str() + " )] "
                    "Failed to convert string to integer: "
                );
            }
        } break;

        case Value::Type_e::FLOAT: {
            try {
                if( _cache.value.initialised() ) {
                    _cache.value.addValueToCurrentContext(
                        Value( unicode::utf32::toDouble( value_str ) ),
                        position
                    );

                } else {
                    _cache.value = ValueCache(
                        Value( unicode::utf32::toDouble( value_str ) ),
                        position
                    );
                }

            } catch( const std::invalid_argument & e ) {
                throw exception::parsing_failure(
                    "[configuration::parser::Parser::addValueToCache( " + blogator::to_string( value_type ) + ", " + unicode::utf8::convert( value_str ) + ", " + position.str() + " )] "
                    "Failed to convert string to double: "
                );
            }
        } break;

        case Value::Type_e::STRING: {
            if( _cache.value.initialised() ) {
                _cache.value.addValueToCurrentContext(
                    Value( value_str ),
                    position
                );
            } else {
                _cache.value = ValueCache(
                    Value( value_str ),
                    position
                );
            }
        } break;

        case Value::Type_e::ARRAY: {
            if( _cache.value.initialised() ) {
                if( !_cache.value.createArray() ) {
                    throw exception::parsing_failure( //should never happen
                        "[configuration::parser::Parser::addValueToCache( " + blogator::to_string( value_type ) + ", " + unicode::utf8::convert( value_str ) + ", " + position.str() + " )] "
                        "Attempt to create an array when the cached root value has not been converted to an array."
                    );
                }

            } else {
                _cache.value = ValueCache(
                    Value( Value::IS_ARRAY_VALUE ),
                    position
                );
            }

        } break;
    }
}

/**
 * Add Key-Value to the configuration
 * @param cached_value Cached value to add
 * @throws blogator::exception::parsing_failure when type conversion of value fails or an unexpected state is encountered
 */
inline void parser::Parser::addToConfiguration( ValueCache & cached_value ) {
    auto nsk = Configuration::Key_t();
    addToConfiguration_( _cache.scope.cbegin(), nsk, cached_value );
    _cache.value = ValueCache();
}

/**
 * Add Key-Value to the configuration
 * @param it Iterator to the current scope
 * @param root_ns Root namespace before the current scope
 * @param cached_value Cached value to add
 * @throws blogator::exception::parsing_failure when type conversion of value fails or an unexpected state is encountered
 */
void parser::Parser::addToConfiguration_( std::vector<Scope_t>::const_iterator it, const Configuration::Key_t & root_ns, ValueCache & cached_value ) {
    if( it == _cache.scope.cend() ) {
        if( cached_value.rootType() != Value::Type_e::ARRAY ) {
            const auto * cfg_val = _config->find( root_ns );

            if( cfg_val != nullptr ) {
                std::stringstream ss;
                ss << *cfg_val << " <- " << *cached_value.root;
                logError( cached_value.position, ErrorCode::VALUE_OVERRIDE, ss.str(), false );
            }
        }

        auto & value = _config->add( root_ns, cached_value.root->clone() );

        if( resolveNamedValue( root_ns, cached_value.position, value ) == 0 ) {
            resolveCachedNamedValue( Configuration::convert( root_ns ), &value );
        }

    } else {
        for( const auto & scoped_nsk : *it ) {
            auto appended_root_ns = root_ns;
            appended_root_ns.insert( appended_root_ns.end(), scoped_nsk.cbegin(), scoped_nsk.cend() );
            addToConfiguration_( std::next( it ), appended_root_ns, cached_value );
        }
    }
}

/**
 * Resolves a Value
 * @param nsk Namespaced key for the value (used for caching on unresolved)
 * @param position Start position of the root for the Value
 * @param value Value to resolve
 * @return Number of unresolved named values
 */
size_t parser::Parser::resolveNamedValue( const Configuration::Key_t & nsk, const TextPos & position, Value & value ) {
    size_t unresolved = 0;

    if( value.type() == Value::Type_e::ARRAY ) {
        auto & arr = value.getArray();

        for( auto & v : arr ) {
            unresolved += resolveNamedValue( nsk, position, v );
        }

    } else if( value.type() == Value::Type_e::NAME ) {
        const auto * resolved_val = _config->find( Configuration::convert( value.getString() ) );

        if( resolved_val != nullptr && resolved_val->type() != Value::Type_e::NAME ) {
            value = *resolved_val; //copy!

        } else {
            unresolved += 1;
            cacheUnresolvedNamedValue( value.getString(), { position, value } );
        }
    }

    return unresolved;
}

/**
 * Gets the current cached pending named value (ns+key)
 * @return Cached named value (NS key)
 */
inline const Configuration::Key_t & parser::Parser::pendingNamedValue() const {
    return _cache.pending_named_value;
}

/**
 * Gets the current cached pending named value position
 * @return Start position of the namespaced key in the source
 */
inline const blogator::TextPos &parser::Parser::pendingNamedValuePosition() const {
    return _cache.pending_named_value_position;
}

/**
 * Appends a part to the current cached pending named value
 * @param str Key section string to append to the Key_t structure
 */
inline void parser::Parser::appendToPendingNamedValue( const std::u32string &str ) {
    _cache.pending_named_value.emplace_back( str );
}

/**
 * Clears the cached pending value
 * @param pos Start position of the soon to be added namespaced key in the source
 */
inline void parser::Parser::resetPendingNamedValueCache( const TextPos & pos ) {
    _cache.pending_named_value.clear();
    _cache.pending_named_value_position = pos;
}

/**
 * Adds a { key, named value } entry to the unresolved named value cache
 * @param nsk Named value
 * @param value_ref Value container for the unresolved entry { text position, ref to pending value object in Configuration }
 */
inline void parser::Parser::cacheUnresolvedNamedValue( const std::u32string & nsk, NamedValues::ValueRef value_ref ) {
    auto [it, inserted] = _cache.unresolved_named_values.try_emplace( nsk, NamedValues { Configuration::convert( nsk ), std::vector<NamedValues::ValueRef>() } );
    it->second.values.emplace_back( value_ref );
}

/**
 * Resolves a known named value with its value
 * @param nsk Namespaced key string of the named value
 * @param value Pointer to the Value to set pending values to
 * @return Resolve success
 */
inline bool parser::Parser::resolveCachedNamedValue( const std::u32string &nsk, const Value * value ) {
    auto it = _cache.unresolved_named_values.find( nsk );

    if( value != nullptr && value->type() != Value::Type_e::NAME && it != _cache.unresolved_named_values.end() ) {
        for( auto & val : it->second.values ) {
            val.ref.get() = *value;
        }

        _cache.unresolved_named_values.erase( it );
        return true;
    }

    return false;
}

/**
 * Pushes a bracket to the stack of opened brackets
 * @param c Bracket character (left/opening - unchecked)
 * @param position Position in source text
 */
inline void parser::Parser::pushBracket( std::u32string c, TextPos position ) {
    _cache.opened_brackets.push_back( { position, c } );
}

/**
 * Gets the current bracket at the top of the opened bracket stack
 * @return Last opened bracket or NUL (0x00) if stack is empty
 */
inline std::u32string parser::Parser::currBracket() const {
    if( !_cache.opened_brackets.empty() ) {
        return _cache.opened_brackets.back().character;
    }

    return {};
}

/**
 * Gets the position of the current bracket at the top of the opened bracket stack
 * @return Position of last opened bracket
 * @throws std::out_of_range when stack is empty
 */
inline const blogator::TextPos & parser::Parser::currBracketPosition() const {
    return _cache.opened_brackets.back().pos;
}

/**
 * Pops the last bracket from the stack of opened brackets
 */
inline void parser::Parser::popBracket() {
    if( !_cache.opened_brackets.empty() ) {
        _cache.opened_brackets.pop_back();
    }
}

/**
 * Closes brackets down to and including the first match found (otherwise does nothing)
 * @param bracket_char Bracket character to match ('{' will "popLastScope()" and '[' will "closeArray()" on cached value)
 * @param position Current position
 * @return Target bracket found
 */
inline bool parser::Parser::closeBracketsDownTo( const std::u32string &bracket_char, const TextPos & position ) {
    auto target_it = std::find_if( _cache.opened_brackets.rbegin(),
                                   _cache.opened_brackets.rend(),
                                   []( const auto & bracket) { return bracket.character == U"["; } );

    if( target_it != _cache.opened_brackets.rend() ) {
        auto it = target_it.base();

        while( it != _cache.opened_brackets.end() ) {
            if( bracket_char != it->character ) {
                logError( position,
                          ErrorCode::UNCLOSED_BRACKET,
                          ( "'" + unicode::utf8::convert( it->character ) + "' at " + position.str() ),
                          false );
            }

            if( it->character == U"{" ) {
                popLastScope();
            } else if( it->character == U"[" ) {
                _cache.value.closeArray();
            }

            it = _cache.opened_brackets.erase( it );
        }

        return true;
    }

    return false;
}

/**
 * Wraps up everything and checks pending scopes, unresolved named values, etc...
 */
void parser::Parser::cleanupEOF() {
    struct Err {
        TextPos        pos;
        int            code = ErrorCode::NONE;
        std::u32string desc;
    };

    auto errors = std::vector<Err>();

    for( auto & unv : _cache.unresolved_named_values ) {
        const auto * resolved = _config->find( unv.second.nsk );

        if( resolved != nullptr && resolved->type() != Value::Type_e::NAME ) {
            for( const auto & entry : unv.second.values ) {
                entry.ref.get() = *resolved;
            }

        } else {
            for( const auto & entry : unv.second.values ) {
                errors.emplace_back( Err { entry.pos, ErrorCode::UNRESOLVED_NAMED_VALUE, unv.first } );
            }
        }
    }

    if( !_cache.opened_brackets.empty() ) {
        for( const auto & bracket : _cache.opened_brackets ) {
            errors.emplace_back( Err { bracket.pos, ErrorCode::UNCLOSED_BRACKET, ( U"'" + bracket.character + U"'" ) } );
        }
    }

    std::sort( errors.begin(),
               errors.end(),
               []( Err & lhs, Err & rhs ) { return ( lhs.pos == rhs.pos ? lhs.desc < rhs.desc : lhs.pos < rhs.pos ); } );

    for( const auto & err : errors ) {
        logError( err.pos, err.code, err.desc );
    }
}