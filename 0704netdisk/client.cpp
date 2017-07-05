#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <openssl/md5.h>
#include <iomanip>

using namespace std;

//md5验证封装函数
void get_md5(char * path, char * md5)
{
    //获取文件的md5
    //==先定义对象
    MD5_CTX c;
    //==初始化对象
    MD5_Init(&c);
    //==读取文件内容给md5对象
    unsigned char md5_tmp[17] = {0};//格式问题，临时储存md5数据
    char buffer[1024] = {0};    //文件读取缓存
    int fd = open(path, O_RDONLY);
    int readlen = 0;
    int i = 0;
    while(readlen = read(fd, buffer, sizeof(buffer)))
    {
        MD5_Update(&c, buffer, readlen);
    }
    //==转md5格式
    MD5_Final(md5_tmp, &c);//=========这里把对象c里的内容输出到tmp？
    //md5传出
    for(i=0; i<16; ++i)
    {
        //cout<< hex << setw(2) << setfill('0') << (int)md5_tmp[i] << endl;;
        sprintf(md5, "%s%02x", md5, md5_tmp[i]);
    }
    close(fd);
}


int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        printf("参数错误\n");
        return -1;
    }

    //建立socket通讯
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9998);
    //连接服务器
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    //发送消息
    char send_buf[1024] = {0};
    char recv_buf[1024] = {0};
    char in_buf[1024] = {0};    //客户端输入

    sprintf(send_buf, "login %s %s", argv[1], argv[2]);
    send(sock, send_buf, strlen(send_buf), 0);

    //接收服务器返回的消息
    recv(sock, recv_buf, sizeof(recv_buf), 0);
    cout << recv_buf << endl;
    //处理消息
    if(strcmp(recv_buf, "login success") == 0)
    {
        //登录成功循环输入命令
        int ret = 0;
        while(1)
        {
            memset(recv_buf, 0, sizeof(recv_buf));
            memset(send_buf, 0, sizeof(send_buf));
            cout << "client >";
            fgets(in_buf, sizeof(in_buf), stdin);
            in_buf[strlen(in_buf) - 1] = '\0';
            char * type = strtok(in_buf, " ");
            //罗列该账户下的网盘文件
            if(strcmp(type, "list") == 0)
            {
                strcpy(send_buf, "list");
                send(sock, send_buf, strlen(send_buf), 0);
                recv(sock, recv_buf, sizeof(recv_buf), 0);
                cout << recv_buf;
            }
            //上传文件
            //upload 本地路径 网盘路径
            else if(strcmp(type, "upload") == 0)
            {
                //解析字符串
                char * path = strtok(NULL, " ");    //本地路径
                char * src_path = strtok(NULL, " ");//网盘路径， /a1/a1.txt
                char * file_name = rindex(src_path, '/')+1; //反向定位截取'\'，文件名

                //stat获取文件属性
                struct stat st;
                stat(path, &st);
                int file_size = st.st_size; //文件大小
                char md5[1024] = {0};
                get_md5(path, md5);         //获取md5

                //拼接字符串发给服务器：upload 文件名 网盘显示路径 文件大小 md5
                sprintf(send_buf, "upload %s %s %d %s", file_name, src_path, file_size, md5);
                send(sock, send_buf, strlen(send_buf), 0);//cout << "send_buf:" << send_buf<< endl;
                //接收服务器返回消息
                recv(sock, recv_buf, sizeof(recv_buf), 0);//cout << "recv_buf:" << recv_buf<< endl;
                //=服务器存在同md5文件，转存成功或者失败
                if(strcmp(recv_buf, "save success") == 0 || strcmp(recv_buf, "save failed") == 0)
                {
                    cout << recv_buf << endl;
                    continue;
                }
                //=服务器没有这个文件，需要上传
                if(strcmp(recv_buf, "upload file") == 0)
                {
                    //==打开文件，向服务器传输内容
                    int fd = open(path, O_RDONLY);
                    if(fd < 0)
                    {
                        cout << "文件创建失败，请检查文件路径是否正确" << endl;
                        continue;
                    }
                    int fsize = file_size;//cout << "fsize:" << fsize << endl;
                    int loop = 1;
                    cout << "文件大小: " << file_size << endl;
                    while(fsize)
                    {
                        char buffer[1024] = {0};
                        int ret = read(fd, buffer, sizeof(buffer));
                        cout <<"第" << loop << "次循环，文件读取" << ret << "字节"<< endl;
                        int res = send(sock, buffer, ret, 0);//cout << buffer << endl;
                        cout <<"第" << loop++ << "次循环，管道写入" << res << "字节"<< endl;
                        fsize -= ret;
                        cout << "未上传文件大小: " << fsize << endl;
                    }
                    close(fd);
                    //==接收服务器返回消息，检查是否传输成功
                    memset(recv_buf, 0, sizeof(recv_buf));
                    recv(sock, recv_buf, sizeof(recv_buf), 0);
                    cout << recv_buf << endl;
                    continue;
                }
            }
            //下载文件
            //download 网盘路径 本地路径
            else if(strcmp(type, "download") == 0)
            {
                //解析字符串
                char * src_path = strtok(NULL, " ");    //网盘路径
                char * path = strtok(NULL, " ");        //本地路径//cout << "debug 1: " << src_path << " " << path << endl;
                //发送报文给服务器
                sprintf(send_buf, "download %s", src_path);//download 网盘路径//cout << "debug 2: " << send_buf << endl;
                send(sock, send_buf, strlen(send_buf), 0);
                memset(send_buf, 0, sizeof(send_buf));
                //服务器返回信息--文件长度--错误判断
                int file_size = 0;
                recv(sock, &file_size, sizeof(file_size), 0);//必须指定长度，防止粘包//cout << "debug 3: " << recv_buf << endl;
                //int fsize = atoi(recv_buf);
                cout << "文件大小: " << file_size << endl;
                if(file_size == 0)
                {
                    cout << "接受文件错误" << endl;
                    continue;
                }
                //创建文件
                int fd = open(path, O_WRONLY | O_CREAT, 0777);//cout << "debug 3.2: " << fd << endl;
                if(fd < 0)
                {
                    cout << "文件创建失败，请检查文件路径是否正确" << endl;
                    continue;
                }
                //接受服务器数据写入文件
                int loop = 1;
                while(file_size > 0)
                {
                    //memset(recv_buf, 0, sizeof(recv_buf));//cout << "进入循环1" << endl;
                    char buf[2048] = {0};//cout << "进入循环2" << endl;
                    ret = read(sock, buf, sizeof(buf));//cout << "debug 3.3: " << ret << endl;
                    cout <<"第" << loop << "次循环，管道读取" << ret << "字节"<< endl;
                    int res = write(fd, buf, ret);//cout << "debug 4: " << buf << endl;//cout << "debug 5: " << res << endl;
                    cout <<"第" << loop++ << "次循环，文件写入" << res << "字节"<< endl;
                    //if(res == 0)/break;
                    file_size -= ret;
                    cout << "未下载文件大小: " << file_size << endl;
                }
                close(fd);
                //验证文件md5//cout << "退出循环" << endl;
                //=调函数取md5信息，发给服务器
                get_md5(path, send_buf);//cout << "debug 6: " << send_buf << endl;
                send(sock, send_buf, strlen(send_buf), 0);
                //=服务器返回校验信息
                memset(recv_buf, 0, sizeof(recv_buf));
                recv(sock, recv_buf, sizeof(recv_buf), 0);//cout << "debug 7: " << recv_buf << endl;
                cout << recv_buf << endl;
            }
            //退出
            else if(strcmp(type, "exit") == 0)
            {
                strcpy(send_buf, "exit");
                send(sock, send_buf, strlen(send_buf), 0);
                cout << "bye" << endl;
                break;
            }
        }
    }

    close(sock);
    return 0;
}
