#ifndef __LOGS_UTIL_H__
#define __LOGS_UTIL_H__

/*
 * util.hpp 实用工具类的实现
 * 1. 获取系统时间
 * 2. 判断文件是否存在
 * 3. 获取文件所在路径
 * 4. 创建目录
*/

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>

namespace cpplogs
{
    namespace util
    {
        class Date
        {
        public:
            //获取时间
            static time_t getTime()
            {
                return time(nullptr);
            }
        };

        class File
        {
        public:
            //文件是否存在
            static bool exists(const std::string& pathname)
            {
                //return (access(pathname.c_str(), F_OK) == 0);//Linux
                struct stat st;
                if(stat(pathname.c_str(), &st) < 0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }

            //获取文件目录
            static std::string path(const std::string& pathname)
            {
                size_t pos = pathname.find_last_of("/\\");
                if(pos == std::string::npos)
                {
                    return ".";
                }
                else
                {
                    return pathname.substr(0, pos + 1);
                }
            }

            //创建目录
            static void createDirectory(const std::string& pathname)
            {
                size_t pos = 0;
                size_t idx = 0;
                while(idx < pathname.size())
                {
                    pos = pathname.find_first_of("/\\", idx);// 查找路径分隔符
                    if(pos == std::string::npos)//没有找到更多分隔符
                    {
                        mkdir(pathname.c_str(), 0777);//创建完整路径
                    }
                    else
                    {
                        std::string parent_dir = pathname.substr(0, pos);//截取父目录
                        if(exists(parent_dir) == true)//检查目录是否存在,存在则跳过
                        {
                            idx = pos + 1;
                            continue;
                        }
                        else
                        {
                            mkdir(parent_dir.c_str(), 0777);
                            idx = pos + 1;
                        }
                    }
                }
            }
        };
    }
}

#endif