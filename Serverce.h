#pragma once
#include"Exception.h"
#include<set>
using namespace std;

#define PRINTF(str) cout << "[" << __func__ << "-" << __LINE__ << "]" << "str=" << str<< endl;//定义打印宏

class Serverce
{
public:

	//发送网页
	void server_file(int client, const char* fileName);

	//从指定的客户端套接字sock，读取一行数据，保存到buff，返回实际读取到的字节数
	int get_line(int sock, char* buff, int size);

	//判断是否是文本文件的函数
	bool isTxt(const char* fileName);

	//判断文件类型，选择合适的Content-type
	const char* getContent_type(const char* fileName);


};

