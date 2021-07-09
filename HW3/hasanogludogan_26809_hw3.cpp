#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>


using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size





void my_malloc(int thread_id, int size)
{
	node temp;
	temp.id = thread_id;
	temp.size = size;
	myqueue.push(temp);

}

void * server_function(void *)
{

	int index=0;

	 while(true)
    {	pthread_mutex_lock(&sharedLock);
        while(!myqueue.empty())
        {
            node tempNode = myqueue.front();
			myqueue.pop();
            if(tempNode.size < MEMORY_SIZE-index)
			{
				thread_message[tempNode.id] = index;
				index += tempNode.size;
			}
            else
			{	
				thread_message[tempNode.id] = -1;
			}

            sem_post(&semlist[tempNode.id]);
        }
		pthread_mutex_unlock(&sharedLock);
    
    }
 
    return 0;

}

void * thread_function(void * id) 
{
	int * idPointer = (int *) id;
	srand (time(NULL) + *idPointer);
	int size = rand() % MEMORY_SIZE/6 + 1;

	pthread_mutex_lock(&sharedLock);
	my_malloc(*idPointer,size);
	pthread_mutex_unlock(&sharedLock);

	sem_wait(&semlist[*idPointer]);

	if(thread_message[*idPointer] == -1)
	{	pthread_mutex_lock(&sharedLock);
		cout << "Thread " << *idPointer << ": No available memory\n" << endl;
		pthread_mutex_unlock(&sharedLock);
	}
	else
	{
		for (unsigned int i = 0; i < size; i++)
	      memory[thread_message[*idPointer]+i] = *idPointer +'0';
		
	}
	

	return NULL;
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
	cout << "Memory Dump: \n";

	for (unsigned int i = 0; i < MEMORY_SIZE; i++)
		cout << memory[i];

}

int main (int argc, char *argv[])
 {

 		int thread_id[NUM_THREADS];

		for (unsigned int i = 0; i < NUM_THREADS; i++) 
			thread_id[i] = i;


 	init();	// call init

	pthread_t threads[NUM_THREADS];

 	for(unsigned int i=0; i < NUM_THREADS; i++)
         pthread_create(&threads[i],NULL,thread_function,(void *) &thread_id[i]);



 	for (unsigned int i = 0; i < NUM_THREADS; i++) 
		pthread_join(threads[i], NULL);


 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
	
 }