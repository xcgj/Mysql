# MySQL 文档
## 1. MySQL 数据库的安装  
`sudo apt install mysql-server-5.7`  
`sudo apt install mysql-client-5.7`  
`sudo apt install libmysqlclient-dev`    //API开发工具包

## 2. MySQL 数据库的登录和退出  
`mysql -u用户名 -p密码`  
eg. `mysql -uroot -p205205`  
`exit`  

## 3. MySQL 服务的启动和停止  
`sudo service mysql start ;`  
`sudo service mysql stop;`
`sudo service mysql status;`

## 重启mysql服务
sudo mysql server restart

## 4. 对数据库的操作
* 查看数据库  
`show databases;`  
`show create database 数据库名;` //查看创建数据库时的语句
* 创建数据库  
`create database 数据库名;`  //默认字符集是latin1(拉丁文，不支持中文)  
`create database 数据库名 character set utf8;` //创建一个数据库指定为utf8字符集  
`create database 数据库名 character set utf8 collate utf8_general_ci;` //对输入的字符集检查
* 删除数据库  
`drop database 数据库名;`  
* 查看当前数据库  
`select database();`
* 切换数据库  
`use 数据库名;`  
* 修改数据库的字符集  
`alter database 数据库名 character set utf8;`

## 5. 对用户的操作  
* 查看用户  
`select host,user from mysql.user;`  
* 创建用户  
`create user '用户名'@'允许连接的主机IP' identified by '密码';`  
eg.  
`create user 'test'@'%' identified by 'test';` //任意地址
`create user 'test'@'127.0.0.1' identified by 'test';`  
`create user 'test'@'localhost' identified by 'test';`  
* 删除用户  
`drop user '用户名'@'允许连接的主机IP';`  
`delete from mysql.user where user = '用户名' and host = '允许连接的主机IP';`  
* 修改用户密码  
`update mysql.user set password = password('新密码') where user = '用户名' and host = '允许连接的主机IP';`  
//mysql 5.7已废弃 password 字段,改为 authentication_string  
`update mysql.user set authentication_string = password('新密码') where user = '用户名' and host = '允许连接的主机IP';`
//=password是将明文转换为密文
* 设置用户权限  
`grant 权限 on 数据库.* to '用户名'@'允许连接的主机IP';`  //需要用户存在    
`grant 权限 on 数据库.* to '用户名'@'允许连接的主机IP' identified by '密码';` //自动创建一个用户  
eg.  
`grant insert,delete,update,select on test.* to 'test'@'localhost';`  
`grant all privileges on test.* to 'test'@'localhost';` //为test用户设置test数据库的所有权限  
* 刷新权限表  `flush privileges;`  


## 6. 对数据表的操作  
* 创建数据表  
`create table 表名(字段名 字段类型，字段名 字段类型，...);`  
==create table test(id int, name varchar(12)/*默认大小12*/, age int);
==运行-运行已选择的（bug）
==show tables(查看表)
==show create table test
==desc test
==select * from test
`create table 表名(字段名 字段类型，字段名 字段类型，...) character set utf8;`
* 删除数据表  
`drop table 表名;`
==drop table test
* 修改数据表  
    1. 修改表名:  
    `rename table 原表名 to 新表名;`  
    2. 增加字段:  
    `alter table 表名 add column 字段名 字段类型;` //column 可省略  
    3. 修改字段类型:  
    `alter table 表名 modify column 字段名 新字段类型;`  
    4. 修改字段名和字段类型:  
    `alter table 表名 change column 原字段名 新字段名 新字段类型;`  
    5. 删除字段:  
    `alter table 表名 drop column 字段名;`  
    6. 修改表的字符集  
    `alter table 表名 character set utf8;`
* 显示数据库中的表  
`show tables;`  
* 显示数据表的结构  
`desc 表名;`  
`describe 表名;`  
* 查看创建表时的语句  
`show create table 表名;`

