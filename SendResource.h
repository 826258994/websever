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
	//���Ͱ�����Ӧ����ͷ��Ϣ
	void headers(int client, const char* type);

	//������Դ
	void cat(int client, FILE* resource);
};

