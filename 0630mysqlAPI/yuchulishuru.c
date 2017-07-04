#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

//业务：
//1 建立mysql对象初始化预处理对象stmt
//2 指定stmt解析的sql语句的格式
//3 stmt不能直接传参数，需要利用bind函数绑定
//4 bind函数绑定sql语句中的字段变量
//5 stmt和bind绑定后，用execute执行

int main()
{
	//建立对象
	MYSQL * mysql = mysql_init(NULL);
	//==建立连接
	mysql = mysql_real_connect(mysql, "192.168.87.130", "root", "xcgj", "test", 3306, NULL, 0);
	if(NULL == mysql)
	{
		printf("err -1\n");
		return -1;
	}
	//==设置字符集
	mysql_query(mysql, "set names utf8");

	//用mysql初始化预处理结构体
	MYSQL_STMT * stmt = mysql_stmt_init(mysql);
	//==stmt绑定sql语句格式，解析一次后，以后都直接传值
	char * sqlcmd = "insert into tes values(?,?,?)";
	mysql_stmt_prepare(stmt, sqlcmd, strlen(sqlcmd));
	//参数绑定
	MYSQL_BIND bind[3];
	//==创建变量准备绑定给给bind函数，用来传数据
	int id = 10;
	char name[1024] = {0};
	unsigned long name_length;
	int score = 10;
	//==绑定数据，这里是边绑定边插入数据了，可以将绑定从循环中优化出来
	int i=0;
	for(i=0; i<3; ++i)
	{
		id += 2;
		strcpy(name, "abcd");
		name_length = (unsigned long)strlen(name);
		score += 20;
		//利用buffer_type和buffer绑定变量
		bind[0].buffer_type = MYSQL_TYPE_LONG;	//指定类型
		bind[0].buffer = (void *)&id;		//绑定变量
		bind[0].buffer_length = sizeof(id);	//存变量大小
		bind[0].is_null = (my_bool *)0;		//空 属性

		bind[1].buffer_type = MYSQL_TYPE_STRING;
		bind[1].buffer = (void *)name;
		bind[1].buffer_length = sizeof(name);//*buffer的总大小，单位为字节
		bind[1].is_null = (my_bool *)0;
		bind[1].length = &name_length;//指针，给空间，指明了存储在*buffer中数据的实际字节数

		bind[2].buffer_type = MYSQL_TYPE_LONG;
		bind[2].buffer = (void*)&score;
		bind[2].buffer_length = sizeof(score);
		bind[2].is_null = (my_bool *)0;

		//以上步骤将3个字段变量和bind函数绑定了
		//现在需要将bind变量数组和stmt句柄建立关系
		//参数输入用mysql_stmt_bind_param()  mysql_stmt_execute()
		//获得输出参数用mysql_stmt_bind_result()  with mysql_stmt_fetch()
		mysql_stmt_bind_param(stmt, bind);
		if(mysql_stmt_execute(stmt))
		{
			printf("execute err\n");
			return -1;
		}
	}

	mysql_stmt_close(stmt);
	mysql_close(mysql);
	return 0;

}
