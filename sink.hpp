#ifndef __LOGS_SINK_H__
#define __LOGS_SINK_H__

/*
 * sink.hpp 日志落地模块类
 * 1. 抽象落地基类
 * 2. 派生子类（根据不同的落地方向进行派生）
 * 3. 使用工厂模式进行创建与表示的分离
*/

#include "util.hpp"
#include <fstream>
#include <memory>
#include <cassert>
#include <sstream>

namespace cpplogs
{
    class LogSink
    {
    public:
        using ptr = std::shared_ptr<LogSink>;
        virtual ~LogSink() {};
        virtual void log(const char* data, size_t len) = 0;
    };

    //落地方向: 标准输出
    class StdoutSink : public LogSink
    {
    public:
        //将日志消息写入到标准输出
        void log(const char* data, size_t len)
        {
            std::cout.write(data, len);
        }
    };
    //落地方向: 指定文件
    class FileSink : public LogSink
    {
    public:
        //构造时传入文件名，打开文件，将文件句柄管理起来
        FileSink(const std::string& pathname)
        : _pathname(pathname)
        {
            //创建日志文件所在的目录
            cpplogs::util::File::createDirectory(cpplogs::util::File::path(_pathname));
            //创建并打开日志文件
            _ofs.open(_pathname, std::ios::binary | std::ios::app);//写入和追加
            assert(_ofs.is_open());
        }
        //将日志消息写入到指定文件
        void log(const char* data, size_t len)
        {
            _ofs.write(data, len);
            assert(_ofs.good());
        }
    private:
        const std::string _pathname;
        std::ofstream _ofs;
    };
    //落地方向: 滚动文件(大小)
    class RollSinkBySize : public LogSink
    {
    public:
        //构造时传入文件名，打开文件，将文件句柄管理起来
        RollSinkBySize(const std::string& basename, size_t max_size)
        : _basename(basename)
        , _max_fsize(max_size)
        , _cur_fsize(0)
        , _name_count(0)
        {
            std::string pathname = createNewFile();
            //创建日志文件所在的目录
            cpplogs::util::File::createDirectory(cpplogs::util::File::path(pathname));
            //创建并打开日志文件
            _ofs.open(pathname, std::ios::binary | std::ios::app);//写入和追加
            assert(_ofs.is_open());
        }

        //将日志消息写入到指定文件
        void log(const char* data, size_t len)
        {
            if(_cur_fsize >= _max_fsize)//进行大小判断，超过最大大小创建新文件
            {
                std::string pathname = createNewFile();
                _ofs.close();//关闭原来打开的文件
                _cur_fsize = 0;
                _ofs.open(pathname, std::ios::binary | std::ios::app);//写入和追加
                assert(_ofs.is_open());
            }
            _ofs.write(data, len);
            _cur_fsize += len;
            assert(_ofs.good());
        }

    private:
        std::string createNewFile()
        {
            //获取系统时间，以时间来构造文件名拓展名
            time_t cur_time = cpplogs::util::Date::getTime();
            struct tm st;
            localtime_r(&cur_time, &st);
            std::stringstream filename;
            filename << _basename;
            filename << st.tm_year + 1900;
            filename << st.tm_mon + 1;
            filename << st.tm_mday;
            filename << st.tm_hour;
            filename << st.tm_min;
            filename << st.tm_sec;
            filename << ++_name_count;
            filename << ".log";
            return filename.str();
        }
    private:
        //通过基础文件名+扩展文件名（以时间生成）组成一个实际的当前输出文件名
        std::string _basename;//基础文件名
        std::ofstream _ofs;
        size_t _max_fsize;//记录最大大小，当前文件写入大小超过了这个大小就要切换文件
        size_t _cur_fsize;//记录当前文件已经写入的数据大小
        size_t _name_count;//名称计数器
    };

    enum class TimeGap
    {
        GAP_SECOND,
        GAP_MINUTE,
        GAP_HOUR,
        GAP_DAY
    };

    //落地方向: 滚动文件(时间)
    class RollSinkByTime : public LogSink
    {
    public:
        //构造时传入文件名，打开文件，将文件句柄管理起来
        RollSinkByTime(const std::string& basename, cpplogs::TimeGap gap_type)
        : _basename(basename)
        {
            switch (gap_type)
            {
            case cpplogs::TimeGap::GAP_SECOND:
                _gap_size = 1;
                break;
            
            case cpplogs::TimeGap::GAP_MINUTE:
                _gap_size = 60;
                break;
            
            case cpplogs::TimeGap::GAP_HOUR:
                _gap_size = 3600;
                break;

            case cpplogs::TimeGap::GAP_DAY:
                _gap_size = 24 * 3600;
                break;
            
            default:
                break;
            }

            _cur_gap = cpplogs::util::Date::getTime()  % _gap_size;//当前是第几个时间段

            std::string pathname = createNewFile();
            cpplogs::util::File::createDirectory(cpplogs::util::File::path(pathname));
            _ofs.open(pathname, std::ios::binary | std::ios::app);//写入和追加
            assert(_ofs.is_open());
        }

        //将日志消息写入到指定文件，进行时间判断，超过最大时间间隔则创建新文件
        void log(const char* data, size_t len)
        {
            time_t cur = cpplogs::util::Date::getTime();
            if(cur % _gap_size > _cur_gap)
            {
                std::string pathname = createNewFile();
                _ofs.close();//关闭原来打开的文件
                _ofs.open(pathname, std::ios::binary | std::ios::app);//写入和追加
                assert(_ofs.is_open());
            }
            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        std::string createNewFile()
        {
            //获取系统时间，以时间来构造文件名拓展名
            time_t cur_time = cpplogs::util::Date::getTime();
            struct tm st;
            localtime_r(&cur_time, &st);
            std::stringstream filename;
            filename << _basename;
            filename << st.tm_year + 1900;
            filename << st.tm_mon + 1;
            filename << st.tm_mday;
            filename << st.tm_hour;
            filename << st.tm_min;
            filename << st.tm_sec;
            filename << ".log";
            return filename.str();
        }
    private:
        //通过基础文件名+扩展文件名（以时间生成）组成一个实际的当前输出文件名
        std::string _basename;//基础文件名
        std::ofstream _ofs;
        size_t _cur_gap; //当前是第几个时间段
        size_t _gap_size; //时间段的大小
    };

    //简单工厂模式 - C++不定参宏函数
    class SinkFactory
    {
    public:
        template<typename SinkType, typename ...Args>
        static cpplogs::LogSink::ptr create(Args&& ...args)
        {
            return std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };
}

#endif