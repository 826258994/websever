#include"Startup.h"
#include"Serverce.h"
using namespace std;

//�̺߳������������߳�ִ�еĺ���
DWORD WINAPI accept_request(LPVOID arg) {

	Serverce serverce;
	Exception ecp;//�쳣����

	char buff[1024];//1k�ڴ�

	int client = (SOCKET)arg;//�ͻ����׽���
	//��ȡһ������
	int numbchars = serverce.get_line(client, buff, sizeof(buff));
	PRINTF(buff);//��ӡ���ĸ�������һ�ж�����ʲô���

	//"GET / HTTP/1.1\n"��"GET"�����󷽷�����
	//��ȡ�ַ����еķ�����һ����GET��HOST
	char method[255];
	int  i = 0;
	while (isspace(buff[i])) i++;//ȥ����ͷ�Ŀ��ַ�
	int j = 0;
	while (!isspace(buff[i]))
	{
		method[j++] = buff[i++];//��"GET"д��method��GET���Կո���ֹ
	}
	method[j] = 0;//'\0' ĩβ����
	PRINTF(method);

	//������󷽷������������Ƿ�֧��
	if (_stricmp(method, "GET") //�ַ�����ȷ���0�����ȷ��ط�0�������ִ�Сд
		&& _stricmp(method, "HOST"))
	{
		//����������ش�����ʾҳ��
		ecp.unimplement(client);
		return 0;
	}

	//������Դ�ļ���·��
	char url[255];//���������Դ������·��
	//"GET / HTTP/1.1\n"
	while (isspace(buff[i]) && i < sizeof(buff)) i++;//ȥ��·��ǰ�Ŀ��ַ�
	j = 0;
	while (!isspace(buff[i]) && j < sizeof(url) - 1 && i < sizeof(buff))
	{
		url[j++] = buff[i++];
	}
	url[j] = 0;//'\0' ĩβ����
	PRINTF(url);

	//����·��
	// url = /
	// url = /network
	char path[512];
	sprintf_s(path, "Webresource%s", url);//C���Ժ�����ƴ���ַ�����path = htroot + url
	if (path[strlen(path) - 1] == '/')
	{
		strcat_s(path, "index.html");//��� url = /�������Ĭ����ҳ path =  htroot/index.html
	}
	PRINTF(path);

	//�ж�url��Ӧ���Ǿ����ļ�����Ŀ¼
	struct stat status;
	if (stat(path, &status) == -1)//����ʧ��
	{
		//��ȡ�������ʣ������ݣ��������Ӱ������ķ���
		while (numbchars > 0 && strcmp(buff, "\n")) //���һ����"\n"�����buff��"\n"��Ⱦͽ���
		{
			numbchars = serverce.get_line(client, buff, sizeof(buff));
		}
		ecp.not_find(client);//������ҳ������
	}
	else {//���ʳɹ�
		if ((status.st_mode & S_IFMT) == S_IFDIR) {//������ʵ���һ��Ŀ¼ DIR
			strcat_s(path, "/index.html");//ƴ��Ĭ���ļ�
		}

		serverce.server_file(client, path);
	}

	closesocket(client);
	return 0;
}

int main(void)//int����main��ʾ�������ͣ�������дvoid��ʾ����һ��Ϊ��
{
	Startup start;
	unsigned short port = 80;//����˿�,Ϊ0�Ͷ�̬����
	int server_socket = start.startup(&port);
	cout << "������׽���Ϊ��" << server_socket << endl;
	cout << "httpd���������������ڼ���" << port << "�˿�..." << endl;

	struct sockaddr_in client_addr;//�ͻ��˵�ַ
	int client_addr_len = sizeof(client_addr);

	while (1)
	{
		//����ʽ�ȴ��û�ͨ��������������
		//accept�ص�
		//������ͬ������������������ģ�û�пͻ������ӣ��Ǿ�һֱ����������š�
		//������ӣ�һ��ֻ��һ����5����Ҫ5��ѭ��
		int client_sock = accept(server_socket,
			(struct sockaddr*)&client_addr,
			&client_addr_len);//�ȴ����ʣ�����������һ���ͻ��˵��׽��֣����û���յ����ʣ����һֱ�ȴ�
		cout << "�ͻ����׽���Ϊ��" << client_sock << endl;
		if (client_sock < 0) error_die("accept");

		//�����µ��̣߳���������ɰ�������߳�
		//ʹ��client_sock���û����з���
		DWORD threadId = 0;//�߳�id
		CreateThread(0, 0,
			accept_request, //�̺߳���
			(void*)client_sock, //�̺߳����Ĳ���
			0, &threadId);

	}
	closesocket(server_socket);
	return 0;//������0��ʾ����ִ�гɹ�
}