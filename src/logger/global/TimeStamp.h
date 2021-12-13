#ifndef BLOGATOR_LOGGER_GLOBAL_TIMESTAMP_H
#define BLOGATOR_LOGGER_GLOBAL_TIMESTAMP_H

#include <string>
#include <chrono>
#include <iomanip>

namespace blogator::logger {
    class TimeStamp {
      public:
        typedef std::chrono::time_point<std::chrono::high_resolution_clock> HiResTimePoint_t;

        TimeStamp();
        explicit TimeStamp( HiResTimePoint_t tp );
        ~TimeStamp() = default;

        friend std::ostream & operator <<( std::ostream & os, const TimeStamp &ts );

        bool operator ==( const TimeStamp &rhs ) const;
        bool operator !=( const TimeStamp &rhs ) const;

        [[nodiscard]] std::string getTime( int fractional_precision = 0 ) const;
        [[nodiscard]] std::string getDate() const;
        [[nodiscard]] std::string getTimeStamp( const std::string &formatter, int fractional_precision = 0 ) const;
        [[nodiscard]] std::string getUTC( int fractional_precision = 0 ) const;
        [[nodiscard]] std::string getUTC( const std::string &formatter, int fractional_precision = 0 ) const;

      private:
        HiResTimePoint_t _time_point;

        void getFractional( std::ostream &os, int precision ) const;
    };

    /**
     * Output to stream as UTC timestamp
     * @param os Output stream
     * @param timestamp Timestamp object
     * @return Output stream
     */
    inline std::ostream & operator <<( std::ostream &os, const TimeStamp &timestamp ) {
        os << timestamp.getUTC( "%FT%TZ", 0 );
        return os;
    }
}

#endif //BLOGATOR_LOGGER_GLOBAL_TIMESTAMP_H