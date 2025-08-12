#ifndef __LOGS_MESSAGE_H__
#define __LOGS_MESSAGE_H__

/*
 * message.hpp 日志消息类，进行日志中间信息的存储
 * 1. 日志输出时间
 * 2. 日志等级，进行日志过滤
 * 3. 源文件名称
 * 4. 源文件行号，定位出错的代码位置
 * 5. 线程ID，定位出错的线程
 * 6. 日志主体消息
 * 7. 日志器名称，支持多日志器同时使用
*/

#include "level.hpp"
#include "util.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace cpplogs
{
    struct LogMsg
    {
        time_t _ctime;//日志产生的时间戳
        size_t _line;//行号
        std::thread::id _tid;//线程ID
        cpplogs::LogLevel::value _level;//日志等级
        std::string _file;//文件
        std::string _logger;//日志器名称
        std::string _payload;//日志信息的有效载荷

        LogMsg(cpplogs::LogLevel::value level,
            size_t line,
            const std::string file,
            const std::string logger,
            const std::string msg)
            : _ctime(cpplogs::util::Date::getTime())
            , _level(level)
            , _line(line)
            , _tid(std::this_thread::get_id())
            , _file(file)
            , _logger(logger)
            , _payload(msg) 
            {}
    };
    
}

#endif