#include "gtest/gtest.h"
#include "../../src/unicode/utf32.h"

#include <fstream>
#include "../helper.h"

using namespace blogator::unicode::utf32;

TEST( unicode_utf32, join ) {
    ASSERT_EQ( 0b00000000'00000000'00000000'00000000, join( 0b00000000, 0b00000000, 0b00000000, 0b00000000 ) );
    ASSERT_EQ( 0b11111111'11111111'11111111'11111111, join( 0b11111111, 0b11111111, 0b11111111, 0b11111111 ) );
    ASSERT_EQ( 0b11111111'00000000'11111111'00000000, join( 0b11111111, 0b00000000, 0b11111111, 0b00000000 ) );
    ASSERT_EQ( 0b00000000'11111111'00000000'11111111, join( 0b00000000, 0b11111111, 0b00000000, 0b11111111 ) );
    ASSERT_EQ( 0b11110000'00111100'00011110'00001111, join( 0b11110000, 0b00111100, 0b00011110, 0b00001111 ) );
}

TEST( unicode_utf32, toxunicode ) {
    ASSERT_EQ( toxunicode( 0, U"U+" ), U"U+0000" );
    ASSERT_EQ( toxunicode( 1, U"U+" ), U"U+0001" );
    ASSERT_EQ( toxunicode( 162, U"U+" ), U"U+00A2" );
    ASSERT_EQ( toxunicode( 4095, U"U+" ), U"U+0FFF" );
    ASSERT_EQ( toxunicode( 59418, U"U+" ), U"U+E81A" );
    ASSERT_EQ( toxunicode( 65535, U"U+" ), U"U+FFFF" );
    ASSERT_EQ( toxunicode( 131071, U"U+" ), U"U+1FFFF" );
    ASSERT_EQ( toxunicode( 6946816, U"U+" ), U"U+6A0000" );
}

TEST( unicode_utf32, toU8_codeunits_1 ) {
    auto test_case = std::vector<uint32_t>( {
        0x00,
        0x61, //'a'
        0x7F
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        char8_t b1 = 0x00, b2 = 0x00, b3 = 0x00, b4 = 0x00;
        ASSERT_EQ( 1, toU8( test_case[i], b1, b2, b3, b4 ) );
        ASSERT_EQ( test_case[i], b1 ) << "Failed: " << (uint8_t) b1 << " != test_case[" << i << "] (" << (uint8_t) test_case[i] << ")";
    }
}

TEST( unicode_utf32, toU8_codeunits_2 ) {
    struct TestCase { uint32_t u32_cp; char8_t b1; char8_t b2; };

    auto test_case = std::vector<TestCase>( {
        { 0x080, 0xC2, 0x80 },
        { 0x52A, 0xD4, 0xAA },
        { 0x7FF, 0xDF, 0xBF },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        char8_t b1 = 0x00, b2 = 0x00, b3 = 0x00, b4 = 0x00;
        ASSERT_EQ( 2, toU8( test_case[i].u32_cp, b1, b2, b3, b4 ) );
        ASSERT_EQ( test_case[i].b1, b1 ) << "Failed: " << (uint8_t) b1 << " != test_case[" << i << "].b1 (" << (uint8_t) test_case[i].b1 << ")";
        ASSERT_EQ( test_case[i].b2, b2 ) << "Failed: " << (uint8_t) b2 << " != test_case[" << i << "].b2 (" << (uint8_t) test_case[i].b2 << ")";
    }
}

TEST( unicode_utf32, toU8_codeunits_3 ) {
    struct TestCase { uint32_t u32_cp; char8_t b1; char8_t b2; char8_t b3; };

    auto test_case = std::vector<TestCase>( {
        { 0x0800, 0xE0, 0xA0, 0x80 },
        { 0xA1B5, 0xEA, 0x86, 0xB5 },
        { 0xFFFF, 0xEF, 0xBF, 0xBF },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        char8_t b1 = 0x00, b2 = 0x00, b3 = 0x00, b4 = 0x00;
        ASSERT_EQ( 3, toU8( test_case[i].u32_cp, b1, b2, b3, b4 ) );
        ASSERT_EQ( test_case[i].b1, b1 ) << "Failed: " << (uint8_t) b1 << " != test_case[" << i << "].b1 (" << (uint8_t) test_case[i].b1 << ")";
        ASSERT_EQ( test_case[i].b2, b2 ) << "Failed: " << (uint8_t) b2 << " != test_case[" << i << "].b2 (" << (uint8_t) test_case[i].b2 << ")";
        ASSERT_EQ( test_case[i].b3, b3 ) << "Failed: " << (uint8_t) b3 << " != test_case[" << i << "].b3 (" << (uint8_t) test_case[i].b3 << ")";
    }
}

TEST( unicode_utf32, toU8_codeunits_4 ) {
    struct TestCase { uint32_t u32_cp; char8_t b1; char8_t b2; char8_t b3; char8_t b4; };

    auto test_case = std::vector<TestCase>( {
        { 0x010000, 0xF0, 0x90, 0x80, 0x80 },
        { 0x0C5112, 0xF3, 0x85, 0x84, 0x92 },
        { 0x10FFFF, 0xF4, 0x8F, 0xBF, 0xBF },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        char8_t b1 = 0x00, b2 = 0x00, b3 = 0x00, b4 = 0x00;
        ASSERT_EQ( 4, toU8( test_case[i].u32_cp, b1, b2, b3, b4 ) );
        ASSERT_EQ( test_case[i].b1, b1 ) << "Failed: " << (uint8_t) b1 << " != test_case[" << i << "].b1 (" << (uint8_t) test_case[i].b1 << ")";
        ASSERT_EQ( test_case[i].b2, b2 ) << "Failed: " << (uint8_t) b2 << " != test_case[" << i << "].b2 (" << (uint8_t) test_case[i].b2 << ")";
        ASSERT_EQ( test_case[i].b3, b3 ) << "Failed: " << (uint8_t) b3 << " != test_case[" << i << "].b3 (" << (uint8_t) test_case[i].b3 << ")";
        ASSERT_EQ( test_case[i].b4, b4 ) << "Failed: " << (uint8_t) b4 << " != test_case[" << i << "].b4 (" << (uint8_t) test_case[i].b4 << ")";
    }
}

TEST( unicode_utf32, toU8_codeunits_fail ) {
    char8_t b1 = 0x00, b2 = 0x00, b3 = 0x00, b4 = 0x00;
    ASSERT_EQ( 0, toU8( 0x110000, b1, b2, b3, b4 ) );
}

TEST( unicode_utf32, toU8_stream_1byte ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x00, { 0x00 } },
        { 0x61, { 0x61 } }, //'a'
        { 0x7F, { 0x7F } }
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 1, toU8( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU8_stream_2bytes ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x080, { 0xC2, 0x80 } },
        { 0x52A, { 0xD4, 0xAA } },
        { 0x7FF, { 0xDF, 0xBF } },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 2, toU8( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU8_stream_3bytes ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x0800, { 0xE0, 0xA0, 0x80 } },
        { 0xA1B5, { 0xEA, 0x86, 0xB5 } },
        { 0xFFFF, { 0xEF, 0xBF, 0xBF } },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 3, toU8( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU8_stream_4bytes ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x010000, { 0xF0, 0x90, 0x80, 0x80 } },
        { 0x0C5112, { 0xF3, 0x85, 0x84, 0x92 } } ,
        { 0x10FFFF, { 0xF4, 0x8F, 0xBF, 0xBF } },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 4, toU8( test_case[i].u32_cp,ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU8_stream_fail ) {
    std::stringstream ss;
    ASSERT_EQ( 0, toU8( 0x110000, ss ) );
    ASSERT_TRUE( ss.str().empty() );
}

TEST( unicode_utf32, toU8_stream_from_file ) {
    std::filesystem::path source = blogator::tests::W3_TEST_FILE_DIRECTORY_PATH + "/UTF-8-demo.html"; //https://www.w3.org/2001/06/utf-8-test/
    std::ifstream         file;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter; //to check against

    auto src_u8  = blogator::tests::loadFile( source );
    auto src_u32 = converter.from_bytes( src_u8 );

    std::stringstream u8_bytes;

    for( auto c : src_u32 ) {
        toU8( c, u8_bytes );
    }

    auto result = u8_bytes.str();

    ASSERT_EQ( result, src_u8 );;
}

TEST( unicode_utf32, toU16_codeunits_1 ) {
    auto test_case = std::vector<uint32_t>( {
        0x0000,
        0x0061, //'a'
        0xD7FF,
        0xE000,
        0xFFFF
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        uint16_t hi = 0x00, lo = 0x00;
        ASSERT_EQ( 1, toU16( test_case[i], hi, lo ) );
        ASSERT_EQ( test_case[i], hi ) << "Failed: " << hi << " != test_case[" << i << "] (" << test_case[i] << ")";
    }
}

TEST( unicode_utf32, toU16_codeunits_2 ) {
    struct TestCase { uint32_t u32_cp; uint16_t u16_hi; uint16_t u16_lo; };

    auto test_case = std::vector<TestCase>( {
        { 0x010000, 0xD800, 0xDC00 },
        { 0x01FAD6, 0xD83E, 0xDED6 }, //🫖
        { 0x10FFFF, 0xDBFF, 0xDFFF },
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        uint16_t hi = 0x00, lo = 0x00;
        ASSERT_EQ( 2, toU16( test_case[i].u32_cp, hi, lo ) );
        ASSERT_EQ( test_case[i].u16_hi, hi ) << "Failed: " << hi << " != test_case[" << i << "].u16_hi (" << test_case[i].u16_hi << ")";
        ASSERT_EQ( test_case[i].u16_lo, lo ) << "Failed: " << lo << " != test_case[" << i << "].u16_lo (" << test_case[i].u16_lo << ")";
    }
}

TEST( unicode_utf32, toU16LE_stream_1 ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x0000, { 0x00, 0x00 } },
        { 0x0061, { 0x61, 0x00 } }, //'a'
        { 0xD7FF, { 0xFF, 0xD7 } },
        { 0xE000, { 0x00, 0xE0 } },
        { 0xFFFF, { 0xFF, 0xFF } }
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 1, toU16LE( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU16LE_stream_2 ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x010000, { 0x00, 0xD8, 0x00, 0xDC } }, //0xD800, 0xDC00
        { 0x01FAD6, { 0x3E, 0xD8, 0xD6, 0xDE } }, //0xD83E, 0xDED6 (🫖)
        { 0x10FFFF, { 0xFF, 0xDB, 0xFF, 0xDF } }, //0xDBFF, 0xDFFF
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 2, toU16LE( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU16BE_stream_1 ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x0000, { 0x00, 0x00 } },
        { 0x0061, { 0x00, 0x61 } }, //'a'
        { 0xD7FF, { 0xD7, 0xFF } },
        { 0xE000, { 0xE0, 0x00 } },
        { 0xFFFF, { 0xFF, 0xFF } }
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 1, toU16BE( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}

TEST( unicode_utf32, toU16BE_stream_2 ) {
    struct TestCase { uint32_t u32_cp; std::vector<char8_t> bytes; };

    auto test_case = std::vector<TestCase>( {
        { 0x010000, { 0xD8, 0x00, 0xDC, 0x00 } }, //0xD800, 0xDC00
        { 0x01FAD6, { 0xD8, 0x3E, 0xDE, 0xD6 } }, //0xD83E, 0xDED6 (🫖)
        { 0x10FFFF, { 0xDB, 0xFF, 0xDF, 0xFF } }, //0xDBFF, 0xDFFF
    } );

    for( auto i = 0; i< test_case.size(); ++i ) {
        std::stringstream ss;
        ASSERT_EQ( 2, toU16BE( test_case[i].u32_cp, ss ) );
        auto out = ss.str();
        ASSERT_EQ( std::string( test_case[i].bytes.begin(), test_case[i].bytes.end() ), out ) << "Failed: test_case[" << i << "]";
    }
}