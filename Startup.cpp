#include "Startup.h"

//ʵ�������ʼ��
//�����׽���(�������˵��׽���)
//�˿ڣ�������port��ʾ�˿�
//			  ���*port��ֵΪ0����ô�Զ�������õĶ˿�
int Startup::startup(unsigned short* port)
{
	//1. ����ͨ�ų�ʼ����Linux����Ҫ��
	WSADATA data;
	int ret = WSAStartup(
		MAKEWORD(1, 1), &data);//1.1�汾��Э��
	if (ret) //ret != 0����ʼ��ʧ��
	{
		error_die("WSAStartup");
	}

	//�����׽���
	int server_socket = socket(PF_INET,//�׽�������
		SOCK_STREAM,//������
		IPPROTO_TCP);
	if (server_socket == -1)
	{
		//����ʧ��
		error_die("�׽���");
	}

	//���ö˿ڿɸ���
	int opt = 1;
	ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
	if (ret == -1) error_die("setsockopt");

	//���÷������˵������ַ
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;//����
	server_addr.sin_port = htons(*port);//�˿�
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//�����������͵�IP���ܽ���


	//���׽���
	ret = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret < 0) error_die("bind");

	//������������
	if (listen(server_socket, 5) < 0) error_die("listen");

	//��̬����˿�
	if (*port == 0)
	{
		int addrLen = sizeof(server_addr);
		if (getsockname(server_socket, (struct sockaddr*)&server_addr,
			&addrLen) == -1)
			error_die("getsockname");
		*port = server_addr.sin_port;//Ŀǰ����������˿�
	}

	return server_socket;
}