#include "SendResource.h"

//���Ͱ�����Ӧ����ͷ��Ϣ
void SendResource::headers(int client, const char* type)
{
	char buff[1024];

	strcpy_s(buff, "HTTP/1.1 200 OK\r\n");//Э��/�汾+�ո�+״̬��+�ո�+����+�س�����
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "Server: PibaoHttpd/0.1\r\n");//�ؼ���: �Զ�������/�Զ���汾��+�س�
	cout << "buff=" << buff << endl;
	send(client, buff, strlen(buff), 0);

	//strcpy_s(buff, "Content-type:text/html\r\n");//�ļ����͹ؼ���:��������+�س�
	//send(client, buff, strlen(buff), 0);
	char buf[1024];
	sprintf_s(buf, "Content-type: %s\r\n", type);
	cout << "���͵�����Ϊ��" << buf << endl;
	send(client, buf, strlen(buf), 0);

	strcpy_s(buff, "\r\n");//���Ϳ��б�ʾ����
	send(client, buff, strlen(buff), 0);

	cout << "��ͷ��Ϣ�������" << endl;
}

//������Դ
void SendResource::cat(int client, FILE* resource)
{
	int count = 0;
	char buff[4096];//���η�������������һ�η�̫���¶�ʧ

	while (1)
	{
		int ret = fread(buff, sizeof(char) // ÿ�ζ������ֽ�
			, sizeof(buff),//���������ֽ�
			resource);

		if (ret <= 0) break;

		send(client, buff, ret, 0);
		count += ret;
	}
	cout << "һ��������" << count << "bit�ֽڸ������" << endl;
}
