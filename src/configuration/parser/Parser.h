#ifndef BLOGATOR_CONFIGURATION_PARSER_PARSER_H
#define BLOGATOR_CONFIGURATION_PARSER_PARSER_H

#include <deque>
#include <stack>
#include <set>
#include <memory>
#include <unordered_map>
#include <filesystem>

#include "specs/InsertionMode.h"
#include "specs/TokenType.h"
#include "token/ConfigTk.h"
#include "dto/NamedValues.h"
#include "dto/ValueCache.h"
#include "../dto/Configuration.h"

namespace blogator::configuration::parser::token {
    class ConfigTk;
}

namespace blogator::configuration::parser {
    /**
     * Configuration parser
     *
     * Takes ConfigTk tokens and creates a Configuration with keys and values from them.
     * ---------------------------------------------------------------------------------------------
     * Note:
     * · undeclared named values can be referenced but once declared they cannot be overridden
     */
    class Parser {
      public:
        Parser();

        void init( std::filesystem::path path );
        virtual void dispatch( std::unique_ptr<token::ConfigTk> token );
        std::unique_ptr<Configuration> reset();

        [[nodiscard]] size_t errors() const;

      private:
        typedef specs::InsertionMode                            InsertionMode_e;
        typedef std::unordered_map<std::u32string, NamedValues> NamedValues_t;
        typedef std::vector<Configuration::Key_t>               Scope_t;

        /**
         * Bracket
         * @param pos Position in source text
         * @param character Character in string
         */
        struct Bracket {
            TextPos        pos;
            std::u32string character;
        };

        /**
         * Cache
         * @param scope Holds the namespace keys for each current nested scope (i.e.: nested "{ ... }" brackets + current).
         * @param unresolved_named_values Keeps track of unresolved named values found and added to the Configuration during parsing
         * @param pending_named_value Holds the accumulated parts of a named value (e.g.: `:key = ns0:key;` -> `Key_t { ns0, key }`)
         * @param pending_named_value_position Holds the start position in source of the accumulated parts of a named value
         * @param opened_brackets Keeps track of opening and closing of brackets (i.e.: '(', ')', '{', '}', '[', ']').
         * @param value keeps track of the current value(s) and any nesting hierarchy
         */
        struct Cache {
            //ns: = ... <- if no "{..}" then warn in log
            //ns:key = ...
            //ns:key1, ns:key2, ..., ns:keyN = ... <- if "{..}" then applies to all key namespaces. e.g: { nsX:k } -> ns:key1:nsX:k, ns:key2:nsX:k ...
            //key = ...
            //key1, key2, ..., keyN = ...

            /*
             * +-------+
             * | Scope |
             * +-------+
             *
             * The purpose of the scope is to track what is currently opened in terms of namespaces so that the full
             * namespace address for a scoped key can be resolved correctly.
             * (i.e. `ns0 = { ns1 = { key = 0; } }` -> `ns0:ns1:key = 0;`)
             *
             * Example:
             * --------
             * src   -> ns0: = { key = "value A";
             *          |        ns1:,
             *          |        ns2:ns3: = { key,
             *          |        |            ns4:key = "value B"; }; };
             *          |        |            |
             *          |        |            |
             *         [0]      [1]          [2]
             *
             * scope @ "value A"
             *          [0] = Scope_t { Key_t { ns0 } }
             *          [1] = Scope_t { Key_t { key } }
             *
             * last value resolution @ "value A"
             *          `ns0:key = "value A";`
             *
             * > NOTE: Once the scope for `key = "value A";` is resolved the last scope is removed.
             * > ----  Then after, each time a scoped is closed (i.e.: with a '}'), a Scope_t is
             * >       removed from the back of the opened scopes.
             *
             * scope after "value A"
             *          [0] = Scope_t { Key_t { ns0 } }
             *
             * scope @ "value B"
             *          [0] = Scope_t { Key_t { ns0 } }
             *          [1] = Scope_t { Key_t { ns1 }, Key_t { ns2, ns3 } }
             *          [2] = Scope_t { Key_t { key }, Key_t { ns4, key } }
             *
             * last value resolution @ "value B"
             *          `ns0:ns1:key         = "value B";`     (0,0,0)
             *          `ns0:ns1:ns4:key     = "value B";`     (0,0,1)
             *          `ns0:ns2:ns3:key     = "value B";`     (0,1,0)
             *          `ns0:ns2:ns3:ns4:key = "value B";`     (0,1,1)
             */
            std::vector<Scope_t> scope;

