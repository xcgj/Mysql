#include "mysqlSingletonClass.h"

//饿汉式初始化单例指针
mysqltool * mysqltool::instance = new mysqltool;

//构造，让用户连接mysql数据库
mysqltool::mysqltool()
{
    //初始化mysql句柄
    mysql = mysql_init(NULL);
    //连接本地mysqld
    mysql_real_connect(mysql, "127.0.0.1", "root", "xcgj", "netdisk", 3306, NULL, 0);
    if(NULL == mysql)
    {
        cout << "connect error" << endl;
    }
    //防止乱码
    mysql_query(mysql, "set names utf8");
}

mysqltool::mysqltool(const mysqltool &)
{
}

//析构函数
mysqltool::~mysqltool()
{
    //关闭和mysql数据库的连接
    mysql_close(mysql);
}

//获取实例
mysqltool *mysqltool::getInstance()
{
    return instance;
}

//验证登录用户名密码
//mysql命令执行成功，但结果集可能是空的，提取id检验
int mysqltool::select(char *sql, int & id)
{
    //执行查询语句
    mysql_query(mysql, sql);//在表中查到数据返回0
    //获取上一部产生的结果集
    MYSQL_RES * result = mysql_store_result(mysql);
    //判断结果集里的行
    MYSQL_ROW row = mysql_fetch_row(result);
    //提取id
    if(row)
        id = atoi(row[0]);
    return 0;
}

//list 指令
int mysqltool::select(char *sql, char * result_buf)
{
    //执行查询语句
    if(mysql_query(mysql, sql))
        return -1;
    //获取上一部产生的结果集
    MYSQL_RES * result = mysql_store_result(mysql);
    //获取行
    MYSQL_ROW row;
    int i = 0;
    int count = mysql_num_fields(result);//结果集中的字段数量
    while(row = mysql_fetch_row(result))//遍历结果集中的所有记录
    {
        for(i=0; i<count; ++i)  //将记录的每个字段值换行写入缓冲
            sprintf(result_buf, "%s%s\n", result_buf, row[i]);
    }
    return 0;
}

//插入
int mysqltool::insert(char *sql)
{
    return mysql_query(mysql, sql);
}
