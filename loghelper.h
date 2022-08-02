#include <string>
#include <fstream>
#include <cassert>
#include <ctime>

using namespace std;

/// <summary>
/// 日志类
/// </summary>
class Log 
{
private:
    /// <summary>
    /// 写入流
    /// </summary>
    ofstream _stream;

public:
    /// <summary>
    /// 日志写入方式
    /// </summary>
    enum Type 
    {
        /// <summary>
        /// 末尾添加
        /// </summary>
        ADD = 0,
        /// <summary>
        /// 覆盖
        /// </summary>
        OVER
    };
    /// <summary>
    /// 日志等级
    /// </summary>
    enum Level 
    {
        /// <summary>
        /// 调试
        /// </summary>
        DEBUG = 0,
        /// <summary>
        /// 信息
        /// </summary>
        INFO,
        /// <summary>
        /// 错误
        /// </summary>
        ERROR
    };

    /// <summary>
    /// 初始化日志类
    /// </summary>
    /// <param name="logfile">日志文件路径</param>
    /// <param name="type">日志添加模式</param>
    Log(const string& logfile, const int type = Log::ADD)
    {
        if (type == this->ADD)
            this->_stream.open(logfile.c_str(), ios_base::out | ios_base::app);
        else if (type == this->OVER)
            this->_stream.open(logfile.c_str(), ios_base::out | ios_base::trunc);

        assert(this->_stream.is_open() && "Log Create Failed!");
    }

    ~Log() 
    {
        if (this->_stream.is_open())
            this->_stream.close();
        return;
    }

    /// <summary>
    /// 写入日志
    /// </summary>
    /// <param name="level">级别</param>
    /// <param name="info">内容</param>
    void write(const int level, const string& info)
    {
        assert(this->_stream.is_open() && "Write Log Failed.");

        //获取时间
        time_t sectime = time(NULL);
        tm tmtime;
        localtime_s(&tmtime, &sectime);
        this->_stream << tmtime.tm_year + 1900 << '-' << tmtime.tm_mon + 1 << '-' << tmtime.tm_mday << ' ' << tmtime.tm_hour << ':' << tmtime.tm_min << ':' << tmtime.tm_sec << " [";

        //获取级别
        switch (level)
        {
        case this->ERROR:
            this->_stream << "ERROR";
            break;
        case this->INFO:
            this->_stream << "INFO";
            break;
        default:
            this->_stream << "DEBUG";
            break;
        }

        //输出内容
        this->_stream << "]: " << info << endl;
        return;
    }
};