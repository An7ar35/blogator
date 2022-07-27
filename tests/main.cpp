#include "gtest/gtest.h"

#include "../src/logger/Logger.h"
#include "../src/parser/logging/ParserLog.h"

int main(int argc, char **argv) {
    blogator::logger::Logger::addOutput(
        blogator::logger::LogLevel::ERROR,
        blogator::logger::formatter::LogFormatterType::DEFAULT_PRINT_COLOUR,
        blogator::logger::output::LogOutputType::TERMINAL,
        "std colour terminal"
    );

    blogator::logger::Logger::start();
    blogator::parser::logging::ParserLog::setBuffering( false );

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}