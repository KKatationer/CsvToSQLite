//
// 2022-06-29
//
// 修改字符串的头文件
//

#include <string>
#include <regex>

using namespace std;

/// <summary>
/// 生成表名
/// </summary>
/// <param name="filename">CSV文件名</param>
/// <returns>表名</returns>
string getTableName(string filename);

/// <summary>
/// 替换某字符串中的全部字符
/// </summary>
/// <param name="src">原字符串</param>
/// <param name="item">待删除的字符</param>
/// <param name="news">替换的字符</param>
void removeString(string& src, char item, const string& news);