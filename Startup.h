#pragma once
#include<WinSock2.h>
#pragma comment(lib,"WS2_32.lib") 
#include"error_die.h"

class Startup
{
public:

	int startup(unsigned short* port);
};

