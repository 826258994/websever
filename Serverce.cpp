#include "Serverce.h"


//从指定的客户端套接字sock，读取一行数据，保存到buff，返回实际读取到的字节数
int Serverce::get_line(int sock, char* buff, int size)
{
	char c = 0;
	int i = 0;


	while (i < size - 1 && c != '\n') {
		int n = recv(sock, &c, 1, 0);//从输入队列读一个删一个
		if (n > 0)
		{
			//意外情况，回车符是\n，也有可能是\r\n，对于\r\n的可能情况也要处理
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);//查看下一个字符是什么，但不取，也就是不删除输入队列的数据
				if (n > 0 && c == '\n') { //下一个字符是\n，说明是\r\n情况，也是回车
					recv(sock, &c, 1, 0);//正式读取
				}
				else {
					//如果\r后面不是\n，那可能是损坏了，就把它变成\n，后面是啥不管了
					c = '\n';
				}
			}
			buff[i++] = c;
		}
		else {
			break;
		}
	}
	buff[i] = 0;//字符串结束符，'\0'
	return i;
}

//发送网页
void Serverce::server_file(int client, const char* fileName)
{
	Exception ecp;//异常处理
	int numbchars = 1;//注意如果是char型，范围是-127~128，若某一行的字符比较多，
	//则char会溢出变负数，就结束了，剩余的数据读不完
	char buff[1024];

	//读取完请求包剩余的数据，否则可能影响后续的服务，引起拥塞
	while (numbchars > 0 && strcmp(buff, "\n")) //最后一行是"\n"，如果buff和"\n"相等就结束
	{
		numbchars = get_line(client, buff, sizeof(buff));
		PRINTF(buff);
	}

	//判断文件类型，看用哪种方式打开
	FILE* resource;
	if (isTxt(fileName))//如果是文本类型的文件
	{
		resource = fopen(fileName, "r");//以文本格式读文件
	}
	else
	{
		//否则以二进制格式打开文件
		resource = fopen(fileName, "rb");
	}

	if (resource == NULL) {
		ecp.not_find(client);
	}
	else
	{
		SendResource sd;
		//正式发送思源给客户端

		//判断请求文件的类型，以便发送正确的请求头content-type
		cout << "Content-type类型：" << getContent_type(fileName) << endl;
		sd.headers(client, getContent_type(fileName));

		//发送请求资源信息
		sd.cat(client, resource);
		cout << "文件资源发送完毕！" << endl;
	}

	fclose(resource);
}

//判断是否是文本文件的函数
bool Serverce::isTxt(const char* fileName)
{
	set<string> textType{ "txt","doc","html","docx","md" };//需要以文本形式打开的后缀库
	int i = 0;
	while (fileName[i] != '.') i++;
	string fileType;
	for (int j = i + 1; j < strlen(fileName); j++)
	{
		fileType += fileName[j];//fileType 等于fileName的后缀名
	}

	return textType.find(fileType) != textType.end();
}

//判断文件类型，选择合适的Content-type
const char* Serverce::getContent_type(const char* fileName)
{
	const char* type = { "text/html" };
	if (isTxt(fileName)) {
		return type;
	}
	else {
		int i = 0;
		while (fileName[i] != '.') i++;
		string fileType;
		for (int j = i + 1; j < strlen(fileName); j++)
		{
			fileType += fileName[j];//fileType 等于fileName的后缀名
		}
		if (fileType == "png") type = { "image/png" };

		else if (fileType == "jpg") type = { "image/jpeg" };

		else if (fileType == "css") type = { "text/css" };

		else if (fileType == "js") type = { "application/x-javascript" };
	}
	return type;
}

