#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
							//gloabal variables for:
bool isPlaneFull = false;   //plane being full or nor
bool flag = false;			// exit flag
int turn = 0;				// thread turn


vector<vector<int> > seatMatrix(2, vector<int>(50, 0));  // vector declaration


void* travelAgency_1(void* param )  // first thread function
{
	int* paramPointer = (int*)param;
	while (!isPlaneFull)   //termination condition
	{

		srand(time(NULL));

		while (turn == 0 && !isPlaneFull)  // do not get in unless your turn
		{
			int row, column;
			row = rand() % 2;
			column = rand() % 50;        // generation phase
			bool reserved = false;
			if (!seatMatrix[row][column])
			{
				printf("Agency 1 Entered Critical Region \n");
				printf("Seat Number %d is reserved by Agency 1 \n", row * 50 + column + 1);

				seatMatrix[row][column] = *paramPointer;     // passing our variable to that seat
				reserved = true;
				printf("Agency 1 Exit Critical Region \n\n");

			}
			if (reserved)   // done with reservation pass the turn
				turn = 1;



		}




	}
	if (isPlaneFull && !flag)
	{
		printf("Agency 1 Entered Critical Region \n");
		printf("Agency 1 Exit Critical Region \n");
		printf("No Seats Left \n\n"); flag = true;
	}
	return 0;

}

void* travelAgency_2(void* param)    // same for thread 2
{

	int* paramPointer = (int*)param;
	while (!isPlaneFull)
	{

		srand(time(NULL));

		while (turn == 1 && !isPlaneFull)
		{
			int row, column;
			row = rand() % 2;
			column = rand() % 50;
			bool reserved = false;
			if (!seatMatrix[row][column])
			{
				printf("Agency 2 Entered Critical Region \n");
				printf("Seat Number %d is reserved by Agency 2 \n", row * 50 + column + 1);

				seatMatrix[row][column] = *paramPointer;
				reserved = true;
				printf("Agency 2 Exit Critical Region \n\n");

			}
			if (reserved)
				turn = 0;


		}


	}

	return 0;

}
int main()
{

	pthread_t thread1, thread2;
	int travelAgency1 = 1, travelAgency2 = 2;              // thread declaration and creation
	pthread_create(&thread1, NULL, travelAgency_1, (void*) & travelAgency1);
	pthread_create(&thread2, NULL, travelAgency_2, (void*) & travelAgency2);

	int counter = 0;
	while (!isPlaneFull)    // making sure untill plane is full and all seats are reserved
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				if (seatMatrix[i][j])
					counter++;
			}
		}
		if (counter == 100)
			isPlaneFull = true;
		else
			counter = 0;
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Plane is full : \n");     // after all printing out the plane seat plan
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			printf(" %d ", seatMatrix[i][j]);
		}
		printf("\n");
	}



	return 0;
}
