#include "TestStrings.h"

using blogator::tests::TestStrings;

const std::u32string TestStrings::STR1 = U"\U0000210c\U0001d522\U0001d529\U0001d529"
                                          "\U0001d52c\U0000002c\U00000020\U0001d534"
                                          "\U0001d52c\U0001d52f\U0001d529\U0001d521"
                                          "\U00000021"; //"ℌ𝔢𝔩𝔩𝔬, 𝔴𝔬𝔯𝔩𝔡!"

void blogator::tests::TestStrings::writeSTR1_utf8( std::ostream &os ) {
    os << (char) 0xe2 << (char) 0x84 << (char) 0x8c << (char) 0xf0
       << (char) 0x9d << (char) 0x94 << (char) 0xa2 << (char) 0xf0
       << (char) 0x9d << (char) 0x94 << (char) 0xa9 << (char) 0xf0
       << (char) 0x9d << (char) 0x94 << (char) 0xa9 << (char) 0xf0
       << (char) 0x9d << (char) 0x94 << (char) 0xac << (char) 0x2c
       << (char) 0x20 << (char) 0xf0 << (char) 0x9d << (char) 0x94
       << (char) 0xb4 << (char) 0xf0 << (char) 0x9d << (char) 0x94
       << (char) 0xac << (char) 0xf0 << (char) 0x9d << (char) 0x94
       << (char) 0xaf << (char) 0xf0 << (char) 0x9d << (char) 0x94
       << (char) 0xa9 << (char) 0xf0 << (char) 0x9d << (char) 0x94
       << (char) 0xa1 << (char) 0x21;
}

void blogator::tests::TestStrings::writeSTR1_utf16LE( std::ostream &os ) {
    os << (char) 0x0C << (char) 0x21 << (char) 0x35 << (char) 0xD8 //c210 35d8
       << (char) 0x22 << (char) 0xDD << (char) 0x35 << (char) 0xD8 //22dd 35d8
       << (char) 0x29 << (char) 0xDD << (char) 0x35 << (char) 0xD8 //29dd 35d8
       << (char) 0x29 << (char) 0xDD << (char) 0x35 << (char) 0xD8 //29dd 35d8
       << (char) 0x2C << (char) 0xDD << (char) 0x2C << (char) 0x00 //2cdd 2c00
       << (char) 0x20 << (char) 0x00 << (char) 0x35 << (char) 0xD8 //2000 35d8
       << (char) 0x34 << (char) 0xDD << (char) 0x35 << (char) 0xD8 //34dd 35d8
       << (char) 0x2C << (char) 0xDD << (char) 0x35 << (char) 0xD8 //2cdd 35d8
       << (char) 0x2F << (char) 0xDD << (char) 0x35 << (char) 0xD8 //2fdd 35d8
       << (char) 0x29 << (char) 0xDD << (char) 0x35 << (char) 0xD8 //29dd 35d8
       << (char) 0x21 << (char) 0xDD << (char) 0x21 << (char) 0x00;//21dd 2100
}

void blogator::tests::TestStrings::writeSTR1_utf16BE( std::ostream &os ) {
    os << (char) 0x21 << (char) 0x0C << (char) 0xD8 << (char) 0x35 //210c d835
       << (char) 0xDD << (char) 0x22 << (char) 0xD8 << (char) 0x35 //dd22 d835
       << (char) 0xDD << (char) 0x29 << (char) 0xD8 << (char) 0x35 //dd29 d835
       << (char) 0xDD << (char) 0x29 << (char) 0xD8 << (char) 0x35 //dd29 d835
       << (char) 0xDD << (char) 0x2C << (char) 0x00 << (char) 0x2C //dd2c 002c
       << (char) 0x00 << (char) 0x20 << (char) 0xD8 << (char) 0x35 //0020 d835
       << (char) 0xDD << (char) 0x34 << (char) 0xD8 << (char) 0x35 //dd34 d835
       << (char) 0xDD << (char) 0x2C << (char) 0xD8 << (char) 0x35 //dd2c d835
       << (char) 0xDD << (char) 0x2F << (char) 0xD8 << (char) 0x35 //dd2f d835
       << (char) 0xDD << (char) 0x29 << (char) 0xD8 << (char) 0x35 //dd29 d835
       << (char) 0xDD << (char) 0x21 << (char) 0x00 << (char) 0x21;//dd21 0021
}

void blogator::tests::TestStrings::writeSTR1_utf32LE( std::ostream &os ) {
    os << (char) 0x0C << (char) 0x21 << (char) 0x00 << (char) 0x00 //0x0000210c
       << (char) 0x22 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d522
       << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
       << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
       << (char) 0x2C << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52c
       << (char) 0x2C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x0000002c
       << (char) 0x20 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x00000020
       << (char) 0x34 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d534
       << (char) 0x2C << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52c
       << (char) 0x2F << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d52f
       << (char) 0x29 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d529
       << (char) 0x21 << (char) 0xD5 << (char) 0x01 << (char) 0x00 //0x0001d521
       << (char) 0x21 << (char) 0x00 << (char) 0x00 << (char) 0x00;//0x00000021
}

void blogator::tests::TestStrings::writeSTR1_utf32BE( std::ostream &os ) {
    os << (char) 0x00 << (char) 0x00 << (char) 0x21 << (char) 0x0C //0x0000210c
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x22 //0x0001d522
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2C //0x0001d52c
       << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x2C //0x0000002c
       << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x20 //0x00000020
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x34 //0x0001d534
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2C //0x0001d52c
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x2F //0x0001d52f
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x29 //0x0001d529
       << (char) 0x00 << (char) 0x01 << (char) 0xD5 << (char) 0x21 //0x0001d521
       << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x21;//0x00000021
}