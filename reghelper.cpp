#include "reghelper.h"

string getTableName(string file)
{
	int firstpos = file.find_last_of('\\') + 1;
	int length = file.size() - firstpos;
	string name = file.substr(firstpos, length);
	name = "'" + name + "'";
	return name;
}

void removeString(string& src, char item, const string& news)
{
	int i = src.find(item);
	while (i != -1)
	{
		src = src.replace(i, 1, news);
		i = src.find(item);
	}
}