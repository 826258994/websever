#pragma once
#include"SendResource.h"
using namespace std;

class Exception
{
public:
	//404网页不存在
	void not_find(int client);

	//错误提示页面函数
	void unimplement(int client);
};

