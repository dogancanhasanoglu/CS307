#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;



int main()
{
	clock_t tStart = clock();
	ifstream loremipsum("loremipsum.txt");
	char myChar;
	unsigned int count_of_A;
	while(!loremipsum.eof())
	{
		loremipsum>>myChar;
		if(myChar=='a')
			count_of_A++;
	}
	
	loremipsum.close();

	cout<<"Number of occurences of character \'a\' :"<<endl<<count_of_A<<endl<<endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;

}