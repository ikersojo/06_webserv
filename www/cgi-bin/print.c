#include <stdio.h>
#include <stdlib.h> 

int main(void)
{
	printf("HTTP/1.1 200 OK\r\n");
	printf("Content-Type: text/html\r\n\r\n");
	printf("<html><head><title>This is a response from a C program</title></head>");
	printf("<body><h1>This is a response from a C program</h1></body></html>");
	return 0;
}
