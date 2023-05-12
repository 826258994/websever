#include"Startup.h"
#include"Serverce.h"
using namespace std;

//线程函数，创建新线程执行的函数
DWORD WINAPI accept_request(LPVOID arg) {

	Serverce serverce;
	Exception ecp;//异常处理

	char buff[1024];//1k内存

	int client = (SOCKET)arg;//客户端套接字
	//读取一行数据
	int numbchars = serverce.get_line(client, buff, sizeof(buff));
	PRINTF(buff);//打印在哪个函数哪一行读到了什么结果

	//"GET / HTTP/1.1\n"，"GET"是请求方法类型
	//提取字符串中的方法，一般是GET、HOST
	char method[255];
	int  i = 0;
	while (isspace(buff[i])) i++;//去除开头的空字符
	int j = 0;
	while (!isspace(buff[i]))
	{
		method[j++] = buff[i++];//把"GET"写入method，GET后以空格终止
	}
	method[j] = 0;//'\0' 末尾结束
	PRINTF(method);

	//检查请求方法，本服务器是否支持
	if (_stricmp(method, "GET") //字符串相等返回0，不等返回非0，不区分大小写
		&& _stricmp(method, "HOST"))
	{
		//向浏览器返回错误提示页面
		ecp.unimplement(client);
		return 0;
	}

	//解析资源文件的路径
	char url[255];//存放请求资源的完整路径
	//"GET / HTTP/1.1\n"
	while (isspace(buff[i]) && i < sizeof(buff)) i++;//去除路径前的空字符
	j = 0;
	while (!isspace(buff[i]) && j < sizeof(url) - 1 && i < sizeof(buff))
	{
		url[j++] = buff[i++];
	}
	url[j] = 0;//'\0' 末尾结束
	PRINTF(url);

	//定义路径
	// url = /
	// url = /network
	char path[512];
	sprintf_s(path, "Webresource%s", url);//C语言函数，拼接字符串，path = htroot + url
	if (path[strlen(path) - 1] == '/')
	{
		strcat_s(path, "index.html");//如果 url = /，则访问默认网页 path =  htroot/index.html
	}
	PRINTF(path);

	//判断url对应的是具体文件还是目录
	struct stat status;
	if (stat(path, &status) == -1)//访问失败
	{
		//读取完请求包剩余的数据，否则可能影响后续的服务
		while (numbchars > 0 && strcmp(buff, "\n")) //最后一行是"\n"，如果buff和"\n"相等就结束
		{
			numbchars = serverce.get_line(client, buff, sizeof(buff));
		}
		ecp.not_find(client);//发送网页不存在
	}
	else {//访问成功
		if ((status.st_mode & S_IFMT) == S_IFDIR) {//如果访问的是一个目录 DIR
			strcat_s(path, "/index.html");//拼接默认文件
		}

		serverce.server_file(client, path);
	}

	closesocket(client);
	return 0;
}

int main(void)//int定义main表示返回整型，括号里写void表示传参一定为空
{
	Startup start;
	unsigned short port = 80;//定义端口,为0就动态分配
	int server_socket = start.startup(&port);
	cout << "服务端套接字为：" << server_socket << endl;
	cout << "httpd服务已启动，正在监听" << port << "端口..." << endl;

	struct sockaddr_in client_addr;//客户端地址
	int client_addr_len = sizeof(client_addr);

	while (1)
	{
		//阻塞式等待用户通过浏览器发起访问
		//accept特点
		//阻塞、同步：这个函数是阻塞的，没有客户端连接，那就一直卡在这儿等着。
		//多个链接：一次只能一个，5个就要5次循环
		int client_sock = accept(server_socket,
			(struct sockaddr*)&client_addr,
			&client_addr_len);//等待访问，创建并返回一个客户端的套接字，如果没有收到访问，则会一直等待
		cout << "客户端套接字为：" << client_sock << endl;
		if (client_sock < 0) error_die("accept");

		//创建新的线程，进程里面可包含多个线程
		//使用client_sock对用户进行访问
		DWORD threadId = 0;//线程id
		CreateThread(0, 0,
			accept_request, //线程函数
			(void*)client_sock, //线程函数的参数
			0, &threadId);

	}
	closesocket(server_socket);
	return 0;//返回了0表示程序执行成功
}