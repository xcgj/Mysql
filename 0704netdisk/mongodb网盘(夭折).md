-- 登陆mongo
xcgj@xcgj:~/0703mongodbAPI/netdisk$ mongo

-- 创建网盘数据库
> use netdisk
switched to db netdisk

--创建网盘文件信息数据集合
> db.file_system.insert({"user_file_id":1, "dst_path":"a1/a1.txt", "md5":"12345"});
WriteResult({ "nInserted" : 1 })

--写服务器客户端，sql和mongodb的操作类
