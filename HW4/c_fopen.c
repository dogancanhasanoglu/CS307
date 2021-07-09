#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{

	clock_t tStart = clock();

	FILE *filePointer;
	char file_name[15] = "loremipsum.txt";
	char my_Char;
	unsigned int count_of_A;
	

	filePointer = fopen(file_name,"r");

	while ((my_Char = fgetc(filePointer)) != EOF)
        {
			if(my_Char=='a')
				count_of_A++;
        }

	fclose(filePointer);
	printf("Number of occurences of character \'a\' : \n\%u\n",count_of_A);
	printf("\nTime taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}



