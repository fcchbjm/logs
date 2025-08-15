#ifndef __LOGS_FMT_H__
#define __LOGS_FMT_H__

/* 格式化类
 * 从日志中取出指定的元素，追加到一块内存空间中
*/

#include "level.hpp"
#include "message.hpp"
#include <memory>
#include <ctime>
#include <vector>

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

    //格式化器
    class Formmater
    {
    public:
        /* 格式说明:
         * %d 表示日期，其中包含子格式 {%H:%M:%S}
         * %t 表示线程ID
         * %c 表示日志器名称
         * %f 表示源码文件名
         * %l 表示源码行号
         * %p 表示日志级别
         * %T 表示制表符缩进
         * %m 表示主体消息
         * %n 表示换行
         */
        Formmater(const std::string pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
        : _pattern(pattern)
        {}

        //对msg进行格式化
        std::string format(cpplogs::LogMsg& msg);
        void format(std::ostream& out, cpplogs::LogMsg& msg);

        //对格式化规则字符串进行解析
        bool parsePattern();

    private:
        //根据不同的格式化字符创建不同的格式化子项对象
        cpplogs::FormatItem::ptr createItem(const std::string& key, const std::string& val);

    private:
        std::string _pattern;//格式化规则字符串
        std::vector<cpplogs::FormatItem::ptr> _items;
    };
}

#endif