## 7. 数据类型  
* 整数类型  
`tinyint    1字节`  
`smallint   2字节`  
`int或integer    4字节`  
`bigint     8字节`  
`float(m,n)      4字节`  //不指定m,n有可能无法通过where条件查询  
`double     8字节`  
`decimal(m,d)   m>d?m+2:d+2     m代表数据的宽度，d代表小数宽度`   
* 字符串类型  
`char   定长字符串`  
`varchar    变长字符串`  //超出自动变长
`text   长文本数据`  
`blob   二进制长文本数据`  
* 日期和时间类型  
`date   4字节 yyyy-mm-dd  日期`  
`time   3字节 hh:mm:ss    时间`  
`year   1字节 yyyy        年份`  
`datetime   8字节 yyyy-mm-dd hh:mm:ss 混合日期时间`  
`timestamp  4字节 yyyy-mm-dd hh:mm:ss 混合日期时间 如果未明确赋值或赋值为null,则自动使用当前系统时间`  
* 复合类型  
`enum   只能使用集合中的一个值或null`  
`set    只能使用集合中的多个值或null`  //使用set时，多个值放在一个字符串内，逗号隔开：‘1，2，3’
## 8. 对数据的操作  
* 插入数据  
`insert into 表名(字段名，字段名，...) values(字段值，字段值，...);` //如果为所有字段插入数据可以不指定字段名  
==可以指定部分字段或全部字段(全部字段可以省略字段名)
* 删除数据  
`delete from 表名;` //删除表中所有数据  
`delete from 表名 where 字段名 = ?;` //根据条件删除  
`truncate 表名;` //这种方式效率高  
* 修改数据  
`update 表名 set 字段名 = ?;` //修改单个字段  
`update 表名 set 字段名 = ?,字段名=?;` //修改多个字段  
`update 表名 set 字段名 = ? where 字段名 = ?;` //根据条件修改  
* 查询数据  
    1. 查询所有数据  
    `select * from 表名;`   
    2. 查询指定字段  
    `select 字段名,字段名 from 表名;`   
    3. 根据条件查询  
    `select 字段名 from 表名 where 字段名 = ?;`   
    *  *  *
    4. 字段别名  
    `select 字段名 as 别名 from 表名;` //as 可以省略  
    5. 去掉重复数据  
    `select distinct 字段名 from 表名;`
    //当指定多个字段时，多个字段必须全部匹配才会成功    
    6. 滤空修正  
    `select ifnull(字段名,替换值) from 表名;`
    7. 使用算术表达式  
    `select 字段名 + 字段名 as '别名' from 表名;`  
    8. 字符串拼接  
    `select concat(str1,str2,...) as '别名' from 表名;`  
    9. 常用函数 min,max,avg,count  
    `select min(字段名) from 表名;`  
    *  *  *  
    10. 条件查询运算符  
    `select * from 表名 where 字段名 > 3;` // =,>,>=,<,<=  
    `select * from 表名 where 字段名 <> 3;` // <> != 不等于  
    `select * from 表名 where 字段名 between 3 and 5;` //区间  
    `select * from 表名 where 字段名 is null;` //空值  
    `select * from 表名 where 字段名 is not null;` //非空值  
    `select * from 表名 where 字段名 = ? and 字段名 = ?;` //与  &&
    `select * from 表名 where 字段名 = ? or 字段名 = ?;` //或  ||
    11. 模糊查询  
    `select * from 表名 where 字段名 like '%xxx';` // % 表示任意个数的任意字符  
    `select * from 表名 where 字段名 like '_xxx';` // _ 表示单个的任意字符  
    `select * from 表名 where 字段名 rlike '[0-9]abc';` // [] 表示单个字符的取值范围  
    `select * from 表名 where 字段名 rlike '[^0-9]abc';`// [^] 表示单个字符的非取值范围  
    `select * from 表名 where 字段名 like '%\%%';` // \ 表示转义,查询字段中包含%的数据  
    12. 分组查询  
    `select * from 表名 where 查询条件 group by 字段名 having 查询条件;`
    ==where写在分组前且不能用聚合函数，where是对分组前的字段进行判断，having对分组前后都可以条件判断
    ==没分组的时候，聚合函数不能和普通字段一起用
    ==分组以后，所查询的字段（select）只能是被分组的字段或者是聚合函数求出来的字段，
    == 因为分组以后，被分组的字段需要合并成一个，那么其他字段也需要合并成一个，所以需要用到聚合函数求一个结果
    13. 排序查询  
    `select * from 表名 where 查询条件 order by 字段名 asc;` // asc 表示升序排列,可省略  
    `select * from 表名 where 查询条件 order by 字段名 desc;` // desc 表示降序排列
    ==多字段排序时，可以给每个字段指定升序还是降序排列，否则每个字段默认以升序排列
    14. 嵌套查询  
    in == = any
    not in == <> all
    `select * from 表名 where 字段名 = (select 字段名 from 表名 where 字段名=?);`  
        * 当子查询返回的值只有一个时，才可以直接使用 =,>,< 等运算符;  
        * 当子查询返回的值是一个结果集时，需要使用 in,not in,any,some,all 等操作符;  
        * all : 匹配结果集中的所有结果, 例如 > all 表示比所有结果都大;  
        * any : 匹配结果信中任意一个结果, 例如 < any 表示只要比其中任意一个小就符合条件;  //some 和any差不多
        * in : 查询的数据在子查询的结果集中,相当于 = any;  
        * not in : 查询的数据不在子查询结果集中,相当于 <> all;  
        * some : 匹配结果集中的部分结果，基本等同于 any,不常用;  
        * 子查询通常只用于返回单个字段的结果集，如果需要匹配多个字段，可以使用多表联查(后面讲);   
    eg.  
    `select * from 表名 where 字段名 > all (select 字段名 from 表名);`  
    15. 多表联查  
        * 数据表的别名  
        `select * from 表名 as 别名;` // as 可省略  
        * 交叉连接(笛卡尔积)  
        //显示两张表的乘积  
        `select * from 表1 cross join 表2 on 连接条件 where 查询条件;` //cross join on 可省略  
        `select * from 表1,表2 where 查询条件;` //默认就是交叉连接,如果 where 条件中有等值判断则称为等值连接  
        * 内连接  
        //显示两张表的交集  
        `select * from 表1 inner join 表2 on 连接条件 where 查询条件;` //inner 可省略  
        == select * from a cross join b on a.id = b.id
        == select * from a cross join b on a.id = b.id
        == select * from a inner join b on a.id = b.id
        * 左外连接  
        //左表显示全集,右表显示交集  
        `select * from 表1 left outer join 表2 on 连接条件 where 查询条件;` //outer 可省略  
        == select * from a left outer join b on a.id = b.id;
        * 右外连接  
        //左表显示交集,右表显示全集  
        `select * from 表1 right outer join 表2 on 连接条件 where 查询条件;` //outer 可省略  
        == select * from a right outer join b on a.id = b.id;
        == select * from a, b where a.id = b.id(+);//仅支持oracl
        * 联合查询  
        //显示两张表的并集,要求两张表的查询字段名必须保持一致  
        `select id,name from 表1 union select id,name from 表2;`
        == select id, age name from a union select id, name from b
        * 全外连接(MySQL不支持)  
        //显示两张表的并集  
        `select * from 表1 full outer join 表2 on 连接条件 where 查询条件;` //outer 可省略  
        ==  select * from a full outer join b on a.id = b.id
        //可以通过左外连接和右外连接的联合查询来实现  
        `select * from 表1 left join 表2 on 连接条件 where 查询条件 union select * from 表1 right join 表2 on 连接条件 where 查询条件;`  
        == select * from a left outer join b on a.id = b.id UNION select * from a right outer join b on a.id = b.id

        *  *  *  
    16. 常用字符串函数  
        * upper 和 ucase  
        //把所有字符转换为大写字母  
        `select upper(name) from 表名;`  
        == select upper('aaa') from dual;
        == select ucase('aaa');
        * lower 和 lcase  
        //把所有字符转换为小写字母  
        `select lcase(name) from 表名;`
        == select lower('BBB') from dual;
        == select lcase('BBB');
        * replace(str, from_str, to_str)  
        //把str中的from_str替换为to_str  
        `select replace(字段名,替换前的值,替换后的值) from 表名;`  
        == replace(name, '任我行', 'aa');
        * repeat(str,count)  
        //返回str重复count次的字符串  
        `select repeat('abc',2) from 表名;` // abcabc  
        == select repeat('aaa', 3) from dual;
        * reverse(str)  
        //逆序字符串  
        `select reverse('abc') from 表名;` // cba  
        == select reverse('abcdefg') from dual;
        * insert(str,pos,len,newstr)  
        //把str中pos位置开始长度为len的字符串替换为newstr  
        `select insert('abcdef',2,3,'hhh');` // ahhhef  
        == select insert('abcdefg', 3, 2, 'kkkk');//超过部分增加字符串长度
        * substring(str from pos)  
        //从str中的pos位置开始返回一个新字符串
        `select substring('abcdef',3);` // cdef  
        * substring_index(str,delim,count)  
        //返回str中第count次出现的delim之前的所有字符,如果count为负数,则从右向左  
        `select substring_index('abacadae','a',3);` // abac
        == select substring_index('abacadae','a',-2);//dae  
        * ltrim(str)  
        //去除字符串左边的空格  
        `select ltrim('  abc');`  
        * rtrim(str)  
        //去除字符串右边的空格  
        `select rtrim('abc  ');`  
        * trim(str)  
        //去除字符串左右两边的空格  
        `select trim('   abc   ');`  
        * mid(str,pos,len)  
        //从str中的pos位置开始返回len个长度的字符串  
        `select mid('abcdef',2,3);` // bcd  
        * lpad(str,len,padstr)  
        //在str左边填充padstr直到str的长度为len  
        `select lpad('abc',8,'de');` // dededabc  
        * rpad(str,len,padstr)  
        //在str右边填充padstr直到str的长度为len  
        `select rpad('abc',8,'de');` // abcdeded  
        * left(str,len)  
        //返回str左边的len个字符  
        `select left('abcd',2);` // ab  
        * right(str,len)  
        //返回str右边的len个字符  
        `select right('abcd',2);` // cd  
        * position(substr in str)  
        //返回substr在str中第一次出现的位置  
        `select position('c' in 'abcdc');` // 3  
        * length(str)  
        //返回字符串的长度  
        `select length('abcd');` // 4  
        * concat(str1,str2,...)  
        //合并字符串  
        `select concat('abc','def','gh');` // abcdefgh  
    17. 日期时间函数  
        * dayofweek(date)  
        //返回date是星期几,1代表星期日,2代表星期一...  
        `select dayofweek('2017-04-09');`  
        * weekday(date)  
        //返回date是星期几,0代表星期一,1代表星期二...  
        `select weekday('2017-04-09');`  
        * dayname(date)  
        //返回date是星期几(按英文名返回)  
        `select dayname('2017-04-09');`
        * dayofmonth(date)  
        //返回date是一个月中的第几天(范围1-31)  
        `select dayofmonth('2017-04-09');`
        * dayofyear(date)  
        //返回date是一年中的第几天(范围1-366)  
        `select dayofyear('2017-04-09');`  
        * month(date)  
        //返回date中的月份数值  
        `select month('2017-04-09');`  
        * monthname(date)  
        //返回date是几月(按英文名返回)  
        `select monthname('2017-04-09');`  
        * quarter(date)  
        //返回date是一年中的第几个季度  
        `select quarter('2017-04-09');`  
        * week(date,first)  
        //返回date是一年中的第几周(first默认值是0,表示周日是一周的开始,取值为1表示周一是一周的开始)  
        `select week('2017-04-09');`  
        `select week('2017-04-09',1);`  
        * year(date)  
        //返回date的年份  
        `select year('2017-04-09');`  
        * hour(time)  
        //返回time的小时数  
        `select hour('18:06:53');`  
        * minute(time)  
        //返回time的分钟数
        `select minute('18:06:53');`  
        * second(time)  
        //返回time的秒数  
        `select second('18:06:53');`  
        * period_add(p,n)  
        //增加n个月到时期p并返回(p的格式为yymm或yyyymm)  
        `select period_add(201702,2);`  
        * period_diff(p1,p2)  
        //返回在时期p1和p2之间的月数(p1,p2的格式为yymm或yyyymm)  
        `select period_diff(201605,201704);`  
        * date_format(date,format)  
        //根据format字符串格式化date  
        `select date_format('2017-04-09','%d-%m-%y');`  
        * time_format(time,format)  
        //根据format字符串格式化time  
        `select time_format('12:22:33','%s-%i-%h');`  
        * curdate() 和 current_date()  
        //以'yyyy-mm-dd'或yyyymmdd的格式返回当前日期值  
        `select curdate();`  
        `select current_date();`  
        * curtime() 和 current_time()  
        //以'hh:mm:ss'或hhmmss格式返回当前时间值  
        `select curtime();`  
        `select current_date();`  
        * now(),sysdate(),current_timestamp()  
        //以'yyyy-mm-dd hh:mm:ss'或yyyymmddhhmmss格式返回当前日期时间
        `select now();`  
        `select sysdate();`  
        `select current_timestamp();`  
        * unix_timestamp()  
        //返回一个unix时间戳(从'1970-01-01 00:00:00'开始到当前时间的秒数)  
        `select unix_timestamp();`  
        * sec_to_time(seconds)  
        //把秒数seconds转化为时间time  
        `select sec_to_time(3666);`  
        * time_to_sec(time)  
        //把时间time转化为秒数seconds  
        `select time_to_sec('01:01:06');`  
    18. top N 问题  
    `select * from 表名 limit 0,N;`  //取第0行开始取N行数据
