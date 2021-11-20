#ifndef BLOGATOR_TESTSTRINGS_H
#define BLOGATOR_TESTSTRINGS_H

#include <ostream>
#include <functional>

namespace blogator::tests {
    enum class EncodingFmt { UTF8, UTF16LE, UTF16BE, UTF32LE, UTF32BE };

    struct TestStrings {
        static void write( size_t id, EncodingFmt fmt, std::ostream &os );
        static const std::u32string & string( size_t id );
        static size_t count();

      private:
        static const size_t                                COUNT { 2 };
        static const std::u32string                        test_str   [COUNT];
        static const std::function<void( std::ostream & )> write_u8   [COUNT];
        static const std::function<void( std::ostream & )> write_u16le[COUNT];
        static const std::function<void( std::ostream & )> write_u16be[COUNT];
        static const std::function<void( std::ostream & )> write_u32le[COUNT];
        static const std::function<void( std::ostream & )> write_u32be[COUNT];
    };
}

#endif //BLOGATOR_TESTSTRINGS_H