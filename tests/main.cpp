#include "gtest/gtest.h"

#include "../src/logger/Logger.h"
#include "../src/reporter/ParseReporter.h"

int main(int argc, char **argv) {
    blogator::logger::Logger::addOutput(
        blogator::logger::LogLevel::ERROR,
        blogator::logger::formatter::LogFormatterType::DEFAULT_PRINT_COLOUR,
        blogator::logger::output::LogOutputType::TERMINAL,
        "std colour terminal"
    );

    blogator::logger::Logger::start();
    blogator::reporter::ParseReporter::setBuffering( false );

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}