#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "dbhelper.h"
#include "reghelper.h"
#include "loghelper.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //设置入口地址

using namespace std;

//日志类和写入信息流
static Log mylog("hmsqlite.log", Log::OVER);
static ostringstream infostream;