#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

static void consumer(void);
static void manager(pid_t pid, int process, int opt);
static int managermenu(void);
static int processmenu(void);
static int menu(void);
static void producer(void);
static void printCharArray(void);
static void printVowelArray(void);
static void printInfo(void);

//Child variables
char characters[50] = {0,};
int vowels[5] = {0,};

//buffer
char buffChar[50] = {0,};
int buffVow[5] = {0,};

//Pipe channels
int channel_ProdCons[2];
int channel_ConsProd[2];
int channel_PrntChld1[2];
int channel_PrntChld2[2];

pid_t prod_pid, cons_pid;

int prod_running = 0;
int cons_running = 0;

int
main(void)
{
	int opt;

	pipe(channel_ProdCons);
	pipe(channel_ConsProd);
	pipe(channel_PrntChld1);
	pipe(channel_PrntChld2);

	do 
	{
		opt = menu();

		switch (opt) 
		{
		
		/* create producer */
		case 1:
		
			if (prod_running < 1) 
			{
				if ((prod_pid = fork()) < 0) 
				{
					printf("Error creating the process\n\n");
				} 
				else if (prod_pid == 0) //Child
				{
					printf("Producer running on PID %d\n\n", getpid());
					producer();
				} 
				else 
				{
					//Close first pipe
					close(channel_ProdCons[1]);
					close(channel_ProdCons[0]);
					//Close second pipe
					close(channel_ConsProd[1]);
					close(channel_ConsProd[0]);
					//Close write end of parent pipe
					close(channel_PrntChld1[1]);
					close(channel_PrntChld2[1]);
					
					prod_running = 1;
					sleep(1);
				}
			} 
			else 
			{
				printf("Producer alredy running\n\n");
			}
			break;

		/* create consumer */
		case 2:
			if (cons_running < 1) 
			{
				if ((cons_pid = fork()) < 0) 
				{
					printf("Error creating the process\n\n");
				} 
				else if (cons_pid == 0) //Child 
				{
					printf("Consumer running on PID %d\n\n", getpid());
					consumer();
				} 
				else 
				{
					//Close first pipe
					close(channel_ProdCons[1]);
					close(channel_ProdCons[0]);
					//Close second pipe
					close(channel_ConsProd[1]);
					close(channel_ConsProd[0]);
					//Close write end of parent pipe
					close(channel_PrntChld1[1]);
					close(channel_PrntChld2[1]);
					
					cons_running = 1;
					sleep(1);
				}
			} 
			else 
			{
				printf("Consumer alredy running\n\n");
			}
			break;

		/* stop and kill process */
		case 3: ;
		
			//Select consumer or producer
			int pid;
			int process = processmenu();
				
			if (process == 1)
			{
				pid = prod_pid;
			}
			else if (process == 2)
			{
				pid = cons_pid;
			}
			else
			{
				printf("Invalid option\n\n");
				break;
			}
			
			//Select action
			int option = managermenu();
			
			if (option > 4 || option < 1) 
			{
				printf("Invalid option\n\n");
			}
			else if (option != 4) 
			{
				manager(pid, process, option);
			}
			else 
			{
				printf("\n\n");
			}

			break;
			
		/* Print Info */
		case 4:
			printInfo();
			break;
			
		/* exit */
		case 5:
		
			if (prod_running >= 1)
				kill(prod_pid, SIGKILL);

			if (cons_running >= 1)
				kill(cons_pid, SIGKILL);
			break;

		/* invalid option */
		default:

			printf("Invalid option\n\n");
			break;
		
		}
	} 
	while (opt != 5);

	return 0;
}

