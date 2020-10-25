
#include <time.h>
#include <stdio.h>

int main()
{

	printf("Hello World\n");
	clock_t begin = clock();

	for(int i = 0 ; i < 100000; i++)
	{
		printf("%d", i);
		printf("\n");
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Execution time: %f miliseconds \n", time_spent);
}
