#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

int main()
{
	//创建mysql对象
	MYSQL * mysql = mysql_init(NULL);
	//连接mysql
	mysql = mysql_real_connect(mysql, "192.168.87.130", "root", "xcgj", "test", 3306, NULL, 0);
	if(NULL == mysql)
	{
		printf("connect err\n");
		return -1;
	}
	//设置字符集
	mysql_query(mysql, "set names utf8");

	//循环接收命令
	while(1)
	{
		printf("mysql > ");
		char sql[1024] = {0};
		fgets(sql, 1024, stdin);

		if(0==strncmp("insert", sql, 6) ||
				0==strncmp("update", sql, 6) ||
				0==strncmp("delete", sql, 6))
		{
			//mysql_query()
			if(mysql_query(mysql, sql))
			{
				printf("cmd err\n");
				continue;
			}
			//打印受影响的行
			int row = mysql_affected_rows(mysql);
			printf("query OK, %d row affected\n", row);
		}
		else if(0==strncmp("select", sql, 6))
		{
			//获取表的信息
			if(mysql_query(mysql, sql))
			{
				printf("query error\n");
				continue;
			}

			//检索结果集
			MYSQL_RES * res = mysql_store_result(mysql);
			if(res == NULL)
			{
				printf("res err\n");
				return 0;
			}

			//获取结果集中的字段数
			unsigned int count = mysql_num_fields(res);
			printf("count = %d\n", count);

			//获取表头字段名
			int i=0;
#if 0
			for(i=0; i<count; ++i)
			{
				MYSQL_FIELD * field = mysql_fetch_field(res);
				printf("%s\t", field->name);
			}
			printf("\n");
#else
			MYSQL_FIELD * field = mysql_fetch_fields(res);
			for(i=0; i<count; ++i)
			{
				printf("%s\t", field[i].name);
			}
			printf("\n");
#endif
			//获取结果集中的数据
			MYSQL_ROW row;
			while(row = mysql_fetch_row(res))
			{
				for(i=0; i<count; ++i)
				{
					printf("%s\t", row[i]);
				}
				printf("\n");
			}
			int rows = mysql_num_rows(res);
			printf("%d row affected\n", rows);

			//释放结果集
			mysql_free_result(res);
		}
		else if(0==strncmp("exit", sql, 4))
		{
			break;
		}
	}
	mysql_close(mysql);
	return 0;
}

