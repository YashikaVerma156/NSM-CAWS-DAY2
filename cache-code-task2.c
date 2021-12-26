#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

#define LOGSIZE 23

int access[1L << LOGSIZE];
int main(int argc, char** argv)
{
	assert(argc == 2);
	int logsize = atoi(argv[1]);
	logsize -= 2;
	assert(logsize < LOGSIZE);
	printf("Building for array with logsize %d\n", logsize+2);
	register int size = 1L << logsize;
	for(int i = 0; i < 16; i++)
		access[i] = (i+15)%16;

	register int j = 0;
	
	while(1)
	{
		j = *(access+j);
	}
}