            /*
             * +-------------------------------+
             * | Unresolved named values (UNV) |
             * +-------------------------------+
             *
             * Keeps track of unresolved named values found and added to the Configuration during parsing.
             *
             * Example:
             * --------
             *
             * `ns0:key = ns1:ns2:key;` leads to the creation of
             * a) a Configuration entry: `[ns0:key]: {null}`.
             * b) a cached entry in the UNV: `["ns1:ns2:key"]: { reference to the Value object @ ns0:key in Configuration }
             *
             * The unresolved named value cache holds a reference to the value field of the key that had the named value string
             *
             * Once the value for `ns1:ns2:key` is known (at the end of parsing), the value is set with the resolved value.
             */
            NamedValues_t unresolved_named_values;

            Configuration::Key_t             pending_named_value;
            TextPos                          pending_named_value_position;
            std::deque<Bracket>              opened_brackets;
            ValueCache                       value;
        };

        size_t                           _errors;
        std::filesystem::path            _src_path;
        std::unique_ptr<Configuration>   _config;
        InsertionMode_e                  _insertion_mode;
        std::unique_ptr<token::ConfigTk> _curr_token;
        std::unique_ptr<token::ConfigTk> _prev_token;
        Cache                            _cache;

        void logError( TextPos position, int err_code );
        void logError( TextPos position, int err_code, const std::string & str, bool print_insertion_mode = true );
        void logError( TextPos position, int err_code, const std::u32string & str, bool print_insertion_mode = true );
        void logError( TextPos position, int err_code, const token::ConfigTk & token );

        template<InsertionMode_e mode> void processContent( const std::unique_ptr<token::ConfigTk> & token );

        [[nodiscard]] InsertionMode_e currentInsertionMode() const;
        void setInsertionMode( InsertionMode_e mode );

        const std::unique_ptr<token::ConfigTk> & currentToken() const;
        const std::unique_ptr<token::ConfigTk> & previousToken() const;
        static bool match( const std::unique_ptr<token::ConfigTk> & tk, specs::TokenType tk_type ) ;
        static bool match( const std::unique_ptr<token::ConfigTk> & tk, const std::u32string & txt )  ;

        Scope_t & createScope();
        Scope_t & lastScope();
        Scope_t::value_type & createScopedKey();
        Scope_t::value_type & lastScopedKey();
        void popLastScope();
        [[nodiscard]] bool hasScope() const;
        [[nodiscard]] bool emptyLastScopedKey() const;
        [[nodiscard]] bool emptyKeysInLastScope() const;
        [[nodiscard]] bool inRootScope() const;

        void addValueToCache( Value::Type_e value_type, const std::u32string & value_str, const TextPos & position );
        void addToConfiguration( ValueCache & cached_value );
        void addToConfiguration_( std::vector<Scope_t>::const_iterator it, const Configuration::Key_t & root_ns, ValueCache & cached_value );
        size_t resolveNamedValue( const Configuration::Key_t & nsk, const TextPos & position, Value & value );

        [[nodiscard]] const Configuration::Key_t & pendingNamedValue() const;
        [[nodiscard]] const TextPos & pendingNamedValuePosition() const;
        void appendToPendingNamedValue( const std::u32string & str );
        void resetPendingNamedValueCache( const TextPos & pos );

        void cacheUnresolvedNamedValue( const std::u32string & nsk, NamedValues::ValueRef value_ref );
        bool resolveCachedNamedValue( const std::u32string & nsk, const Value * value );

        void pushBracket( std::u32string c, TextPos position );
        std::u32string currBracket() const;
        void popBracket();
        bool closeBracketsDownTo( const std::u32string & bracket_char, const TextPos & position );
        [[nodiscard]] bool inBracketScope( const std::u32string & bracket_char );

        void cleanupEOF();
    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_PARSER_H