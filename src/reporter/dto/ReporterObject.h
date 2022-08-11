#ifndef BLOGATOR_REPORTER_DTO_REPORTEROBJECT_H
#define BLOGATOR_REPORTER_DTO_REPORTEROBJECT_H

#include <filesystem>

#include "../enum/Context.h"
#include "../../dto/TextPos.h"

namespace blogator::reporter {
    class ReporterObject {
      public:
        ReporterObject();
        ReporterObject( std::filesystem::path src, Context ctx, int err_code, TextPos position );
        ReporterObject( std::filesystem::path src, Context ctx, int err_code, TextPos position, std::string txt );

        friend std::ostream & operator <<( std::ostream &os, const ReporterObject &err );

        bool operator ==( const ReporterObject &rhs ) const;
        bool operator !=( const ReporterObject &rhs ) const;

        [[nodiscard]] std::string filepath() const;
        [[nodiscard]] std::string context() const;
        [[nodiscard]] std::string error() const;
        [[nodiscard]] std::string detailed() const;
        [[nodiscard]] std::string position() const;
        [[nodiscard]] TextPos textpos() const;

        [[nodiscard]] Context ctxcode() const;
        [[nodiscard]] int errcode() const;

        std::ostream & filepath( std::ostream & os ) const;
        std::ostream & context( std::ostream & os ) const;
        std::ostream & error( std::ostream & os ) const;
        std::ostream & detailed( std::ostream & os ) const;
        std::ostream & position( std::ostream & os ) const;

      private:
        std::filesystem::path _src_file;
        Context               _context;
        int                   _code;
        std::string           _text;
        TextPos               _position;
    };

    std::ostream & operator <<( std::ostream &os, const ReporterObject &err );
}

#endif //BLOGATOR_REPORTER_DTO_REPORTEROBJECT_H
