#include"error_die.h"

void error_die(const char* str) {
	perror(str);//打印错误原因和字符
	exit(1);
}