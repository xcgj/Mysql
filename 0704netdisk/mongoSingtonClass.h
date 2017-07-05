#ifndef _MONGOTOOL_H
#define _MONGOTOOL_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

//mongo的单例类

class mongotool
{
private:
    mongotool();
    mongotool(const mongotool&);
    ~mongotool();

    static mongotool * tool;
    mongocxx::instance instance{};//mongo实例
    mongocxx::client client{mongocxx::uri{}};//客户端连接
    mongocxx::database db;//数据库
    mongocxx::collection collection;//集合

public:
    static mongotool * getInstance();
    //插入文档函数
    int insert(int user_file_id, char * real_path, char * md5);
};

#endif
