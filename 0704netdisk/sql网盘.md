xcgj@xcgj:~/0703mongodbAPI/netdisk$ mysql -uroot -pxcgj
-- 创建网盘数据库
mysql> create database netdisk character set utf8;
Query OK, 1 row affected (0.00 sec)

mysql> use netdisk;
Database changed

-- 创建保存用户名和密码的表
mysql> create table user(id int primary key auto_increment, username varchar(12) not null unique, password varchar(16) not null);
Query OK, 0 rows affected (0.03 sec)

-- 创建保存文件信息的表
mysql> create table user_file(id int primary key auto_increment, userid int, foreign key(userid) references user(id), file_name varchar(12) not null, src_path varchar(100) not null, dst_path varchar(100) not null, upload_date timestamp, file_size int not null, md5 varchar(20) not null);
Query OK, 0 rows affected (0.03 sec)

--创建用户信息
mysql> insert into user values(NULL, 'a1', '111111');
Query OK, 1 row affected (0.01 sec)
mysql> insert into user values(NULL, 'a2', '222222');
Query OK, 1 row affected (0.01 sec)

--写服务器，客户端，sql的操作类
