#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(void)
{   
	clock_t tStart = clock();
	int fd = open ("loremipsum.txt", O_RDONLY);
	struct stat s;
    size_t size;
    int status = fstat(fd, &s);
    size = s.st_size;

	unsigned int count_of_A;
	
	char *ptr = mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t i=0;
	for(; i<size; i++)
	{
		if(ptr[i]=='a')
			count_of_A++;
	
	}
	
	
	close(fd);

	printf("Number of occurences of character \'a\' : \n\%u\n",count_of_A);
	printf("\nTime taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	status= munmap(ptr,size);
    return 0;
}


