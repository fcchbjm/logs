#include "util.hpp"
#include "level.hpp"
#include "message.hpp"

int main()
{
    /*
    std::cout << cpplogs::util::Date::getTime() << std::endl;
    std::string pathname = "./ab/cd/e.txt";
    cpplogs::util::File::createDirectory(cpplogs::util::File::path(pathname));
    */

    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::UNKNOW) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::DEBUG) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::INFO) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::WARN) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::ERROR) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::FATAL) << std::endl;
    std::cout << cpplogs::LogLevel::toString(cpplogs::LogLevel::value::OFF) << std::endl;
    
    return 0;
}