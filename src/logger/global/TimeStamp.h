#ifndef BLOGATOR_LOGGER_GLOBAL_TIMESTAMP_H
#define BLOGATOR_LOGGER_GLOBAL_TIMESTAMP_H

#include <string>
#include <chrono>
#include <iomanip>

namespace blogator::logger {
    class TimeStamp {
      public:
        TimeStamp();
        explicit TimeStamp( std::time_t ts );
        ~TimeStamp() = default;

        friend std::ostream & operator <<( std::ostream & os, const TimeStamp &ts );

        bool operator ==( const TimeStamp &rhs ) const;
        bool operator !=( const TimeStamp &rhs ) const;

        [[nodiscard]] std::string getTime() const;
        [[nodiscard]] std::string getDate() const;
        [[nodiscard]] std::string getTimeStamp( const std::string &formatter ) const;
        [[nodiscard]] std::string getUTC() const;
        [[nodiscard]] std::string getUTC( const std::string &formatter ) const;

      private:
        std::time_t _ts;
    };

    /**
     * Output to stream as UTC timestamp
     * @param os Output stream
     * @param ts Timestamp object
     * @return Output stream
     */
    inline std::ostream & operator <<( std::ostream &os, const TimeStamp &ts ) {
        os << std::put_time( std::gmtime( &ts._ts ), "%FT%TZ" );
        return os;
    }
}

#endif //BLOGATOR_LOGGER_GLOBAL_TIMESTAMP_H