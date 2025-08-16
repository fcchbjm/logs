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
#include <cassert>
#include <sstream>

namespace cpplogs
{
    //抽象格式化子项基类
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream& out, const cpplogs::LogMsg& msg) = 0;
    };

    //派生格式化子类 -- 消息，等级，时间，文件名，行号，线程ID，日志器名，制表符，换行，其它
    class MsgFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << msg._payload;
        }
    };

    class LevelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
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
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
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
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << msg._file;
        }
    };

    class LineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << msg._line;
        }
    };

    class ThreadFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << msg._logger;
        }
    };

    class TabFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << "\t";
        }
    };

    class NewLineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
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
        void format(std::ostream& out, const cpplogs::LogMsg& msg) override
        {
            out << _str;
        }
    private:
        std::string _str;
    };

    //格式化器
    class Formmatter
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
        Formmatter(const std::string pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
        : _pattern(pattern)
        {
            assert(parsePattern());
        }

        //对msg进行格式化
        std::string format(const cpplogs::LogMsg& msg)
        {
            std::stringstream ss;
            format(ss, msg);
            return ss.str();
        }
        void format(std::ostream& out, const cpplogs::LogMsg& msg)
        {
            for(auto& item : _items)
            {
                item->format(out, msg);
            }
        }

    private:
        //对格式化规则字符串进行解析
        bool parsePattern()
        {
            //对格式化规则字符串进行解析
            std::vector<std::pair<std::string, std::string>> fmt_order;
            size_t pos = 0;
            std::string key, val;
            while(pos < _pattern.size())
            {
                //处理原始字符串-是否为%，如果不是，则为原始字符
                if(_pattern[pos] != '%')
                {
                    val.push_back(_pattern[pos++]);
                    continue;
                }
                else if(pos + 1 < _pattern.size() && _pattern[pos + 1] == '%')//%% 为 '%'（类似转义字符）
                {
                    val.push_back('%');
                    pos += 2;
                    continue;
                }
                else//原始字符串处理完毕，开始处理格式化字符
                {
                    if(!val.empty())
                    {
                        fmt_order.push_back(std::make_pair("", val));
                        val.clear();
                    }
                    pos += 1;//pos 指向格式化字符位置
                    if(_pattern.size() == pos)
                    {
                        std::cerr << "[ERROR]cpplogs::Formmatter::parsePattern::未匹配的%." << std::endl;
                        return false;
                    }
                    key = _pattern[pos];
                    pos += 1;//pos 指向格式化字符下一位
                    if(pos < _pattern.size() && _pattern[pos] == '{')//有子规则的情况
                    {
                        pos += 1;//pos 子规则起始位置
                        while(pos < _pattern.size() && _pattern[pos] != '}')
                        {
                            val.push_back(_pattern[pos++]);
                        }
                        if(_pattern.size() == pos)//没有找到'}'
                        {
                            std::cerr << "[ERROR]cpplogs::Formmatter::parsePattern::子格式{}匹配出错." << std::endl;
                            return false;
                        }
                        pos += 1;
                    }
                    fmt_order.push_back(std::make_pair(key, val));
                    key.clear();
                    val.clear();
                }
            }
            // 处理循环结束后剩余的val（原始字符串）
            if (!val.empty()) 
            {
                fmt_order.push_back(std::make_pair("", val));
            }
            //根据解析得到的数据初始化格式化子项数组成员
            for(auto& it : fmt_order)
            {
                _items.push_back(cpplogs::Formmatter::createItem(it.first, it.second));
            }
            return true;
        }

        //根据不同的格式化字符创建不同的格式化子项对象
        cpplogs::FormatItem::ptr createItem(const std::string& key, const std::string& val)
        {
            if(key == "d")
            {
                return std::make_shared<cpplogs::TimeFormatItem>(val);
            }
            else if(key == "t")
            {
                return std::make_shared<cpplogs::ThreadFormatItem>();
            }
            else if(key == "c")
            {
                return std::make_shared<cpplogs::LoggerFormatItem>();
            }
            else if(key == "f")
            {
                return std::make_shared<cpplogs::FileFormatItem>();
            }
            else if(key == "l")
            {
                return std::make_shared<cpplogs::LineFormatItem>();
            }
            else if(key == "p")
            {
                return std::make_shared<cpplogs::LevelFormatItem>();
            }
            else if(key == "T")
            {
                return std::make_shared<cpplogs::TabFormatItem>();
            }
            else if(key == "m")
            {
                return std::make_shared<cpplogs::MsgFormatItem>();
            }
            else if(key == "n")
            {
                return std::make_shared<cpplogs::NewLineFormatItem>();
            }
            else
            {
                if(!key.empty())
                {
                    std::cerr << "[WARN]cpplogs::Formmatter::createItem::没有对应的格式化字符: %" << key << "." << std::endl;
                }
                return std::make_shared<cpplogs::OtherFormatItem>(val);
            }
        }

    private:
        std::string _pattern;//格式化规则字符串
        std::vector<cpplogs::FormatItem::ptr> _items;
    };
}

#endif