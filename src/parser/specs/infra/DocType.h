#ifndef BLOGATOR_PARSER_SPECS_INFRA_DOCTYPE_H
#define BLOGATOR_PARSER_SPECS_INFRA_DOCTYPE_H

#include <set>
#include <string>

#include "../../dom/enum/QuirksMode.h"
#include "../../../datastructure/Trie.h"

namespace blogator::parser::specs::infra {
    class DocType {
      public:
        static dom::QuirksMode validate( std::u32string pid_str, bool sid_exists, std::u32string sid_str );

      private:
        static const std::set<std::u32string> _full_quirks_complete_pid;
        static const Trie<char32_t>           _full_quirks_partial_pid;
        static const std::set<std::u32string> _full_quirks_complete_sid;
        static const Trie<char32_t>           _full_quirks_partial_pid_no_sid;
        static const Trie<char32_t>           _ltd_quirks_partial_pid;
        static const Trie<char32_t>           _ltd_quirks_partial_pid_with_sid;

        static std::set<std::u32string> loadFullPIDMatchesFullQuirks() noexcept;
        static Trie<char32_t> loadPartialPIDMatchesFullQuirks() noexcept;
        static std::set<std::u32string> loadFullSIDMatchesFullQuirks() noexcept;
        static Trie<char32_t> loadPartialPIDNoSIDMatchesFullQuirks() noexcept;
        static Trie<char32_t> loadPartialPIDMatchesLtdQuirks() noexcept;
        static Trie<char32_t> loadPartialPIDWithSIDMatchesLtdQuirks() noexcept;
    };
}

#endif //BLOGATOR_PARSER_SPECS_INFRA_DOCTYPE_H