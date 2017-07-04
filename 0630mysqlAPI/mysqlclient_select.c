#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
	int ret = 0;
	MYSQL * mysql = mysql_init(NULL);

	mysql = mysql_real_connect(mysql, "192.168.87.130", "root", "xcgj", "test", 3306, NULL, CLIENT_MULTI_STATEMENTS);
	if(mysql == NULL)
	{
		printf("errno -1\n");
		return ret;
	}
	mysql_query(mysql, "set names utf8");

	ret = mysql_query(mysql, "insert into tes values(9, '风格的', 88);insert into tes values(10, '我空间', 85)");
	if(ret){printf("errno -2\n");return ret;}
	
	mysql_query(mysql, "commit");
	
	//执行查询语句
	ret = mysql_query(mysql, "select * from tes");
	printf("select\n");

	//检索结果集
	MYSQL_RES * res = mysql_store_result(mysql);
	if(res == NULL)
	{
		printf("res = NULL\n");
	}

	//获取结果集中的字段数
	unsigned int count = mysql_num_fields(res);
	printf("count = %d\n", count);
	//获取表头字段名
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		MYSQL_FIELD * field = mysql_fetch_field(res);
		printf("%s\t", field->name);
	}
	printf("\n");
	
	//获取结果集中行的数据
	MYSQL_ROW row = NULL;
	while(row = mysql_fetch_row(res))
	{
		for(i=0; i<count; ++i)
		{
			printf("%s\t", row[i]);
		}
		printf("\n");
	}
	
	//释放结果集
	mysql_free_result(res);
	//关闭mysql连接
	mysql_close(mysql);
	return 0;
}
