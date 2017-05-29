/*
Implement a program that gets as arguments a file name followed by words. 
For each word, create a separate thread that counts its appearances in the given file.
Print out the sum of the appearances of all words.
for i in {1..1000}; do ./project3 example man ok; done
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int count=0;
FILE* fp;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* func (void* ptr)
{
	char* word = (char*) ptr;
	char x[1024];
	// we lock the mutex when a thread start to parse the file
	pthread_mutex_lock(&mutex1);
	
	// if the word is found, the counter is incremented
	while (fscanf(fp," %1023s", x) == 1)
		if (strcmp(x, word)==0)
			count++;
	// after each thread parses all the file, we reset the pointer to the beginning of file
	fseek(fp, 0, SEEK_SET);

	pthread_mutex_unlock(&mutex1);
	return NULL;
}

int main(int argc, char* argv[])
{
fp = fopen(argv[1], "r");
// after we open file, we verify if the file was successfully opened. if not we print a message
if (fp == NULL)
	{
		printf("\nNo such file!\n");
		return 0;
	}
pthread_t th[100];
int i;

// we create a thread for each word passed as parameter
for (i=0; i<argc-2; i++)
	{
		pthread_create(&th[i], NULL, func,(void*) argv[i+2]);
	}

for (i=0; i<argc-2; i++)
	pthread_join(th[i], NULL);

printf("Number of occurences: %d\n", count);

}
