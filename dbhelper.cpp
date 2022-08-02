#include "reghelper.h"
#include "main.h"
#include "dbhelper.h"

constexpr auto DBPATH = "data.db";

bool createDB()
{
	//创建“data.db”文件
	sqlite3* db;
	int rtn = sqlite3_open(DBPATH, &db);

	//返回结果
	if (rtn != SQLITE_OK)
	{
		infostream << "无法创建或打开数据库文件" << DBPATH << "：" << sqlite3_errmsg(db);
		mylog.write(Log::ERROR, infostream.str());
		infostream.str("");
		return false;
	}
	sqlite3_close(db);
	return true;
}

bool executeSql(sqlite3** db, const string& str)
{
	//错误原因
	char* err;

	//生成SQL语句
	char* sql = new char[str.length() + 1];
	strcpy_s(sql, str.length() + 1, str.c_str());

	//执行SQL语句
	int rtn = sqlite3_exec(*db, sql, 0, 0, &err);
	delete[]sql;

	//返回结果
	if (rtn != SQLITE_OK)
	{
		infostream << "SQL语句错误：" << err;
		mylog.write(Log::ERROR, infostream.str());
		infostream.str("");
		sqlite3_free(err);
		return false;
	}
	return true;
}

int createTable(ifstream& filestream, const string& tablename, vector<int>& indexs)
{
	//获取CSV流的第一行，生成待插入表的字段向量
	string line;
	vector<string> columns;
	if (getline(filestream, line))
	{
		int index = -1;
		string column;
		istringstream stream(line);
		while (getline(stream, column, ';'))
		{
			index++;
			removeString(column, '\ufffd', "");
			//过滤可能重复的字段名
			if (find(columns.begin(), columns.end(), column) != columns.end())
				continue;
			columns.push_back(column);
			indexs.push_back(index);
		}
		stream.clear();
	}

	sqlite3* db;
	sqlite3_open(DBPATH, &db);

	//生成创建表SQL语句
	string sql = "CREATE TABLE IF NOT EXISTS " + tablename + "(REC_ID integer PRIMARY KEY autoincrement,";
	int i = 0;
	for (i; i < columns.size() - 1; i++)
		sql += "'" + columns[i] + "' varchar(64),";
	sql += "'" + columns[i] + "' varchar(64))";

	//执行SQL语句，获取返回值
	int rtn = 0;
	bool flag = executeSql(&db, sql);
	if (flag)
		rtn = columns.size();
	else
		rtn = -1;

	sqlite3_close(db);
	return rtn;
}

int getTableCount(sqlite3** db, const string& tablename)
{
	//生成查询该表行数的SQL语句
	string selectcount = "SELECT COUNT(*) FROM " + tablename;
	char* sql = new char[selectcount.length() + 1];
	strcpy_s(sql, selectcount.length() + 1, selectcount.c_str());

	int rowcount = 0;//返回的行数
	int rownum;//查询结果行数
	int colnum;//查询结果列数
	char** countresult;//二维表压成一维的字符串结果
	char* err;
	sqlite3_get_table(*db, sql, &countresult, &rownum, &colnum, &err);
	delete[] sql;

	//获取结果
	int index = colnum;//字符串结果游标
	for (int i = 0; i < rownum; i++)
	{
		for (int j = 0; j < colnum; j++)
		{
			rowcount = atoi(countresult[index]);
			++index;
		}
	}

	sqlite3_free_table(countresult);
	return rowcount;
}

int insertTable(ifstream& filestream, const string& tablename, const vector<int>& indexs)
{
	bool flag = true;
	char* err;
	sqlite3* db;
	sqlite3_open(DBPATH, &db);

	//获取上次读取到的行数
	int LastRow = getTableCount(&db, tablename);

	string line;
	int NowRow = 0;//当前所在行
	sqlite3_exec(db, "begin;", 0, 0, &err);//开启事务
	while (getline(filestream, line))
	{
		//差量更新
		NowRow++;
		if (NowRow <= LastRow)
			continue;

		//通过索引向量提取数据
		int index = -1;
		string obj;
		vector<string> objs;
		istringstream stream(line);
		while (getline(stream, obj, ';'))
		{
			index++;
			if (find(indexs.begin(), indexs.end(), index) != indexs.end())
			{
				//过滤非法字符
				removeString(obj, '\ufffd', "");
				removeString(obj, '\0', "");
				removeString(obj, ' ', "");
				objs.push_back(obj);
			}
		}
		stream.clear();

		//生成并执行SQL
		string sql = "INSERT INTO " + tablename + " VALUES(null,";
		int i = 0;
		for (i; i < objs.size() - 1; i++)
			sql += "'" + objs[i] + "',";
		sql += "'" + objs[i] + "')";

		flag = executeSql(&db, sql);
		if (!flag)
			break;
	}

	//获取更新的行数
	int num = 0;
	if (flag)
	{
		num = NowRow - LastRow;
		sqlite3_exec(db, "commit;", 0, 0, 0);//提交事务
	}
	else
	{
		num = -1;
		sqlite3_exec(db, "rollback;", 0, 0, 0);//回滚事务
	}

	sqlite3_close(db);
	return num;
}