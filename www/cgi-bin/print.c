#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char * operator;

	operator = getenv("OPERATOR");
	printf("HTTP/1.1 200 OK\r\n");
	printf("Content-Type: text/plain\r\n\r\n");
	if (operator[0] == 'a')
		printf("%.2f", atof(getenv("NUM1"))+atof(getenv("NUM2")));
	if (operator[0] == 's')
		printf("%.2f", atof(getenv("NUM1"))-atof(getenv("NUM2")));
	if (operator[0] == 'm')
		printf("%.2f", atof(getenv("NUM1"))*atof(getenv("NUM2")));
	if (operator[0] == 'd')
		printf("%.2f", atof(getenv("NUM1"))/atof(getenv("NUM2")));
	return 0;
}
