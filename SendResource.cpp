#include "SendResource.h"

//发送包含响应包的头信息
void SendResource::headers(int client, const char* type)
{
	char buff[1024];

	strcpy_s(buff, "HTTP/1.1 200 OK\r\n");//协议/版本+空格+状态码+空格+短语+回车换行
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "Server: PibaoHttpd/0.1\r\n");//关键字: 自定义名称/自定义版本号+回车
	cout << "buff=" << buff << endl;
	send(client, buff, strlen(buff), 0);

	//strcpy_s(buff, "Content-type:text/html\r\n");//文件类型关键字:具体类型+回车
	//send(client, buff, strlen(buff), 0);
	char buf[1024];
	sprintf_s(buf, "Content-type: %s\r\n", type);
	cout << "发送的类型为：" << buf << endl;
	send(client, buf, strlen(buf), 0);

	strcpy_s(buff, "\r\n");//发送空行表示结束
	send(client, buff, strlen(buff), 0);

	cout << "包头信息传输完毕" << endl;
}

//发送资源
void SendResource::cat(int client, FILE* resource)
{
	int count = 0;
	char buff[4096];//单次发包的数据量，一次发太多怕丢失

	while (1)
	{
		int ret = fread(buff, sizeof(char) // 每次读几个字节
			, sizeof(buff),//最多读满的字节
			resource);

		if (ret <= 0) break;

		send(client, buff, ret, 0);
		count += ret;
	}
	cout << "一共发送了" << count << "bit字节给浏览器" << endl;
}