## 9. 索引  
* 普通索引   
`create index 索引名 on 表名(字段名(索引长度));//长度针对字符串类型`，其他类型不需要加长度  
== create index name_index on test(name(5));//根据name的前5个字符排序
== create index id_index on test(id);
`alter table 表名 add index 索引名 (字段名(索引长度));`  
== alter table test1 add index numindex(num(4));
`create table 表名(字段名 字段类型,字段名 字段类型,index 索引名 (字段名(索引长度));`
== create table test2(id int, vi varchar(10), index vi_index(vi(10)));
* 唯一索引  
`create unique index 索引名 on 表名(字段名(索引长度));`//要求字段名是唯一键，否则创建失败
== create unique index uindex on test(id);
`alter table 表名 add unique 索引名 (字段名(索引长度));`  
== alter table test add unique uindex(id);
`create table 表名(字段名 字段类型,字段名 字段类型,unique 索引名 (字段名(索引长度));`  
== create table test12(id int, age int, unique uindex(id));

* 全文索引  
//只支持 MyISAM 引擎,innoDB不支持
`create fulltext index 索引名 on 表名(字段名);`  
== create fulltext index myindex on test(id);
`alter table 表名 add fulltext 索引名(字段名);`  
== alter table test add fulltext myindex(id);
`create table 表名(字段名 字段类型,字段名 字段类型,fulltext (字段名);`
== create table test(age int, id, int, fulltext(id));
* 组合索引  
`create index 索引名 on 表名(字段名(索引长度),字段名(索引长度),...);`  
== create index uni_index on test(id, name(5));//必须这两个字段一起查，单个查需要另建索引
`alter table 表名 add index 索引名 (字段名(索引长度),字段名(索引长度),...;`  
== alter table test add index uni_index(id, name(5));
`create table 表名(字段名 字段类型,字段名 字段类型,index 索引名 (字段名(索引长度),字段名(索引长度));`  
== create table test(id int, name varchar(5), index uni_index(id, name(5)));
* 查看索引  
`show index from 表名;`  
== show index from test;
* 删除索引  
`alter table 表名 drop index 索引名;`
== alter table test drop index id_index;
## 10. 约束  
* 主键约束，表会为主键自动创建索引， 主键是唯一的
`create table 表名(字段名 字段类型 primary key,字段 字段类型,...);`  
== create table test(id int primary key, name varchar(10));
//一个表只能有一个主键,这个主键可以由一列或多列组成  
`create table 表名(字段1 字段类型,字段2 字段类型,primary key(字段1,字段2);`  
== create table test(id int, age int, primary key(id, age));
//增加现有表的主键
== alter table test add constraint primary key(id);

