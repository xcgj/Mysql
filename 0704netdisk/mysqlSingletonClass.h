#ifndef _MYSQLTOOL_H_
#define _MYSQLTOOL_H_
#include <iostream>
#include <stdio.h>
#include <mysql/mysql.h>
using namespace std;

//mysql的单例类

class mysqltool
{
private:
    mysqltool();
    mysqltool(const mysqltool&);
    ~mysqltool();

    static mysqltool * instance;
    MYSQL * mysql;//mysql句柄

public:
    static mysqltool * getInstance();

    //验证登录用户名密码
    int select(char *sql, int & id);
    //list 指令
    int select(char * sql, char * result_buf);
    //插入数据
    int insert(char * sql);
};

#endif