int
menu(void)
{
	//system("clear");
	int option;
	printf("Main Menu\n\n");
	printf("  1. Create producer\n");
	printf("  2. Create consumer\n");
	printf("  3. Manage process\n");
	printf("  4. Print Info\n");
	printf("  5. Exit program\n");
	printf("What do you want to do?: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

int
processmenu(void)
{
	int option;
	printf(" Process\n\n");
	printf("  1. Producer\n");
	printf("  2. Consumer\n");
	printf("Select a Process: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

int
managermenu(void)
{
	printf("Manager Menu\n\n");
	printf("  1. Stop process\n");
	printf("  2. Kill process\n");
	printf("  3. Restart process\n");
	printf("  4. Nothing\n");
	printf("What do you want to do?: ");
	int option;
	scanf("%d", &option);
	return option;
}

void
manager(pid_t pid, int process, int opt)
{
	
	if(pid != 0)
	{
		switch (opt) 
		{
		
		//Stop process
		case 1:
			kill(pid, SIGSTOP);
			printf("Process stopped\n\n");
			break;
			
		//Kill process
		case 2:
			kill(pid, SIGTERM);
			printf("Process killed\n\n");

			if (process == 1)
				prod_running = 0;
			else
				cons_running = 0;
			break;
			
		//Restart process
		case 3:
			kill(pid, SIGCONT);
			printf("Process restarted\n\n");
			break;
		

		default:
			break;
		}
	}
	else
	{
	printf("Process does not exist\n\n");
	}
}

void printInfo(void)
{
	printf("\nProcess info:\n");
			
	if(prod_running)
	{
		printf("\nProducer PID: %d\n", prod_pid);		
		read(channel_PrntChld1[0], characters, sizeof(characters));
		printCharArray();
	}
	else
	{
		printf("\nProducer not running...\n");
	}
	if(cons_running)
	{
		printf("\nConsumer PID: %d\n", cons_pid);
		read(channel_PrntChld2[0], vowels, sizeof(vowels));
		printVowelArray();
	}
	else
	{
		printf("\nConsumer not running...\n");
	}
	printf("\n\n");
}


//Child process
void
consumer(void) 
{
	int number = 0;
	
	//Close write end of first pipe
	close(channel_ProdCons[1]);
	//Close read end of second pipe
	close(channel_ConsProd[0]);
	//Close read end of parent pipe
	close(channel_PrntChld1[0]);
	close(channel_PrntChld1[1]);
	close(channel_PrntChld2[0]);
	
	for (;;) 
	{
		char character = buffChar[number];

		for(number = 0; number < 50; number++)
		{
			switch (character) 
			{
			
			case 'a':
				vowels[0]++;
				buffChar[number] = '#';
				break;
			case 'e':
				vowels[1]++;
				buffChar[number] = '#';
				break;
			case 'i':
				vowels[2]++;
				buffChar[number] = '#';
				break;
			case 'o':
				vowels[3]++;
				buffChar[number] = '#';
				break;
			case 'u':
				vowels[4]++;
				buffChar[number] = '#';
				break;
			default:
				break;
		
			}
		}
		write(channel_PrntChld2[1], vowels, sizeof(vowels));
		write(channel_ConsProd[1], buffChar, sizeof(buffChar));

		read(channel_ProdCons[0], buffChar, sizeof(buffChar));
			
		number = 0;
		sleep(5);
	}
}

//Child Process
void
producer(void)
{
	//Close write end of first pipe
	close(channel_ProdCons[1]);
	//Close read end of second pipe
	close(channel_ConsProd[0]);
	//Close read end of parent pipe
	close(channel_PrntChld1[0]);
	close(channel_PrntChld2[1]);
	close(channel_PrntChld2[0]);

	int number = 0;

	for (;;) 
	{
		srand(time(NULL));
		char letter = (rand() % 26) + 97;
		characters[number] = 'a';

		if (number == 50)
		{
			number = 0;
		}
		else
		{
			number++;
		}
		write(channel_ProdCons[1], characters, sizeof(characters));
		write(channel_PrntChld1[1], characters, sizeof(characters));
		
		sleep(5);
		read(channel_ConsProd[0], characters, sizeof(characters));
	}
}

void 
printCharArray(void)
{
	printf("\nCharacters:\n");
	
	for(int i = 0; i < 50; i++)
	{
		if(characters[i] == '\0')
		{
			printf("- ");
		}
		else
		{
			printf("%c ", characters[i]);
		}
	}
}

void 
printVowelArray(void)
{
	printf("\nVowels:\n");
	printf("a: %d \n", vowels[0]);
	printf("e: %d \n", vowels[1]);
	printf("i: %d \n", vowels[2]);
	printf("o: %d \n", vowels[3]);
	printf("u: %d \n", vowels[4]);
}

