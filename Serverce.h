#pragma once
#include"Exception.h"
#include<set>
using namespace std;

#define PRINTF(str) cout << "[" << __func__ << "-" << __LINE__ << "]" << "str=" << str<< endl;//�����ӡ��

class Serverce
{
public:

	//������ҳ
	void server_file(int client, const char* fileName);

	//��ָ���Ŀͻ����׽���sock����ȡһ�����ݣ����浽buff������ʵ�ʶ�ȡ�����ֽ���
	int get_line(int sock, char* buff, int size);

	//�ж��Ƿ����ı��ļ��ĺ���
	bool isTxt(const char* fileName);

	//�ж��ļ����ͣ�ѡ����ʵ�Content-type
	const char* getContent_type(const char* fileName);


};

