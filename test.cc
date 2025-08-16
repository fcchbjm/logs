#include "util.hpp"
#include "level.hpp"
#include "message.hpp"
#include "format.hpp"

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
    cpplogs::Formmatter fmt("abc%%def[%d{%H:%M:%S}] %m%n %M%N");
    std::string str = fmt.format(msg);
    std::cout << str << std::endl;

    return 0;
}