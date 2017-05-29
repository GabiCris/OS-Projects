#include <stdio.h>
#include <time.h>
#include <pthread.h>

int sum=0;

typedef struct _bounds
{
	int left;
	int right;
} bounds, *pbounds;

void* computeSum(void* ptr)
{
	bounds* b=(bounds*) ptr;
	int i;
	for (i= b->left; i<=b->right; i++)
		sum+=i;
	return NULL;
}

int main()
{
//pthread_mutex_t
pthread_t threads[12];
bounds bList[11];
int start=1, i;
for (i=0; i<10; i++)
{
	bList[i].left = start;
	start +=99;
	bList[i].right = start;
	start++;
}

for (i=0; i<10; i++)
	pthread_create(&threads[i], NULL, computeSum, (void*)&bList[i]);

for (i=0; i<10; i++)
	pthread_join(threads[i], NULL);
printf("Sum: %d\n", sum);
return 0;
}
