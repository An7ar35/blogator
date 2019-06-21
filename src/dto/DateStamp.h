#ifndef BLOGATOR_DTO_DATESTAMP_H
#define BLOGATOR_DTO_DATESTAMP_H

namespace blogator::dto {
    struct DateStamp {
        DateStamp() = default;

        DateStamp( unsigned year, unsigned month, unsigned day ) :
            _year( year ),
            _month( month ),
            _day( day )
        {}

        friend std::ostream &operator <<( std::ostream &s, const DateStamp &stamp ) {
            s << stamp._day << "/" << stamp._month << "/" << stamp._year;
            return s;
        }

        unsigned _year;
        unsigned _month;
        unsigned _day;

        static int compare( const DateStamp &a, const DateStamp &b ) {
            if( a._year < b._year )
                return -1;
            if( a._year > b._year )
                return 1;
            if( a._month < b._month )
                return -1;
            if( a._month > b._month )
                return 1;
            if( a._day < b._day )
                return -1;
            if( a._day > b._day )
                return 1;
            return 0;
        }

        static unsigned concatYYYYMM( const DateStamp &stamp ) {
            return stamp._year * 100 + stamp._month;
        }
    };
}

#endif //BLOGATOR_DTO_DATESTAMP_H

