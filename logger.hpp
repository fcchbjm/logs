#ifndef __LOGS_LOGGER_H__
#define __LOGS_LOGGER_H__

/* 
 * logger.hpp 日志器模块
 * 1. 抽象日志器基类
 * 2. 派生出不同的子类（同步日志器类&异步日志器类）
 * 
*/

#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
#include "sink.hpp"
#include <atomic>
#include <mutex>

namespace cpplogs
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<cpplogs::Logger>;

        //完成构造日志对象信息并完成初始化，得到格式化后的日志消息字符串，最后落地输出
        void debug(const std::string& file, size_t line, const std::string& fmt, ...);
        void info(const std::string& file, size_t line, const std::string& fmt, ...);
        void warn(const std::string& file, size_t line, const std::string& fmt, ...);
        void error(const std::string& file, size_t line, const std::string& fmt, ...);
        void fatal(const std::string& file, size_t line, const std::string& fmt, ...);

    protected:
        //抽象接口完成实际的落地输出，不同的日志器有不同的输出方式
        virtual void log(const char* data, size_t len) = 0;
    private:
        std::mutex _mutex;//互斥锁
        std::string _logger_name;//日志器名称
        std::atomic<cpplogs::LogLevel::value> _limit_level;//日志限制等级
        cpplogs::Formmatter::ptr _formmater;//输出格式
        std::vector<cpplogs::LogSink> _sinks;//落地方向
    };

    class SyncLogger : public Logger
    {
    public:
        
    protected:
        
    private:
        
    };
}

#endif