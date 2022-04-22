#include "DocType.h"

#include "../../../sys/terminate.h"
#include "../../../unicode/ascii.h"

using namespace blogator::parser::specs::infra;

const std::set<std::u32string> DocType::_full_quirks_complete_pid        = DocType::loadFullPIDMatchesFullQuirks();
const std::set<std::u32string> DocType::_full_quirks_complete_sid        = DocType::loadFullSIDMatchesFullQuirks();
const blogator::Trie<char32_t> DocType::_full_quirks_partial_pid         = DocType::loadPartialPIDMatchesFullQuirks();
const blogator::Trie<char32_t> DocType::_full_quirks_partial_pid_no_sid  = DocType::loadPartialPIDNoSIDMatchesFullQuirks();
const blogator::Trie<char32_t> DocType::_ltd_quirks_partial_pid          = DocType::loadPartialPIDMatchesLtdQuirks();
const blogator::Trie<char32_t> DocType::_ltd_quirks_partial_pid_with_sid = DocType::loadPartialPIDWithSIDMatchesLtdQuirks();

/**
 * Validates Public and System ID pairs
 * @param pid_str Public ID string
 * @param sid_exists Existence flag for the SID
 * @param sid_str System ID string
 * @return Validation state
 */
blogator::parser::dom::QuirksMode DocType::validate( std::u32string pid_str, bool sid_exists, std::u32string sid_str ) {
    blogator::unicode::ascii::tolower( pid_str );

    if( sid_exists ) {
        blogator::unicode::ascii::tolower( sid_str );
    }

    //Full quirks
    if( DocType::_full_quirks_complete_pid.contains( pid_str ) ||
        DocType::_full_quirks_partial_pid.has( pid_str.begin(), pid_str.end() ) ||
        DocType::_full_quirks_complete_sid.contains( sid_str ) ||
        ( !sid_exists && DocType::_full_quirks_partial_pid_no_sid.has( pid_str.cbegin(), pid_str.cend() ) ) )
    {
        return dom::QuirksMode::QUIRKS;
    }

    //Limited quirks
    if( DocType::_ltd_quirks_partial_pid.has( pid_str.cbegin(), pid_str.cend() ) ||
        ( sid_exists && DocType::_ltd_quirks_partial_pid_with_sid.has( pid_str.cbegin(), pid_str.cend() ) ) )
    {
        return dom::QuirksMode::LIMITED_QUIRKS;
    }

    return dom::QuirksMode::NO_QUIRKS;
}

/**
 * Loads complete 'full quirks' Public ID strings
 * @return Set of complete PIDs
 */
std::set<std::u32string> DocType::loadFullPIDMatchesFullQuirks() noexcept {
    try {
        return std::set<std::u32string>( {
            U"-//w3o//dtd w3 html strict 3.0//en//",
            U"-/w3c/dtd html 4.0 transitional/en",
            U"html"
        } );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::DocType::loadFullPIDMatchesFullQuirks()] Failed to initialise." );
    }
}

/**
 * Loads incomplete 'full quirks' Public ID strings
 * @return Trie filled with partial PIDs
 */
