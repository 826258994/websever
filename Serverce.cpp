#include "Serverce.h"


//��ָ���Ŀͻ����׽���sock����ȡһ�����ݣ����浽buff������ʵ�ʶ�ȡ�����ֽ���
int Serverce::get_line(int sock, char* buff, int size)
{
	char c = 0;
	int i = 0;


	while (i < size - 1 && c != '\n') {
		int n = recv(sock, &c, 1, 0);//��������ж�һ��ɾһ��
		if (n > 0)
		{
			//����������س�����\n��Ҳ�п�����\r\n������\r\n�Ŀ������ҲҪ����
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);//�鿴��һ���ַ���ʲô������ȡ��Ҳ���ǲ�ɾ��������е�����
				if (n > 0 && c == '\n') { //��һ���ַ���\n��˵����\r\n�����Ҳ�ǻس�
					recv(sock, &c, 1, 0);//��ʽ��ȡ
				}
				else {
					//���\r���治��\n���ǿ��������ˣ��Ͱ������\n��������ɶ������
					c = '\n';
				}
			}
			buff[i++] = c;
		}
		else {
			break;
		}
	}
	buff[i] = 0;//�ַ�����������'\0'
	return i;
}

//������ҳ
void Serverce::server_file(int client, const char* fileName)
{
	Exception ecp;//�쳣����
	int numbchars = 1;//ע�������char�ͣ���Χ��-127~128����ĳһ�е��ַ��Ƚ϶࣬
	//��char������为�����ͽ����ˣ�ʣ������ݶ�����
	char buff[1024];

	//��ȡ�������ʣ������ݣ��������Ӱ������ķ�������ӵ��
	while (numbchars > 0 && strcmp(buff, "\n")) //���һ����"\n"�����buff��"\n"��Ⱦͽ���
	{
		numbchars = get_line(client, buff, sizeof(buff));
		PRINTF(buff);
	}

	//�ж��ļ����ͣ��������ַ�ʽ��
	FILE* resource;
	if (isTxt(fileName))//������ı����͵��ļ�
	{
		resource = fopen(fileName, "r");//���ı���ʽ���ļ�
	}
	else
	{
		//�����Զ����Ƹ�ʽ���ļ�
		resource = fopen(fileName, "rb");
	}

	if (resource == NULL) {
		ecp.not_find(client);
	}
	else
	{
		SendResource sd;
		//��ʽ����˼Դ���ͻ���

		//�ж������ļ������ͣ��Ա㷢����ȷ������ͷcontent-type
		cout << "Content-type���ͣ�" << getContent_type(fileName) << endl;
		sd.headers(client, getContent_type(fileName));

		//����������Դ��Ϣ
		sd.cat(client, resource);
		cout << "�ļ���Դ������ϣ�" << endl;
	}

	fclose(resource);
}

//�ж��Ƿ����ı��ļ��ĺ���
bool Serverce::isTxt(const char* fileName)
{
	set<string> textType{ "txt","doc","html","docx","md" };//��Ҫ���ı���ʽ�򿪵ĺ�׺��
	int i = 0;
	while (fileName[i] != '.') i++;
	string fileType;
	for (int j = i + 1; j < strlen(fileName); j++)
	{
		fileType += fileName[j];//fileType ����fileName�ĺ�׺��
	}

	return textType.find(fileType) != textType.end();
}

//�ж��ļ����ͣ�ѡ����ʵ�Content-type
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
			fileType += fileName[j];//fileType ����fileName�ĺ�׺��
		}
		if (fileType == "png") type = { "image/png" };

		else if (fileType == "jpg") type = { "image/jpeg" };

		else if (fileType == "css") type = { "text/css" };

		else if (fileType == "js") type = { "application/x-javascript" };
	}
	return type;
}

