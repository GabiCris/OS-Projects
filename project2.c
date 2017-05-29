/*The wheel. The main process (A) will create a subprocess (B) that will create another subprocess (C).
Then the main process (A) will send the generate a random number (between 1000 and 2000) to process B. 
The process B will subtract 10 units and will send the number to process C. C will subtract 20 units and will
send the number to A. The A process will subtract 30 units and will send the number to B, again. And so on, 
until the values is less then zero. In this moment the game will stop. The winner is the process that established 
the negative number. 
The communication between all the processes are done using pipe channels.*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
int pipeAB[2], pipeBC[2], pipeAC[2], result[2];
//we create all necesary pipes
pipe(pipeAB); pipe(pipeBC); pipe(pipeAC); pipe(result);

int no0, f2;
// generate random number using time seed
srand(time(NULL));
no0 =1000+rand()%1000;
printf("Chosen random numbers is: %d\n", no0);

//we create the first process - B
int f1 = fork();

if (f1 == 0)
{
	//Inside process B we create the third process C
	f2 = fork();
	if (f2 == 0)
	{
		int no2;
		while (1)
		{
			//we read the number from the pipe coming from process B
			read(pipeBC[0], &no2, sizeof(int));
			//if the number from the pipe is already negative, exit the process
			if (no2 < 0)
			{
				write(pipeAC[1], &no2, sizeof(int));
				exit(2);
			}
			//if the newly computed value is negative, then signal that this is the 
			//winning process by adding value 2 to the result pipe
			else if (no2-20 < 0)
			{
				
				int var = 2;
				write(result[1], &var, sizeof(int));
				write(pipeAC[1], &no2, sizeof(int));
				exit(2);
			}
			//if number not negative and process not yet winner, update value
			// and pass it on to process A
			no2 -= 20;
			write(pipeAC[1], &no2, sizeof(int));
		}
	}
	//entering process B
	else
	{
		//Similar operations as process C
		int no1;
		while (1)
		{
			read(pipeAB[0], &no1, sizeof(int));
			if (no1 < 0)
			{
				write(pipeBC[1], &no1, sizeof(int));
				exit(1);
			}
			else if (no1-10 < 0)
			{
				
				int var = 1;
				write(result[1], &var, sizeof(int));
				write(pipeBC[1], &no1, sizeof(int));
				exit(2);
			}
			no1 -= 10;
			write(pipeBC[1], &no1, sizeof(int));
		}
	}
}
// entering process A - main
else
{
	// insert the initial value in the pipe in order to start loop
	write(pipeAC[1], &no0, sizeof(int));
	while (1)
	{
		read(pipeAC[0], &no0, sizeof(int));
		//in both cases below we break the while loop
		// due to the fact that the value is now negative
		if (no0 < 0)
		{
			write(pipeAB[1], &no0, sizeof(int));
			break;
		}
		else if (no0-30 < 0)
		{
			int var = 0;
			write(result[1], &var, sizeof(int));
			write(pipeAB[1], &no0, sizeof(int));
			break;
			
		}
		//update value and pass it on to process B
		no0 -= 30;
		write(pipeAB[1], &no0, sizeof(int));
	}
}

// get result from result pipe and print the corresponding message
int res;
read(result[0], &res, sizeof(int));
if (res == 0)
	printf("Process A wins!\n");
if (res == 1)
	printf("Process B wins!\n");
if (res == 2)
	printf("Process C wins!\n");

return 0;
}
