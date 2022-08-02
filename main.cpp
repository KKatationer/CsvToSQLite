//
// 2022-06-28
//
//****** 主函数说明 ******
// 传入参数仅限于csv文件路径，csv文件分隔符为”;“。
//
// 程序无输出，只有返回值，代码如下：
// 1000：运行成功；
// 0：无传入参数
// -1：生成本地数据库文件失败（空间不足，权限不足）；
// -2：CSV读取失败（文件是否存在，权限不足）；
// -3：生成表字段名称失败（CSV第一行可能存在逻辑错误）；
// -4：插入数据失败（可能存在非法内容，或者程序Bug）；
// 其它：程序异常（运行库）。
//

#include "main.h"

int main(int argc, char *argv[])
{
	if (argc == 0)
		return 0;

	//创建本地数据文件
	if (!createDB())
		return -1;

	//根据传入的参数获取csv文件路径
	for (int i = 1; i < argc; i++)
	{
		//打开csv文件，创建流指针
		string file = argv[i];
		ifstream filestream(file, ios::in);
		if (!filestream.is_open())
		{
			infostream << "无法打开csv文件：" << file;
			mylog.write(Log::ERROR, infostream.str());
			infostream.str("");
			return -2;
		}
		
		//生成数据表
		string tableName = getTableName(file);
		vector<int> indexs;
		int colCount = createTable(filestream, tableName, indexs);
		if (colCount >= 0)
		{
			infostream << file << " 已生成字段数：" << colCount;
			mylog.write(Log::INFO, infostream.str());
			infostream.str("");
		}
		else
			return -3;

		//插入数据
		int lineCount = insertTable(filestream, tableName, indexs);
		if (lineCount >= 0)
		{
			infostream << file << " 已插入行数：" << lineCount;
			mylog.write(Log::INFO, infostream.str());
			infostream.str("");
		}
		else
			return -4;

		//关闭流指针
		filestream.close();
	}

	return 1000;
}
