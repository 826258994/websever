#include"error_die.h"

void error_die(const char* str) {
	perror(str);//��ӡ����ԭ����ַ�
	exit(1);
}