blogator::Trie<char32_t> DocType::loadPartialPIDMatchesFullQuirks() noexcept {
    try {
        const auto pids = std::vector<std::u32string>( {
            U"+//silmaril//dtd html pro v0r11 19970101//",
            U"-//as//dtd html 3.0 aswedit + extensions//",
            U"-//advasoft ltd//dtd html 3.0 aswedit + extensions//",
            U"-//ietf//dtd html 2.0 level 1//",
            U"-//ietf//dtd html 2.0 level 2//",
            U"-//ietf//dtd html 2.0 strict level 1//",
            U"-//ietf//dtd html 2.0 strict level 2//",
            U"-//ietf//dtd html 2.0 strict//",
            U"-//ietf//dtd html 2.0//",
            U"-//ietf//dtd html 2.1e//",
            U"-//ietf//dtd html 3.0//",
            U"-//ietf//dtd html 3.2 final//",
            U"-//ietf//dtd html 3.2//",
            U"-//ietf//dtd html 3//",
            U"-//ietf//dtd html level 0//",
            U"-//ietf//dtd html level 1//",
            U"-//ietf//dtd html level 2//",
            U"-//ietf//dtd html level 3//",
            U"-//ietf//dtd html strict level 0//",
            U"-//ietf//dtd html strict level 1//",
            U"-//ietf//dtd html strict level 2//",
            U"-//ietf//dtd html strict level 3//",
            U"-//ietf//dtd html strict//",
            U"-//ietf//dtd html//",
            U"-//metrius//dtd metrius presentational//",
            U"-//microsoft//dtd internet explorer 2.0 html strict//",
            U"-//microsoft//dtd internet explorer 2.0 html//",
            U"-//microsoft//dtd internet explorer 2.0 tables//",
            U"-//microsoft//dtd internet explorer 3.0 html strict//",
            U"-//microsoft//dtd internet explorer 3.0 html//",
            U"-//microsoft//dtd internet explorer 3.0 tables//",
            U"-//netscape comm. corp.//dtd html//",
            U"-//netscape comm. corp.//dtd strict html//",
            U"-//o'reilly and associates//dtd html 2.0//",
            U"-//o'reilly and associates//dtd html extended 1.0//",
            U"-//o'reilly and associates//dtd html extended relaxed 1.0//",
            U"-//sq//dtd html 2.0 hotmetal + extensions//",
            U"-//softquad software//dtd hotmetal pro 6.0::19990601::extensions to html 4.0//",
            U"-//softquad//dtd hotmetal pro 4.0::19971010::extensions to html 4.0//",
            U"-//spyglass//dtd html 2.0 extended//",
            U"-//sun microsystems corp.//dtd hotjava html//",
            U"-//sun microsystems corp.//dtd hotjava strict html//",
            U"-//w3c//dtd html 3 1995-03-24//",
            U"-//w3c//dtd html 3.2 draft//",
            U"-//w3c//dtd html 3.2 final//",
            U"-//w3c//dtd html 3.2//",
            U"-//w3c//dtd html 3.2s draft//",
            U"-//w3c//dtd html 4.0 frameset//",
            U"-//w3c//dtd html 4.0 transitional//",
            U"-//w3c//dtd html experimental 19960712//",
            U"-//w3c//dtd html experimental 970421//",
            U"-//w3c//dtd w3 html//",
            U"-//w3o//dtd w3 html 3.0//",
            U"-//webtechs//dtd mozilla html 2.0//",
            U"-//webtechs//dtd mozilla html//",
        } );

        auto trie = blogator::Trie<char32_t>();

        for( const auto & pid : pids ) {
            trie.add( pid.cbegin(), pid.cend() );
        }

        return std::move( trie );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::DocType::loadPartialPIDMatchesFullQuirks()] Failed to initialise." );
    }
}

/**
 * Loads complete 'full quirks' System ID strings
 * @return Set of complete SIDs
 */
std::set<std::u32string> DocType::loadFullSIDMatchesFullQuirks() noexcept {
    try {
        return std::set<std::u32string>( {
            U"http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd"
        } );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::DocType::loadFullSIDMatchesFullQuirks()] Failed to initialise." );
    }
}

/**
 * Loads incomplete 'full quirks' Public IDs for case where System IDs are absent
 * @return Trie filled with partial PIDs
 */
blogator::Trie<char32_t> DocType::loadPartialPIDNoSIDMatchesFullQuirks() noexcept {
    try {
        const auto pids = std::vector<std::u32string>( {
            U"-//w3c//dtd html 4.01 frameset//",
            U"-//w3c//dtd html 4.01 transitional//",
        } );

        auto trie = blogator::Trie<char32_t>();

        for( const auto & pid : pids ) {
            trie.add( pid.cbegin(), pid.cend() );
        }

        return std::move( trie );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::DocType::loadPartialPIDNoSIDMatchesFullQuirks()] Failed to initialise." );
    }
}

/**
 * Loads incomplete 'limited quirks' Public IDs for case where System IDs are present
 * @return Trie filled with partial PIDs
 */
blogator::Trie<char32_t> DocType::loadPartialPIDMatchesLtdQuirks() noexcept {
        try {
        const auto pids = std::vector<std::u32string>( {
            U"-//w3c//dtd xhtml 1.0 frameset//",
            U"-//w3c//dtd xhtml 1.0 transitional//",
        } );

        auto trie = blogator::Trie<char32_t>();

        for( const auto & pid : pids ) {
            trie.add( pid.cbegin(), pid.cend() );
        }

        return std::move( trie );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::DocType::loadPartialPIDMatchesLtdQuirks()] Failed to initialise." );
    }
}

/**
 * Loads incomplete 'limited quirks' Public ID strings
 * @return  Trie filled with partial PIDs
 */
blogator::Trie<char32_t> DocType::loadPartialPIDWithSIDMatchesLtdQuirks() noexcept {
    try {
        const auto pids = std::vector<std::u32string>( {
            U"-//w3c//dtd html 4.01 frameset//",
            U"-//w3c//dtd html 4.01 transitional//",
        } );

        auto trie = blogator::Trie<char32_t>();

        for( const auto & pid : pids ) {
            trie.add( pid.cbegin(), pid.cend() );
        }

        return std::move( trie );

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::DocType::loadPartialPIDWithSIDMatchesLtdQuirks()] Failed to initialise." );
    }
}
