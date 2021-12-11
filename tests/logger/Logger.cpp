#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../../src/logger/Logger.h"

using namespace blogator::logger;

//TODO launch logger with custom formatter to intercept LogMsg for testing

class LogMsgCatcher : public formatter::LogFormatter {
  public:
    LogMsgCatcher( std::shared_ptr<std::vector<LogMsg>> catcher ) :
        formatter::LogFormatter( formatter::LogFormatterType::CUSTOM ),
        _messages( std::move( catcher ) )
    {}

    std::string format( const LogMsg &obj ) override {
        if( _messages ) {
            _messages->emplace_back( obj );
        }

        return obj.message();
    }

  protected:
    void formatLogCounter( std::ostream &os, const LogMsg &obj ) override {}
    void formatLogLevel( std::ostream &os, const LogMsg &obj ) override {}
    void formatTimestamp( std::ostream &os, const LogMsg &obj ) override {}
    void formatSource( std::ostream &os, const LogMsg &obj ) override {}
    void formatMessageStr( std::ostream &os, const LogMsg &obj ) override {}

  private:
    std::shared_ptr<std::vector<LogMsg>> _messages;
};

//TODO create a blocking object that waits on either a timeout or a callback to unblock

TEST( logger_Logger_Tests, print_0 ) {
    auto msgs = std::make_shared<std::vector<LogMsg>>();

    ASSERT_FALSE( Logger::ready() );

    Logger::addOutput( LogLevel::TRACE,
                       std::make_unique<LogMsgCatcher>( msgs ),
                           output::LogOutputType::TERMINAL,
                           "LogMsgCatcher" );

    ASSERT_TRUE( Logger::ready() );
    ASSERT_TRUE( Logger::start() );

    LOG_NOTICE( "integer ", 10 );


    ASSERT_EQ( msgs->size(), 1 );

    auto log_msg = msgs->front();
    ASSERT_EQ( log_msg.id(), 0 );
    ASSERT_EQ( log_msg.source(), "Logger.cpp" );
    ASSERT_EQ( log_msg.line(), 47 );
    ASSERT_EQ( log_msg.level(), LogLevel::NOTICE );
    ASSERT_EQ( log_msg.message(), "integer 10" );
}