#ifndef BLOGATOR_TESTSTRINGS_H
#define BLOGATOR_TESTSTRINGS_H

#include <ostream>

namespace blogator::tests {
    struct TestStrings {
        static const std::u32string STR1; //"ℌ𝔢𝔩𝔩𝔬, 𝔴𝔬𝔯𝔩𝔡!"
        static void writeSTR1_utf8( std::ostream &os );
        static void writeSTR1_utf16LE( std::ostream &os );
        static void writeSTR1_utf16BE( std::ostream &os );
        static void writeSTR1_utf32LE( std::ostream &os );
        static void writeSTR1_utf32BE( std::ostream &os );
    };
}

#endif //BLOGATOR_TESTSTRINGS_H