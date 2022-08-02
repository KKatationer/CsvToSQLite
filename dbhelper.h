//
// 2022-06-29
//
// SQLite实现的头文件
//

#include <string>
#include <vector>
#include <fstream>
#include "sqlite3.h"

using namespace std;

/// <summary>
/// 创建本地SQLite数据库：data.db文件
/// </summary>
/// <returns>是否成功</returns>
bool createDB();

/// <summary>
/// 执行SQL语句
/// </summary>
/// <param name="db">sqlite3实例</param>
/// <param name="s">sql语句</param>
/// <returns>执行是否成功</returns>
bool executeSql(sqlite3** db, const string& s);

/// <summary>
/// 创建数据表
/// </summary>
/// <param name="filestream">文本流</param>
/// <param name="tablename">表名</param>
/// <param name="indexs">索引向量</param>
/// <returns>是否成功</returns>
int createTable(ifstream& filestream, const string& tablename, vector<int>& indexs);

/// <summary>
/// 读取已有行数
/// </summary>
/// <param name="db">sqlite3实例</param>
/// <param name="tablename">表名</param>
/// <returns>该表已有行数</returns>
int getTableCount(sqlite3** db, const string& tablename);

/// <summary>
/// 插入数据
/// </summary>
/// <param name="filestream">文本流</param>
/// <param name="tablename">表名</param>
/// <param name="indexs">索引向量</param>
/// <returns>插入的行数</returns>
int insertTable(ifstream& filestream, const string& tablename, const vector<int>& indexs);