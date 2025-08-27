#include "util.hpp"
#include "level.hpp"
#include "message.hpp"
#include "format.hpp"
#include "sink.hpp"

int main()
{
    /*
    std::cout << cpplogs::util::Date::getTime() << std::endl;
    std::string pathname = "./ab/cd/e.txt";
    cpplogs::util::File::createDirectory(cpplogs::util::File::path(pathname));
    */

    /*
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::UNKNOW) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::DEBUG) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::INFO) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::WARN) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::ERROR) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::FATAL) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::OFF) << std::endl;
    */

    cpplogs::LogMsg msg(cpplogs::LogLevel::value::INFO, __LINE__, __FILE__, "root", "TestInfo");
    cpplogs::Formmatter fmt;
    std::string str = fmt.format(msg);
    std::cout << str << std::endl;

    /*
    cpplogs::LogSink::ptr stdout_pls = cpplogs::SinkFactory::create<cpplogs::StdoutSink>();
    cpplogs::LogSink::ptr file_pls = cpplogs::SinkFactory::create<cpplogs::FileSink>("./test_log/file.log");
    cpplogs::LogSink::ptr roll_pls = cpplogs::SinkFactory::create<cpplogs::RollSinkBySize>("./test_log/roll-", 1024*1024);
    stdout_pls->log(str.c_str(), str.size());
    file_pls->log(str.c_str(), str.size());
    size_t cur_size = 0;
    size_t count = 0;
    while(cur_size < 1024 * 1024 * 50)
    {
        std::string roll_str = str + std::to_string(count++);
        roll_pls->log(roll_str.c_str(), roll_str.size());
        cur_size += roll_str.size();
    }
    */
    /*
    cpplogs::LogSink::ptr time_pls = cpplogs::SinkFactory::create<cpplogs::RollSinkByTime>("./test_log/timeroll-", cpplogs::TimeGap::GAP_MINUTE);
    time_t oldtime = cpplogs::util::Date::getTime();
    while(cpplogs::util::Date::getTime() < oldtime + 65)
    {
        time_pls->log(str.c_str(), str.size());
        sleep(1);
    }
    */
    cpplogs::LogSink::ptr time_pls = cpplogs::SinkFactory::create<cpplogs::RollSinkByTime>("./test_log/timeroll-", 5);
    time_t oldtime = cpplogs::util::Date::getTime();
    while(cpplogs::util::Date::getTime() < oldtime + 20)
    {
        time_pls->log(str.c_str(), str.size());
        sleep(1);
    }
    return 0;
}