* 唯一键约束，字段里面的值都是唯一的，不会重复
`create table 表名(字段名 字段类型 unique,字段名 字段类型,...);`  
== create table test(id int unique, age int);

* 外键约束，要求所引用的表（表2）的字段必须是一个主键
//引用后sid只能使用id里面的值
//删除表2被引用的字段的值需要先解除引用（先删子表引用记录）
`create table 表1(字段1 字段类型,字段2 字段类型,foreign key(字段1) references 表2(字段名),...);`  
== create table main(id int, name varchar(10), primary key(id));//先创建有主键的表
   create table sub(sid int, age int, foreign key(sid) references main(id));//再引用，

* 非空约束  
`create table 表名(字段名 字段类型 not null,字段名 字段类型,...);`  
== create table test5(id int, age int not null);

* 默认值约束  
`create table 表名(字段名 字段类型 default 默认值,字段名 字段类型,...);`  
== create table test6(id int, age int default 20);

* check约束(MySQL 不支持) ，不符合条件就报错
`create table 表名(字段1 字段类型,字段2 字段类型,check(字段1 > 30),...);`
== create table test7(id int, age int, check(age > 18));

* 自增长字段，必须是一个主键
== create table test8(id int primary key auto_increment, age int);
    insert into test8(age) values(10);//默认第一行从1开始
