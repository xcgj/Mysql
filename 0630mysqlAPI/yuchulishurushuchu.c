#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

// 业务：
//1 创建mysql句柄初始化stmt句柄
//2 stmt绑定sql语句，语句需带?，绑定一个bind用于传参查询
//3 stmt绑定新的bind用于输出结果
//4 bind绑定字段变量，接收结果集的值
//5 mysql_stmt_bind_result()绑定stmt和bind
//6 mysql_stmt_execute()产生结果集
//7 mysql_stmt_store_result()将结果集保存到本地客户端缓冲区
//8 mysql_stmt_fetch()提取缓冲区信息

int main()
{
	//创建mysql
	MYSQL *mysql = mysql_init(NULL);
	//连接服务器
	mysql = mysql_real_connect(mysql, "192.168.87.130", "root", "xcgj", "test", 3306, NULL, 0);
	if (NULL == mysql)
	{
		printf("connect error\n");
		return -1;
	}
	//设置字符集
	mysql_query(mysql, "set names utf8");

	//创建stmt句柄
	MYSQL_STMT *stmt = mysql_stmt_init(mysql);

#if 0	//输入
	//sql语句
	char *sql = "insert into tes values(?,?,?,?)";
	//绑定sql语句
	mysql_stmt_prepare(stmt, sql, strlen(sql));
	
	//band函数绑定参数
	MYSQL_BIND bind[3];

	int id = 10;
	char name[1024]={0};
	unsigned long name_length;
	int score = 20;
	
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = (void *)&id;
	bind[0].buffer_length = sizeof(id);
	bind[0].is_null = (my_bool *)0;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (void *)name;
	bind[1].buffer_length = strlen(name);
	bind[1].is_null = (my_bool *)0;
	bind[1].length = &name_length; 
	
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = (void *)&score;
	bind[2].buffer_length = sizeof(score);
	bind[2].is_null = (my_bool *)0;
	
	//将bind和stmt绑定传参
	mysql_stmt_bind_param(stmt, bind);

	for(int i=0;i<3;++i)
	{
		id=i+20;
	 	strcpy(name,"aa");
	 	score = i+30;
	 	name_length = (unsigned long)strlen(name);
	 	if(mysql_stmt_execute(stmt))
	 	{
	 		printf("execute error\n");
	 		return -1;
		}
	}
#else	//输出
	//sql语句绑定，必须有？传参
	char *sql = "select * from tes where id = ?";
	mysql_stmt_prepare(stmt, sql, strlen(sql));

	//？输入绑定，指定参数查询
	int condition = 3;
	MYSQL_BIND bind_condition;
	bind_condition.buffer_type = MYSQL_TYPE_LONG;
	bind_condition.buffer = (void *)&condition;
	bind_condition.buffer_length = sizeof(condition);
	bind_condition.is_null = (my_bool *)0;
	mysql_stmt_bind_param(stmt,&bind_condition);

	//保存输出结果的参数
	int id;
	char name[1024];
	int score;

	//参数绑定bind，只需要参数类型和长度
	MYSQL_BIND bind[3];
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = (void *)&id;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (void *)name;
	bind[1].buffer_length = strlen(name);

	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = (void *)&score;
	
	//绑定stmt和bind数组
	mysql_stmt_bind_result(stmt, bind);
	
	//获得结果集，存储到stmt， 数据在服务器的缓冲区里
	mysql_stmt_execute(stmt);

	//将服务器段的数据存到本地客户端缓冲区
	mysql_stmt_store_result(stmt);

	//从缓冲区提取数据
	while(!mysql_stmt_fetch(stmt))
	{
		printf("%d\t",id);
		printf("%s\t",name);
		printf("%d\t",score);
		printf("\n");
	}
	//释放结果集
	mysql_stmt_free_result(stmt);
#endif
	mysql_stmt_close(stmt);
	mysql_close(mysql);
	return 0;
}
