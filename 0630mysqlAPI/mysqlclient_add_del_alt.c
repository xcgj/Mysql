#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
	int ret = 0;
	MYSQL * mysql = mysql_init(NULL);

	mysql = mysql_real_connect(mysql, "192.168.87.130", "root", "xcgj", "test",
			3306, NULL, 0);
	if(mysql == NULL)
	{
		printf("errno -1\n");
		return ret;
	}
	mysql_query(mysql, "set names utf8");

	char * sqlcmd = "insert into test values(8, '阿斯顿', 89)";

	ret = mysql_query(mysql, sqlcmd);
	if(ret)
	{
		printf("errno -2\n");
		return ret;
	}
	mysql_close(mysql);
	return 0;
}
