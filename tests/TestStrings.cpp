#include "TestStrings.h"

#include <cassert>

using blogator::tests::TestStrings;

const std::u32string TestStrings::test_str[2] = {
    U"Hello, world!",

    U"\U0000210c\U0001d522\U0001d529\U0001d529\U0001d52c\U0000002c"
     "\U00000020\U0001d534\U0001d52c\U0001d52f\U0001d529\U0001d521"
     "\U00000021" //"ℌ𝔢𝔩𝔩𝔬, 𝔴𝔬𝔯𝔩𝔡!"
};


const std::function<void( std::ostream & )> TestStrings::write_u8[2] = {
    []( std::ostream & os ) {
        os << (char) 0x48 << (char) 0x65 << (char) 0x6C << (char) 0x6C
           << (char) 0x6F << (char) 0x2C << (char) 0x20 << (char) 0x77
           << (char) 0x6F << (char) 0x72 << (char) 0x6C << (char) 0x64
           << (char) 0x21;
    },

    []( std::ostream & os ) {
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
};

const std::function<void( std::ostream & )> TestStrings::write_u16le[2] = {
    []( std::ostream &os ) {
        os << (char) 0x48 << (char) 0x00 //4800
           << (char) 0x65 << (char) 0x00 //6500
           << (char) 0x6C << (char) 0x00 //6c00
           << (char) 0x6C << (char) 0x00 //6c00
           << (char) 0x6F << (char) 0x00 //6f00
           << (char) 0x2C << (char) 0x00 //2c00
           << (char) 0x20 << (char) 0x00 //2000
           << (char) 0x77 << (char) 0x00 //7700
           << (char) 0x6F << (char) 0x00 //6f00
           << (char) 0x72 << (char) 0x00 //7200
           << (char) 0x6C << (char) 0x00 //6c00
           << (char) 0x64 << (char) 0x00 //6400
           << (char) 0x21 << (char) 0x00;//2100
    },

    []( std::ostream &os ) {
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
};

const std::function<void( std::ostream & )> TestStrings::write_u16be[2] = {
    []( std::ostream &os ) {
        os << (char) 0x00 << (char) 0x48 //0048
           << (char) 0x00 << (char) 0x65 //0065
           << (char) 0x00 << (char) 0x6C //006c
           << (char) 0x00 << (char) 0x6C //006c
           << (char) 0x00 << (char) 0x6F //006f
           << (char) 0x00 << (char) 0x2C //002c
           << (char) 0x00 << (char) 0x20 //0020
           << (char) 0x00 << (char) 0x77 //0077
           << (char) 0x00 << (char) 0x6F //006f
           << (char) 0x00 << (char) 0x72 //0072
           << (char) 0x00 << (char) 0x6C //006c
           << (char) 0x00 << (char) 0x64 //0064
           << (char) 0x00 << (char) 0x21;//0021
    },

    []( std::ostream &os ) {
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
};

const std::function<void( std::ostream & )> TestStrings::write_u32le[2] = {
    []( std::ostream &os ) {
        os << (char) 0x48 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x48000000
           << (char) 0x65 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x65000000
           << (char) 0x6C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x6c000000
           << (char) 0x6C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x6c000000
           << (char) 0x6F << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x6f000000
           << (char) 0x2C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x2c000000
           << (char) 0x20 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x20000000
           << (char) 0x77 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x77000000
           << (char) 0x6F << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x6f000000
           << (char) 0x72 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x72000000
           << (char) 0x6C << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x6c000000
           << (char) 0x64 << (char) 0x00 << (char) 0x00 << (char) 0x00 //0x64000000
           << (char) 0x21 << (char) 0x00 << (char) 0x00 << (char) 0x00;//0x21000000
    },

    []( std::ostream &os ) {
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
};

const std::function<void( std::ostream & )> TestStrings::write_u32be[2] = {
    []( std::ostream &os ) {
        os << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x48 //0x00000048
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x65 //0x00000065
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x6C //0x0000006c
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x6C //0x0000006c
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x6F //0x0000006f
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x2C //0x0000002c
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x20 //0x00000020
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x77 //0x00000077
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x6F //0x0000006f
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x72 //0x00000072
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x6C //0x0000006c
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x64 //0x00000064
           << (char) 0x00 << (char) 0x00 << (char) 0x00 << (char) 0x21;//0x00000021
    },

    []( std::ostream &os ) {
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
};

/**
 * Writes a the byte stream of a test string
 * @param id String ID
 * @param fmt Output format
 * @param os Output stream
 */
void TestStrings::write( size_t id, EncodingFmt fmt, std::ostream &os ) {
    assert( id < COUNT );

    switch( fmt ) {
        case EncodingFmt::UTF8   : { write_u8   [id]( os ); } break;
        case EncodingFmt::UTF16LE: { write_u16le[id]( os ); } break;
        case EncodingFmt::UTF16BE: { write_u16be[id]( os ); } break;
        case EncodingFmt::UTF32LE: { write_u32le[id]( os ); } break;
        case EncodingFmt::UTF32BE: { write_u32be[id]( os ); } break;
    }
}

/**
 * Gets a UTF-32 test string
 * @param id String ID
 * @return Test string
 */
const std::u32string & TestStrings::string( size_t id ) {
    assert( id < COUNT );
    return test_str[id];
}

/**
 * Gets the number of test strings available
 * @return Test string count
 */
size_t TestStrings::count() {
    return COUNT;
}