== create table test9(id int primary key auto_increment, age int) auto_increment=100;//sql server 指定增长点和增长量， 先创建再指定
== create table test(id int primary key identity(10,2));//从10开始，增长量2

## 11. 事务
//保证操作的原子性
`begin;`  
`要执行的操作;`  
`commit;` 或 `rollback;`//commit以后才会生效，commit后再rollback无效

## 12. 触发器  
`create trigger 触发器名`  
`{before|after}`  
`insert|update|delete}`  
`on 表名`  
`for each row`  //行触发器(被引用几次就执行几次),MySQL不支持语句触发器(执行语句执行一次就触发一次)  
`begin`  
`触发器执行的操作` //如果要获取表中的数据当作条件,insert时用NEW，delete时用OLD,update时都可以  
`end;`

== 可用于删除外键：
create table main(id int, name varchar(10), primary key(id));
create table sub(sid int, age int, foreign key(sid) references main(id));
select * from main;
select * from sub;
insert into main values(1, 10);
insert into main values(2, 20);
insert into main values(3, 30);
insert into sub values(1, 11);
insert into sub values(2, 22);
insert into sub values(3, 34);
insert into sub values(1, 21);
insert into sub values(2, 32);
insert into sub values(3, 44);

create trigger outdel
before
delete
on main
for each row
begin
delete from sub where sid = OLD.id;
end;

delete from main where id = 2;

## 13. 存储过程  
* 创建存储过程，参数分为输入和输出参数  
`create procedure 存储过程名(参数)`   
`begin`  
`要执行的操作`  
`end;`  
* 调用存储过程  
`call 存储过程名();`  
* 删除存储过程  
`drop procedure 存储过程名;`

==
create procedure func(IN id int, IN age, int)
begin
  insert into test(id, age);
end;

call func;

drop procedure func;

## 14. 视图  
`create view 视图名`  
`as`  
`查询的SQL语句`  

==
select * from main right join sub on main.id = sub.sid;//原始查询

create view tv
as
select main.id, main.name, sub.sid, sub.age
from main right join sub on main.id=sub.sid;//建立视图

select * from tv;//查询视图

## 15. 数据库的导入导出  
* 导入  
`source xxx.sql;`  
* 导出  
`mysqldump -u root -p 数据库名[表名] > xxx.sql;`
