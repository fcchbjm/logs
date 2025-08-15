#ifndef __LOGS_FMT_H__
#define __LOGS_FMT_H__

/* 格式化子项类
 * 从日志中取出指定的元素，追加到一块内存空间中
*/

#include "level.hpp"
#include "message.hpp"
#include <memory>
#include <ctime>

namespace cpplogs
{
    //抽象格式化子项基类
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream& out, cpplogs::LogMsg& msg) = 0;
    };

    //派生格式化子类 -- 消息，等级，时间，文件名，行号，线程ID，日志器名，制表符，换行，其它
    class MsgFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << msg._payload;
        }
    };

    class LevelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << cpplogs::LogLevel::toString(msg._level);
        }
    };

    class TimeFormatItem : public FormatItem
    {
    public:
        TimeFormatItem(const std::string& fmt = "%H:%M:%S")
        : _time_fmt(fmt)
        {}
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            struct tm t;
            localtime_r(&(msg._ctime), &t);
            char tmp[32] = { 0 };
            strftime(tmp, 31, _time_fmt.c_str(), &t);

            out << tmp;
        }
    private:
        std::string _time_fmt;
    };

    class FileFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << msg._file;
        }
    };

    class LineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << msg._line;
        }
    };

    class ThreadFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << msg._logger;
        }
    };

    class TabFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << "\t";
        }
    };

    class NewLineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << "\n";
        }
    };

    class OtherFormatItem : public FormatItem
    {
    public:
        OtherFormatItem(const std::string& str)
        :_str(str)
        {}
        void format(std::ostream& out, cpplogs::LogMsg& msg) override
        {
            out << _str;
        }
    private:
        std::string _str;
    };
}

#endif