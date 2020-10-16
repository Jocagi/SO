#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* macros */
#define RUNNING (1)
#define STOPPED (0)
#define KILLED (-1)

/* type definitions */
typedef struct
{
	int id;
	size_t size;
} Shm;

/* function declarations */
static void consumer(Shm *shmc, Shm *shmv);
static void manageprocess(pid_t prod, pid_t cons, int *statep, int *statec);
static int menu(void);
static int menumanager(void);
static int processmenu(pid_t prod, pid_t cons);
static void producer(Shm *shm);
static void shmdel(Shm *shm);
static Shm *shmnew(size_t size);
static void shmread(void *data, Shm *shm);
static void shmwrite(Shm *shm, void *data);
static void showinfo(Shm *shmc, Shm *shmv);

/* global variables */
static char characters[50];
static int vowels[5] = {0, 0, 0, 0, 0};

/* function implementations */
void
consumer(Shm *shmc, Shm *shmv) {
	int number = 0;

	for (;;) {
		shmread(&characters, shmc);
		shmread(&vowels, shmv);
		char character = characters[number];

		switch (character) {
		case 'a':
			vowels[0] = vowels[0] + 1;
			characters[number] = '#';
			break;
		case 'e':
			vowels[1] = vowels[1] + 1;
			characters[number] = '#';
			break;
		case 'i':
			vowels[2] = vowels[2] + 1;
			characters[number] = '#';
			break;
		case 'o':
			vowels[3] = vowels[3] + 1;
			characters[number] = '#';
			break;
		case 'u':
			vowels[4] = vowels[4] + 1;
			characters[number] = '#';
			break;
		default:
			break;
		}

		if (number < 49)
			number++;
		else
			number = 0;

		shmwrite(shmc, &characters);
		shmwrite(shmv, &vowels);
		sleep(5);
	}
}

void
manageprocess(pid_t prod, pid_t cons, int *statep, int *statec)
{
	int process;
	int option = menumanager();

	if (option >= 1 && option <= 3) {
		process = processmenu(prod, cons);

		switch(option) {
			case 1:
				if (process == 1) {
					kill(prod, SIGSTOP);
					*statep = STOPPED;
				} else if (process == 2) {
					kill(cons, SIGSTOP);
					*statec = STOPPED;
				}
				printf("Process stopped\n\n");
				break;
			case 2:
				if (process == 1) {
					kill(prod, SIGTERM);
					*statep = KILLED;
				} else if (process == 2) {
					kill(cons, SIGTERM);
					*statec = KILLED;
				}
				printf("Process killed\n\n");
				break;
			case 3:
				if (process == 1) {
					kill(prod, SIGCONT);
					*statep = RUNNING;
				} else if (process == 2) {
					kill(cons, SIGCONT);
					*statec = RUNNING;
				}
				printf("Process restarted\n\n");
				break;
			case 4:
				break;
			default:
				printf("Invalid option\n\n");
				break;
		}
	} else if (option != 4) {
		printf("Invalid option\n\n");
	}
}

int
menu(void)
{
	int option;
	printf("Main Menu\n\n");
	printf("  1. Create producer\n");
	printf("  2. Create consumer\n");
	printf("  3. Manage process\n");
	printf("  4. Show process info\n");
	printf("  5. Exit program\n");
	printf("What do you want to do?: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

int
menumanager(void)
{
	int option;
	printf("Manager Menu\n\n");
	printf("  1. Stop process\n");
	printf("  2. Kill process\n");
	printf("  3. Restart process\n");
	printf("  4. Nothing\n");
	printf("What do you want to do?: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

int
processmenu(pid_t prod, pid_t cons)
{
	int option;
	printf("Process Menu\n\n");
	printf("  1. Producer (PID %d)\n", prod);
	printf("  2. Consumer (PID %d)\n", cons);
	printf("  3. None\n");
	printf("What process do you want to use?: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

void
producer(Shm *shm)
{
	int number = 0;
	for (;;) {
		shmread(&characters, shm);
		srand(time(NULL));
		char letter = (rand() % 26) + 97;
		characters[number] = letter;

		if (number < 49)
			number++;
		else
			number = 0;

		shmwrite(shm, &characters);
		sleep(5);
	}
}

void
shmdel(Shm *shm) {
	shmctl(shm->id, IPC_RMID, 0);
	free(shm);
}

Shm
*shmnew(size_t size)
{
	Shm *shm = calloc(1, sizeof *shm);
	shm->size = size;

	if ((shm->id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) < 0) {
		perror("shmget");
		free(shm);
		return NULL;
	}

	return shm;
}

void
shmread(void *data, Shm *shm)
{
	void *shm_data;

	if ((shm_data = shmat(shm->id, NULL, 0)) == (void *) -1) {
		perror("read");
		return;

	}
	memcpy(data, shm_data, shm->size);
	shmdt(shm_data);
}

void
shmwrite(Shm *shm, void *data)
{
	void *shm_data;

	if ((shm_data = shmat(shm->id, NULL, 0)) == (void *) -1) {
		perror("write");
		return;
	}

	memcpy(shm_data, data, shm->size);
	shmdt(shm_data);
}

void
showinfo(Shm *shmc, Shm *shmv)
{
	int i;
	shmread(&characters, shmc);
	shmread(&vowels, shmv);

	for (i = 0; i < 50; i++)
		printf("Characters array in position %d = %c\n", i, characters[i]);
	printf("\nVowels array (a) = %d\n", vowels[0]);
	printf("Vowels array (e) = %d\n", vowels[1]);
	printf("Vowels array (i) = %d\n", vowels[2]);
	printf("Vowels array (o) = %d\n", vowels[3]);
	printf("Vowels array (u) = %d\n\n", vowels[4]);
}

int
main(void)
{
	int statep = KILLED;
	int statec = KILLED;
	Shm *shmc = shmnew(sizeof(characters));
	Shm *shmv = shmnew(sizeof(vowels));
	pid_t prod, cons, info;
	int opt;

	do {
		opt = menu();

		switch (opt) {
		case 1:
			if (statep == KILLED) {
				if ((prod = fork()) < 0) {
					printf("Error creating the process\n\n");
				} else if (prod  == 0) {
					printf("Producer running on PID %d\n\n", getpid());
					producer(shmc);
				} else {
					statep = RUNNING;
					sleep(1);
				}
			} else {
				printf("Producer alredy running\n\n");
			}
			break;
		case 2:
			if (statec == KILLED) {
				if ((cons = fork()) < 0) {
					printf("Error creating the process\n\n");
				} else if (cons  == 0) {
					printf("Consumer running on PID %d\n\n", getpid());
					consumer(shmc, shmv);
				} else {
					statec = RUNNING;
					sleep(1);
				}
			} else {
				printf("Consumer alredy running\n\n");
			}
			break;
		case 3:
			manageprocess(prod, cons, &statep, &statec);
			break;
		case 4:
			if ((info = fork()) < 0) {
				printf("Error creating the process\n\n");
			} else if (info == 0) {
				showinfo(shmc, shmv);
				exit(0);
			} else {
				int status;
				waitpid(info, &status, 0);

				if (status != 0)
					printf("Error closing the info process\n\n");
			}
			break;
		case 5:
			kill(prod, SIGTERM);
			kill(cons, SIGTERM);
			shmdel(shmc);
			shmdel(shmv);
			break;
		default:
			printf("Invalid option\n\n");
			break;
		}
	} while (opt != 5);

	return 0;
}
