/*
 * level.hpp 日志等级类
 * 1. 定义枚举类，枚举日志等级
 * 2. 提供转换接口：将枚举转换成对应字符串
*/

namespace cpplogs
{
    class LogLevel
    {
    public:
        enum class value
        {
            UNKNOW = 0,
            OFF,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };
        const char* toString(cpplogs::LogLevel::value level)
        {
            switch (level)
            {
            case cpplogs::LogLevel::value::OFF:
                return "OFF";
                break;
            case cpplogs::LogLevel::value::DEBUG:
                return "DEBUG";
                break;
            case cpplogs::LogLevel::value::INFO:
                return "INFO";
                break;
            case cpplogs::LogLevel::value::WARN:
                return "WARN";
                break;
            case cpplogs::LogLevel::value::FATAL:
                return "FATAL";
                break;
            default:
                return "UNKNOW";
                break;
            }
        }
    };
}