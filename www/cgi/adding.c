#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Define boolean values
#define true 1
#define false 0
typedef int bool;

bool isNumber(const char *str)
{
	if (!*str)
		return false;
	while (*str)
	{
		if (!isdigit(*str))
			return false;
		str++;
	}
	return true;
}

int	main(int argc, char ** argv)
{
	if (argc == 3 && isNumber(argv[1]) && isNumber(argv[2]))
	{
		int a, b, res;

		a = atoi(argv[1]);
		b = atoi(argv[2]);
		res = a + b;
		printf("Successfull CGI Call: %d + %d = %d\n", a, b, res);
	}
	else
		printf ("Error: not valid arguments\n");
	return 0;
}
