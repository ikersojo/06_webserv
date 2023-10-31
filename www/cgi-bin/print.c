#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("HTTP/1.1 200 OK\r\n");
	printf("Content-Type: text/html\r\n\r\n");
	printf("<html><head><title>This is a response from a C program</title></head>");
	printf("<body><h1>This is a response from a C program</h1>");
	(getenv("NAME")) ? printf("<h2>Hi %s!</h2>", getenv("NAME")) : printf("<h2>No name inserted</h2>");
	printf("</body></html>");
	return 0;
}
