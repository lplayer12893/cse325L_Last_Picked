/*
 * filename: shell.c
 * description: This is an implementation of a shell in C
 *  capable of running in interactive or batch mode.
 * authors: Salinas, Sean
 *          Stuyvesant, Lucas
 * class: CSE 325 S15
 * instructor: Zheng
 * assignment: Project #2
 * assigned: January 28th, 2015
 * due: February 11th, 2015
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

char * getString();

int main(int argc, char ** argv)
{
	// TODO: Look at number of arguments, if more than one, batch mode, more than 2, error
	// TODO: If batch file doesn't exist, error out
	
	// TODO: Use a dynamic array for argument array
	char * args[80];
	int keep_running = 1;

	char * input = NULL;
	/* char * prog_to_run; */
	// int i;
	int argnum;
	
	// Interactive Mode
	while (keep_running)
	{
		/* Display prompt and flush stdout*/
		printf("TeamLastPicked > ");
		fflush(stdout);

		/* Read input from user */

		input = getString();

		/* Strip the newline character.
		 This isn't needed anymore, since getString does it for us
		for (i = 0; i < MAX_LINE; i++)
		{
			if (input[i] == '\n')
			{
				input[i] = '\0';
				break;
			}
		}
		*/

		/* Sometimes people enter nothing. We don't want to try to parse it. */

		// TODO: Tokenize on the semicolon
		// Repeat this loop for every command separated by semicolon, but they should not include the wait command, IE they should run concurrently (except quit)
		if (input[0] != 0)
		{
			// int i = 0;
			argnum = 0;
			/* Tokenize using the space as the token */
			while (1)
			{
				if (argnum == 0)
					args[argnum] = strtok(input, " ");
				else
					args[argnum] = strtok(NULL, " ");
				if (args[argnum] == NULL)
					break;
				argnum++;
			}

			/* Check for exit */
			// FIXME: Change this to quit
			// TODO: Check for end of string, should also force quit (Ctrl-D or end of batch argument)
			if (strcmp(args[0], "exit") == 0)
			{
				/* Received Exit command. */
				printf("Goodbye!\n");
				keep_running = 0;
			}
			else
			{
				/* Not exit command. Run as normal. */
				int pid;
				// i = 1;

				/* For debugging
				printf("Going to run %s", args[0]);
				if (argnum > 1)
				{
					printf(" with options: \n");
					for (i = 1; i < argnum; i++)
						printf("\t%s\n", args[i]);
				}
				else
					printf("\n");
				*/

				pid = fork();
				if (pid == 0)
				{
					/* Child */
					execvp(args[0], args);
					/* execvp() only returns on an error. It terminates the child
					 * if everything went ok.
					 */

					perror("Error");
					exit(1);
				}
				else
				{
					/* Parent */
					/* Check if we are running it as a job. */
					if (strcmp(args[argnum - 1], "&") != 0)
					{
						/* Not running as job. */
						int status;
						waitpid(pid, &status, 0);
					}
					/* for debugging
					else
					{
						printf("Running as a job!");
					}
					*/
				}
			}
		}
		free(input);
		input = NULL;
	}
	return 0;
}


/**
 * This is a function that gets a string of infinite length from stdin. This string should be freed after use.
 * 
 * @return pointer to new string.
 */
char * getString()
{
	char * str = malloc(sizeof(char));
	unsigned int len = 0;
	char c = 0x0;
	if (str == NULL)
	{
		perror("Cannot malloc");
		return NULL;
	}
	while ((c = getchar()) != '\n')
	{
		str[len] = c;
		len++;
		str = realloc(str,(len+1)*sizeof(char));
	}
	str[len] = 0x0;
	return str;
}
