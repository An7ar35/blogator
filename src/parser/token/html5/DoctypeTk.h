#ifndef BLOGATOR_PARSER_TOKEN_HTML5_DOCTYPETK_H
#define BLOGATOR_PARSER_TOKEN_HTML5_DOCTYPETK_H

#include "HTML5Tk.h"

namespace blogator::parser::token::html5 {
    /**
     * DOCTYPE HTML5 token
     */
    class DoctypeTk : public HTML5Tk {
      public:
        explicit DoctypeTk( TextPos position );

        [[nodiscard]] std::u32string name() const;
        [[nodiscard]] std::u32string publicIdentifier() const;
        [[nodiscard]] std::u32string systemIdentifier() const;

        [[nodiscard]] bool hasName() const;
        [[nodiscard]] bool hasPID() const;
        [[nodiscard]] bool hasSID() const;
        [[nodiscard]] bool forceQuirks() const;

        void setName( std::u32string name );
        void setPID( std::u32string pid );
        void setSID( std::u32string sid );
        void setForceQuirks( bool flag = true );

      protected:
        void toStr( std::ostream &os ) const override;

      private:
        bool           _has_name;
        bool           _pid_flag;
        bool           _sid_flag;
        bool           _force_quirks;
        std::u32string _public_id;
        std::u32string _system_id;
    };
}

#endif //BLOGATOR_PARSER_TOKEN_HTML5_DOCTYPETK_H
