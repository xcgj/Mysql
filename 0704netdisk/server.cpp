#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "mysqlSingletonClass.h"
#include <iomanip>
#include <sys/wait.h>


//mysql服务器
//g++ mysqlSingletonClass.cpp server.cpp -o server -I/usr/include/mysql -lmysqlclient


void chld_handle(int sig)
{
    while(1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);// 每次回收都应该用非阻塞方式去回收
        cout << "子进程已回收" << ret << endl;
        if(ret <= 0)
        {
            //cout << "退出" << endl;
            break;
        }
    }
}

int main()
{
    signal(SIGCHLD, chld_handle);

    //初始化服务器socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(9998);
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 10);

    //获取mysqltool实例
    mysqltool * mysql = mysqltool::getInstance();//构造完成了连接mysql
    if(mysql == NULL)
    {
        cout << "mysql == NULL" << endl;
    }

    /*
    pid_t pid = fork(); //问题代码，待检验 建议：signal(SIGCHLD, chld_handle)
    if(pid == 0)
    {
        while(1)
        {
            wait(NULL);
        }
    }
    */

    while(1)
    {
        //接收客户端连接，产生子进程处理
        int newserverfd = accept(sock, NULL, NULL);
        pid_t pidt = fork();
        if(pidt == 0)
        {
            // 客户端发过来的报文格式：
            // login a1 111111
            char recv_buf[1024] = {0};  //用于接收客户端消息
            char send_buf[1024] = {0};  //用于给客户端发消息
            char sql[1024] = {0};       //用于构建sql命令
            char result_buf[1024] = {0};//用于查询命令返回结果集
            char username[1024] = {0};  //保存客户端用户名
            int userid = 0;             //用户的在数据库的专属id

            while(1)
            {
                memset(recv_buf, 0, sizeof(recv_buf));
                memset(send_buf, 0, sizeof(send_buf));
                memset(sql, 0, sizeof(sql));
                memset(result_buf, 0, sizeof(result_buf));
                //接收用户登录信息
                recv(newserverfd, recv_buf, sizeof(recv_buf), 0);
                //截取报文,获取用户操作类型
                char * type = strtok(recv_buf, " ");
                cout << "用户" << userid << "选择的操作类型是：" << type << endl;
                //解析报文
                int ret = 0;
                if(strcmp(type, "login") == 0)  //验证登录
                {
                    strcpy(username, strtok(NULL, " "));
                    char * password = strtok(NULL, " ");//cout << username << " " << password << endl;
                    //mysql命令若执行成功，但结果集可能是空的，提取id检验
                    sprintf(sql, "select id from user where username = '%s' and password = '%s'", username, password);//cout << sql << endl;
                    ret = mysql->select(sql, userid);
                    if(ret) //myslq命令执行失败
                    {
                        strcpy(send_buf, "login fail");
                    }
                    else    //mysql命令执行成功
                    {
                        if(userid)
                            strcpy(send_buf, "login success");
                        else
                            strcpy(send_buf, "login failed");
                    }
                    send(newserverfd, send_buf, strlen(send_buf), 0);
                    cout << send_buf << endl;
                }
                else if(strcmp(type, "regist") == 0)  //注册
                {
                    //验证用户名密码格式
                    //在user表中插入数据
                    //返回登录成功信息
                }
                else if(strcmp(type, "list") == 0)//查看网盘文件
                {
                    sprintf(sql, "select src_path from user_file where userid = (select id from user where username = '%s')", username);//cout << sql << endl;
                    ret = mysql->select(sql, result_buf);
                    if(ret)  //sql语句执行失败
                    {
                        strcpy(send_buf, "list failed");
                    }//cout << "debug" << endl;
                    else
                    {
                        if(strlen(result_buf) == 0) //网盘文件为空
                            strcpy(send_buf, "empty table\n");
                        else
                            memcpy(send_buf, result_buf, sizeof(result_buf));   //有文件记录
                    }
                    send(newserverfd, send_buf, strlen(send_buf), 0);
                }
                else if(strcmp(type, "upload") == 0)    //传文件
                {
                    //提取报文中的信息:upload 文件名 网盘显示路径 文件大小 md5
                    char * file_name = strtok(NULL, " ");
                    char * src_path = strtok(NULL, " ");
                    int file_size = atoi(strtok(NULL, " "));
                    char * md5 = strtok(NULL, " ");//cout << "in upload" << endl;

                    //mysql语句执行，检查服务器有没有存储相同md5的文件，找到服务器路径
                    sprintf(sql, "select dst_path from user_file where md5 = '%s'", md5);//cout << "sql:" << sql << endl;
                    //检查文件表中的md5是否存在
                    ret = mysql->select(sql, result_buf);//cout << "ret:" << ret << endl;
                    if(ret) //执行失败
                    {
                        //服务器内部错误，不用发给客户端
                        cout << "upload select error" << endl;
                        continue;
                    }

                    //sql语句执行成功，检查result_buf
                    //cout << "in sql语句执行成功，检查result_buf" << endl;;
                    ret = strlen(result_buf);
                    if(ret) //有文件记录
                    {
                        //用result_buf暂时保存文件路径
                        result_buf[ret - 1] = '\0';
                        //在user_file表中插入信息，就是给文件增加拥有者和新的网盘路径
                        memset(sql, 0, sizeof(sql));
                        sprintf(sql, "insert into user_file values(NULL,%d,'%s','%s','%s',NULL,%d,'%s')", userid, file_name, src_path, result_buf, file_size, md5);//cout << "sql: " << sql << endl;
                        memset(send_buf, 0, sizeof(send_buf));
                        if(mysql->insert(sql))  //sql语句执行失败
                            strcpy(send_buf, "save failed");
                        else    //插入成功
                            strcpy(send_buf, "save success");
                    }
                    else    //么有文件记录，需要通知客户端上传
                    {//cout << "么有文件记录，需要通知客户端上传" << endl;
                        //发送接受文件信息
                        strcpy(send_buf, "upload file");
                        send(newserverfd, send_buf, strlen(send_buf), 0);
                        //创建文件, 准备接受文件
                        memset(result_buf, 0, sizeof(result_buf));
                        sprintf(result_buf, "/home/xcgj/ServerFile/%s_%s", username, file_name);//cout << "result_buf: " << result_buf << endl;
                        int fd = open(result_buf, O_WRONLY | O_CREAT, 0777);//cout << "fd: " << fd << endl;
                        if(fd < 0)
                        {
                            cout << "服务器文件创建失败" << endl;
                            continue;
                        }
                        //接收文件,newserverfd接收文件并写入本地
                        int fsize = file_size;//cout << "fsize: " << fsize << endl;
                        int loop = 1;
                        cout << "文件大小: " << file_size << endl;
                        while(fsize)
                        {
                            char buffer[1024] = {0};
                            memset(buffer, 0, sizeof(buffer));
                            ret = recv(newserverfd, buffer, sizeof(buffer), 0);
                            cout <<"第" << loop << "次循环，管道读取" << ret << "字节"<< endl;
                            int res = write(fd, buffer, ret);//cout << buffer << endl;
                            cout <<"第" << loop++ << "次循环，文件写入" << res << "字节"<< endl;
                            fsize -= ret;
                            cout << "未接收文件大小: " << fsize << endl;
                        }
                        close(fd);
                        //验证md5信息...
                        //在user_file表中插入信息，就是给文件增加拥有者和新的网盘路径
                        memset(sql, 0, sizeof(sql));
                        sprintf(sql, "insert into user_file values(NULL,%d,'%s','%s','%s',NULL,%d,'%s')", userid, file_name, src_path, result_buf, file_size, md5);//cout << "sql: " << sql << endl;
                        memset(send_buf, 0, sizeof(send_buf));
                        if(mysql->insert(sql))  //sql语句执行失败
                            strcpy(send_buf, "upload failed");
                        else    //插入成功
                            strcpy(send_buf, "upload success");
                    }
                    send(newserverfd, send_buf, strlen(send_buf), 0);
                    cout << send_buf << endl;
                }
                else if(strcmp(type, "download") == 0)  //下载文件
                {
                    //提取报文中的信息：download 网盘路径
                    char * src_path = strtok(NULL, " ");//cout << "debug 1: " << src_path << endl;
                    //mysql语句执行，如果出错，给客户端发送文件长度为0
                    sprintf(sql, "select file_size, dst_path, md5 from user_file where userid = %d and src_path = '%s'", userid, src_path);
                    ret = mysql->select(sql, result_buf);//cout << "debug 2: " << sql << endl;//cout << "debug 3: " << ret << endl;//cout << "debug 3.1: " << result_buf << endl;
                    if(ret)
                    {
                        int emptyres = 0;//sql执行失败，返回0字节给客户端
                        send(newserverfd, &emptyres, sizeof(emptyres), 0);//必须指定长度，防止粘包//cout << "debug 4: " << badexec << endl;
                        continue;
                    }
                    //根据结果集截取文件信息
                    int file_size = atoi(strtok(result_buf, "\n"));
                    char * dst_path = strtok(NULL, "\n");
                    char * md5_net = strtok(NULL, "\n");//cout << "debug 5: " << file_size << endl;//cout << "debug 6: " << dst_path << endl;//cout << "debug 7: " << md5_net << endl;
                    //newserverfd发送文件长度
                    send(newserverfd, &file_size, sizeof(file_size), 0);//必须指定长度，防止粘包
                    //打开文件再发送文件内容
                    int fd = open(dst_path, O_RDONLY);//cout << "debug 8: " << fd << endl;
                    if(fd < 0)
                    {
                        cout << "文件创建失败" << endl;
                        continue;
                    }
                    int fsize = file_size;
                    cout << "文件大小: " << file_size << endl;
                    int loop = 1;
                    while(fsize > 0)
                    {//cout << "进入循环" << endl;
                        //memset(send_buf, 0, sizeof(send_buf));
                        char buf[1024] = {0};
                        ret = read(fd, buf, sizeof(buf));
                        cout <<"第" << loop << "次循环，文件读取" << ret << "字节"<< endl;
                        int res = write(newserverfd, buf, ret);//cout << "debug 9: " << buf << endl;//cout << "debug 9.1: " << ret << endl;//cout << "debug 10: " << res << endl;
                        //if(res == 0)break;
                        cout <<"第" << loop++ << "次循环，管道写入" << res << "字节"<< endl;
                        fsize -= ret;//cout << "debug 10.1: " << fsize << endl;
                        cout << "未发送文件大小: " << fsize << endl;
                    }//cout << "退出循环" << endl;
                    close(fd);
                    //接受客户端传来的md5
                    char md5_client[1024] = {0};
                    recv(newserverfd, md5_client, sizeof(md5_client), 0);//cout << "debug 11: " << md5_client << endl;
                    //验证md5并发送验证结果
                    //memset(send_buf, 0, sizeof(send_buf));
                    ret = strcmp(md5_client, md5_net);
                    if(ret) //不匹配
                        strcpy(send_buf, "download failed");
                    else
                        strcpy(send_buf, "download success");
                    send(newserverfd, send_buf, strlen(send_buf), 0);//cout << "debug 12: " << send_buf << endl;
                    cout << send_buf << endl;
                }
                else if(strcmp(type, "exit") == 0)  //用户退出
                {
                    cout << "用户" << userid <<"退出" << endl;
                    break;
                }
            }
        _exit(1);// exit(0)
        }
        close(newserverfd);
    }
    close(sock);
    return 0;
}
