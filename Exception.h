#pragma once
#include"SendResource.h"
using namespace std;

class Exception
{
public:
	//404��ҳ������
	void not_find(int client);

	//������ʾҳ�溯��
	void unimplement(int client);
};

