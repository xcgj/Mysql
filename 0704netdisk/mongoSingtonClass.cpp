#include "mongoSingtonClass.h"

mongotool * mongotool::tool = new mongotool;

mongotool::mongotool()
{
    //指定使用的数据库和集合
    db = client["netdisk"];
    collection = db["file_system"];
}

mongotool::mongotool(const mongotool &)
{
}

mongotool::~mongotool()
{

}

mongotool *mongotool::getInstance()
{
    return tool;
}

int mongotool::insert(int user_file_id, char *real_path, char *md5)
{
    //先组建文档，document{}是流构建器
    bsoncxx::document::value doc_value = document{}<<"user_file_id"<<user_file_id<<"dst_path"<<real_path<<"md5"<<md5<<finalize;
    //再把文档插入集合，通过集合调用insert_one
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collate.insert_one((bsoncxx::document::value&&)doc_value);
    if(result)
    {
        cout<<"插入成功"<<endl;
        return 0;
    }
    else
    {
        cout<<"插入失败"<<endl;
        return -1;
    }
}

/*
int main()
{
    mongotool * mongo = mongotool::getInstance();
    int ret = mongo->insert(2, (string)"/root/a.txt", (string)"098765");
    if(ret)
        cout<<"插入失败"<<endl;
    else
        cout<<"插入成功"<<endl;
    return 0;
}
*/
