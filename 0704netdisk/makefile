all:out
out:mysqlSingletonClass.cpp server.cpp
	g++ mysqlSingletonClass.cpp server.cpp -o server -I/usr/include/mysql -lmysqlclient
	g++ client.cpp -o client -lssl -lcrypto
#	g++ mysqlSingletonClass.cpp -o montool --std=c++11 \
	-I/usr/local/include/mongocxx/v_noabi \
	-I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/bsoncxx/v_noabi \
	-I/usr/local/include/libbson-1.0 \
	-L/usr/local/lib \
	-lmongocxx \
	-lbsoncxx
#	g++ mysqlSingletonClass.cpp -o sqltool -I/usr/include/mysql -lmysqlclient
#	g++ client.cpp -o client.out
