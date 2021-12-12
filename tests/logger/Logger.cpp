#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../../src/logger/Logger.h"

#include "../AsyncNotify.h"

using namespace blogator::logger;

const uint64_t ASYNC_TEST_TIMEOUT = 100; //ms

class LogMsgCatcher : public formatter::LogFormatter {
  public:
    LogMsgCatcher( std::shared_ptr<std::vector<LogMsg>> catcher, std::function<void( const LogMsg & )> event_cb ) :
        formatter::LogFormatter( formatter::LogFormatterType::CUSTOM ),
        _messages( std::move( catcher ) ),
        _event_cb( std::move( event_cb ) )
    {}

    std::string format( const LogMsg &obj ) override {
        if( _messages ) {
            _messages->emplace_back( obj );
        }
        _event_cb( obj );
        return obj.message();
    }

  protected:
    void formatLogCounter( std::ostream &os, const LogMsg &obj ) override {}
    void formatLogLevel( std::ostream &os, const LogMsg &obj ) override {}
    void formatTimestamp( std::ostream &os, const LogMsg &obj ) override {}
    void formatSource( std::ostream &os, const LogMsg &obj ) override {}
    void formatMessageStr( std::ostream &os, const LogMsg &obj ) override {}

  private:
    std::function<void( const LogMsg & )> _event_cb;
    std::shared_ptr<std::vector<LogMsg>>  _messages;
};

/**
 * Test suite class for `Logger`
 * -----------------------------
 * Since Logger only has a single static instance that is shared across everything and processes LogMsg
 * objects asynchronously, the tests require a `LogMsg` "catcher". This is in the form of a `LogFormatter`
 * (`LogMsgCatcher`) which copies the LogMsgs and also uses a callback method to signal that there was
 * an event.
 *
 * The callback is setup to also pass the event's LogMsg so that it can be compared with an "expected"
 * object via yet another callback, this time injected in an `AsyncNotify` object. Since other tests
 * may run in parallel which may produce their own log events, this approach enables testing at each
 * event regardless of the origin.
 *
 * Admittedly it is a little tortuous but it is just to check that Log events are properly passed to
 * the worker thread. I.e. tests that the async producer->consumer system of the logger works as intended.
 */
class logger_Logger_Tests : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
        const auto event_cb = [AwaitNotificationObj = &(async_notify)]( const LogMsg &msg ) {
            AwaitNotificationObj->notify( msg );
        };

        Logger::addOutput( LogLevel::TRACE,
                           std::make_unique<LogMsgCatcher>( msgs, event_cb ),
                           output::LogOutputType::TERMINAL,
                           "LogMsgCatcher"
        );

        if( Logger::ready() ) {
            if( !Logger::start() ) {
                std::cerr << "[logger_Logger_Tests::SetUpTestSuite()] LOGGER DID NOT START!";
            }
        } else {
            std::cerr << "[logger_Logger_Tests::SetUpTestSuite()] LOGGER NOT READY!";
        }
    }

    void TearDown() override {
        async_notify.reset();
    }

    static std::shared_ptr<std::vector<LogMsg>> msgs; //collection of _any-and-all_ LogMsg created
    static blogator::tests::AsyncNotify<LogMsg> async_notify; //notification object
};

std::shared_ptr<std::vector<LogMsg>> logger_Logger_Tests::msgs = std::make_shared<std::vector<LogMsg>>();
blogator::tests::AsyncNotify<LogMsg> logger_Logger_Tests::async_notify = blogator::tests::AsyncNotify<LogMsg>();


TEST_F( logger_Logger_Tests, log_notice ) {
    auto expected_msg = LogMsg( 1, LogLevel::NOTICE, "Logger.cpp", 96, "integer 10" );

    auto check_fn = [&expected_msg]( const LogMsg &msg ){ return expected_msg.isEquivalent( msg ); };

    async_notify.setCheckFn( check_fn );

    LOG_NOTICE( "integer ", 10 );

    ASSERT_TRUE( Logger::running() ) << "Logger is not running.";
    ASSERT_TRUE( async_notify.waitForCheckSuccess( ASYNC_TEST_TIMEOUT ) ) << "Timed out waiting for LogMsg match.";

    logger_Logger_Tests::async_notify.reset();
}

