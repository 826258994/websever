#pragma once
#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"WS2_32.lib") 

#include<sys/types.h>
#include<sys/stat.h>

using namespace std;

class SendResource
{
public:
	//发送包含响应包的头信息
	void headers(int client, const char* type);

	//发送资源
	void cat(int client, FILE* resource);
};

