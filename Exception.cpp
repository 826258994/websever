#include "Exception.h"

void Exception::not_find(int client)
{
	//�����������notfind����ҳ
	char path[] = "Webresource/not_find.html";
	FILE* resource = fopen(path, "r");

	//����ͷ
	char buff[1024];

	strcpy_s(buff, "HTTP/1.1 404 NOT FOUND\r\n");//������Ӧ404
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "Server: PibaoHttpd/0.1\r\n");//�ؼ���: �Զ�������/�Զ���汾��+�س�
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "Content-type:text/html\r\n");//�ļ����͹ؼ���:��������+�س�
	send(client, buff, strlen(buff), 0);

	strcpy_s(buff, "\r\n");//���Ϳ��б�ʾ����
	send(client, buff, strlen(buff), 0);

	SendResource sd;
	//����������Դ��Ϣ
	sd.cat(client, resource);
	cout << "������ʾҳ�淢�����" << endl;
}

//������ʾҳ�溯��
void Exception::unimplement(int client)
{
	//��ָ�����׽��֣�������ʾ��û��ʵ�ֵĴ���ҳ��

}