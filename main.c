#include<stdio.h>
#include<string.h>
#include <unistd.h>


int main()
{

	FILE *f ;
	char buffer[1024];

	while(1)
	{
		f = fopen("/proc/stat", "r");
		fread(buffer, 1, 1024, f);
		printf("%s\n\n\n", buffer);
		fclose(f);
		usleep(1000);
	}
	printf("Hello\n");
	return 0;
}

