#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int ms_min(int x, int y)
{
	return (x < y) ? x : y;
}

int ms_max (int x, int y)
{
	return - ms_min (-x, -y);
}

int ms_get_char()
{
	char buffer[3];
	fgets(buffer, 3, stdin);
	if (buffer[strlen(buffer) - 1] != '\n')
		while(getchar() != '\n');
	return buffer[0];
}

int ms_get_int()
{
	char buffer[4];
	fgets(buffer, 4, stdin);
	printf("%s", buffer);
	if (buffer[strlen(buffer) - 1] != '\n')
		while(getchar() != '\n');
	int result = atoi(buffer);
	return result;
}

