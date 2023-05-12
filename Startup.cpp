#include "Startup.h"

//实现网络初始化
//返回套接字(服务器端的套接字)
//端口，参数：port表示端口
//			  如果*port的值为0，那么自动分配可用的端口
int Startup::startup(unsigned short* port)
{
	//1. 网络通信初始化（Linux不需要）
	WSADATA data;
	int ret = WSAStartup(
		MAKEWORD(1, 1), &data);//1.1版本的协议
	if (ret) //ret != 0，初始化失败
	{
		error_die("WSAStartup");
	}

	//创建套接字
	int server_socket = socket(PF_INET,//套接字类型
		SOCK_STREAM,//数据流
		IPPROTO_TCP);
	if (server_socket == -1)
	{
		//创建失败
		error_die("套接字");
	}

	//设置端口可复用
	int opt = 1;
	ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
	if (ret == -1) error_die("setsockopt");

	//配置服务器端的网络地址
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;//类型
	server_addr.sin_port = htons(*port);//端口
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//设置任意类型的IP都能进来


	//绑定套接字
	ret = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret < 0) error_die("bind");

	//创建监听队列
	if (listen(server_socket, 5) < 0) error_die("listen");

	//动态分配端口
	if (*port == 0)
	{
		int addrLen = sizeof(server_addr);
		if (getsockname(server_socket, (struct sockaddr*)&server_addr,
			&addrLen) == -1)
			error_die("getsockname");
		*port = server_addr.sin_port;//目前分配的真正端口
	}

	return server_socket;
}