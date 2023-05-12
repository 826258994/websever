#include "Exception.h"

void Exception::not_find(int client)
{
	//向浏览器发送notfind的网页
	char path[] = "Webresource/not_find.html";
	FILE* resource = fopen(path, "r");

	//发送头
	char buff[1024];

	strcpy_s(buff, "HTTP/1.1 404 NOT FOUND\r\n");//错误响应404
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "Server: PibaoHttpd/0.1\r\n");//关键字: 自定义名称/自定义版本号+回车
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "Content-type:text/html\r\n");//文件类型关键字:具体类型+回车
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "\r\n");//发送空行表示结束
	send(client, buff, strlen(buff), 0);

	SendResource sd;
	//发送请求资源信息
	sd.cat(client, resource);
	cout << "错误提示页面发送完成" << endl;
}

//错误提示页面函数
void Exception::unimplement(int client)
{
	//向指定的套接字，发送提示还没有实现的错误页面

}