#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void consumer(void);
static void manager(pid_t pid, int process, int opt);
static int managermenu(void);
static int menu(void);
static void producer(void);

char characters[50];
int vowels[5];
int channel[2];
int prod_running = 0;
int cons_running = 0;

int
main(void)
{
	int opt;
	pid_t prod_pid, cons_pid;

	for (int i = 0; i < 5; i++) {
		vowels[i] = 0;
	}

	pipe(channel);

	do {
		opt = menu();

		switch (opt) {
		/* create producer */
		case 1:
			if (prod_running < 1) {
				if ((prod_pid = fork()) < 0) {
					printf("Error creating the process\n\n");
				} else if (prod_pid == 0) {
					printf("Producer running on PID %d\n\n", getpid());
					producer();
				} else {
					prod_running = 1;
					sleep(1);
				}
			} else {
				printf("Producer alredy running\n\n");
			}
			break;

		/* create consumer */
		case 2:
			if (cons_running < 1) {
				if ((cons_pid = fork()) < 0) {
					printf("Error creating the process\n\n");
				} else if (cons_pid == 0) {
					printf("Consumer running on PID %d\n\n", getpid());
					consumer();
				} else {
					cons_running = 1;
					sleep(1);
				}
			} else {
				printf("Consumer alredy running\n\n");
			}
			break;

		/* stop and kill process */
		case 3: {
			int option = managermenu();

			if (option > 4 || option < 1) {
				printf("Invalid option\n\n");
			}
			else if (option != 4) {
				int pid;
				printf("\nWhat is the PID of the process?: ");
				scanf("%d", &pid);

				if (pid == prod_pid)
					manager(pid, 1, option);
				else if (pid == cons_pid)
					manager(pid, 2, option);
				else
					printf("That isn't the PID of any process\n\n");
			}
			else {
				printf("\n\n");
			}

			break;
		}

		/* exit */
		case 4:
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
	} while (opt != 4);

	return 0;
}

void
manager(pid_t pid, int process, int opt)
{
	switch (opt) {
	case 1:
		kill(pid, SIGSTOP);
		printf("Process stopped\n\n");
		break;

	case 2:
		kill(pid, SIGTERM);
		printf("Process killed\n\n");

		if (process == 1)
			prod_running = 0;
		else
			cons_running = 0;

		break;

	default:
		kill(pid, SIGCONT);
		printf("Process restarted\n\n");
		break;
	}
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

int
menu(void)
{
	int option;
	printf("Main Menu\n\n");
	printf("  1. Create producer\n");
	printf("  2. Create consumer\n");
	printf("  3. Manage process\n");
	printf("  4. Exit program\n");
	printf("What do you want to do?: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

void
consumer(void) {
	int number = 0;
	close(channel[1]);

	for (;;) {
		read(channel[0], &characters, sizeof(characters));
		char character = characters[number];

		switch (character) {
		case 'a':
			vowels[0]++;
			characters[number] = '#';
			write(channel[0], &vowels, sizeof(vowels));
			write(channel[0], &characters, sizeof(characters));
			break;
		case 'e':
			vowels[1]++;
			characters[number] = '#';
			write(channel[0], &vowels, sizeof(vowels));
			write(channel[0], &characters, sizeof(characters));
			break;
		case 'i':
			vowels[2]++;
			characters[number] = '#';
			write(channel[0], &vowels, sizeof(vowels));
			write(channel[0], &characters, sizeof(characters));
			break;
		case 'o':
			vowels[3]++;
			characters[number] = '#';
			write(channel[0], &vowels, sizeof(vowels));
			write(channel[0], &characters, sizeof(characters));
			break;
		case 'u':
			vowels[4]++;
			characters[number] = '#';
			write(channel[0], &vowels, sizeof(vowels));
			write(channel[0], &characters, sizeof(characters));
			break;
		default:
			break;
		}

		if (number < 49)
			number++;
		else
			number = 0;

		sleep(5);
	}
}

void
producer(void)
{
	int number = 0;
	close(channel[0]);

	for (;;) {
		char letter = (rand() % 26) + 97;
		characters[number] = letter;

		if (number < 49)
			number++;
		else
			number = 0;

		write(channel[1], &characters, sizeof(characters));
		sleep(5);
	}
}
