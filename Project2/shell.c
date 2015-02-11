/*
 * filename: shell.c
 * description: This is an implementation of a shell in C
 *  capable of running in interactive or batch mode.
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #2
 * assigned: January 28th, 2015
 * due: February 11th, 2015
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#include "getStrings.h"

#define WHITESPACE 0x0
#define CHARACTER 0x1

#define BATCH 0x0
#define INTERACTIVE 0x1

char * getString();
char * getStringFromFile(FILE *);
char ** getCommands(char *);
char ** splitCommandAndArgs(char *, int *);

int main(int argc, char ** argv)
{
	char opMode = 0x0;
	char ** args = NULL;
	int keep_running = 1;
	int i = 0, j=0;
	char * input = NULL;
	char ** cmds = NULL;
	int argnum = 0x0;
	FILE * batchFile = NULL;

	// First, check for interactive or batch mode
	if (argc == 1)
	{
		// Interactive mode
		opMode = INTERACTIVE;
		printf("Welcome to interactive mode. Type 'quit' to quit.\n");
	}
	else if (argc == 2)
	{
		// Batch Mode
		opMode = BATCH;
		// Open file
		batchFile = fopen(argv[1], "r");
		// Check for errors
		if (batchFile == NULL)
		{
			// Problem with file specified. Error out
			perror("Couldn't open batchfile");
			return 1;
		}
	}
	else
	{
		// Too many arguments
		fprintf(stderr,"Error: Too many arguments\nUsage: %s [batchfile]\n\tNote: [batchfile] is optional.\n",argv[0]);
		return 1;
	}

	while (keep_running)
	{
		if (opMode == INTERACTIVE)
		{
			// Display prompt and flush stdout
			printf("TeamLastPicked > ");
			fflush(stdout);
			// Read input from user
			input = getString();
		}
		else
		{
			// Batch Mode
			// Read a line from the file
			input = getStringFromFile(batchFile);
			if (input == NULL)
				break;
		}

		// Tokenize on the semicolon
		cmds = getCommands(input);

		i = 0;
		while(cmds[i] != NULL)
        {
				// printf("cmds[%d]: (%p) >%s<\n",i,cmds[i], cmds[i]);
            // Why if(input[0] != 0)
            argnum = 0;
				printf("calling splitCommandAndArgs with cmds[%d], address %p\n",i,cmds[i]);
            args = splitCommandAndArgs(cmds[i], &argnum);

            // Will exit when cmds is exhausted
				if ((args == NULL) || (argnum == 0))
				{
				}
				else if(strcmp(args[0], "quit") == 0)
            {
                keep_running = 0;
            }
            else
            {
                /**
                 * PSEUDOCODE:
                 * fork()
                 * Loop start{
                 * assume & is not an argument
                 * Child:
                 *      execvp(args[0],args)
                 *      return on error
                 * Parent:
                 *      repeat loop
                 * Loop end}
                 * wait for all pid's
                 *
                 *
                 *
                 */
				/* Not exit command. Run as normal. */
				int pid;

				// For debugging
				printf("Going to run %s", args[0]);
				if (argnum > 1)
				{
					printf(" with options: \n");
					for (j = 1; j < argnum; j++)
						printf("\t%s\n", args[j]);
				}
				else
					printf("\n");


				pid = fork();
				if (pid == 0)
				{
					/* Child */
					printf("Calling execvp(%p, %p)\n",args[0],args);
					execvp(args[0], args);
					/* execvp() only returns on an error. It terminates the child
					 * if everything went ok.
					 */

					perror("Error running execvp");
					exit(1);
				}
				else
				{
					/* Parent */
					/* Check if we are running it as a job. */
					if (strcmp(args[argnum - 1], "&") != 0)
					{
						/* Not running as job. */
						// TODO: When we are running with semicolons, we shouldn't call this for each one.
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
			i++;
			free(args);
			args = NULL;
        }

        // Wait for children to exit. PARENT ONLY
        int status;
        pid_t pid;
        while (i > 0)
        {
            pid = wait(&status);
				if (pid == -1)
				{
					perror("wait");
				}
            printf("PID %i returned\n",pid);
            i--;
        }
/**
		// Repeat this loop for every command separated by semicolon, but they should not include the wait command, IE they should run concurrently (except quit)
		if (input[0] != 0)
		{
			// int i = 0;
			argnum = 0;
			args = splitCommandAndArgs(input, &argnum);

			Check for quit *
			// TODO: Check for end of string, should also force quit (Ctrl-D or end of batch argument)
			if (strcmp(args[0], "quit") == 0)
			{
				Received quit command. *
				printf("Goodbye!\n");
				keep_running = 0;
			}
			else
			{
				Not exit command. Run as normal. *
				int pid;

				For debugging
				printf("Going to run %s", args[0]);
				if (argnum > 1)
				{
					printf(" with options: \n");
					for (i = 1; i < argnum; i++)
						printf("\t%s\n", args[i]);
				}
				else
					printf("\n");
				*

				pid = fork();
				if (pid == 0)
				{
					Child *
					execvp(args[0], args);
                    execvp() only returns on an error. It terminates the child
					 * if everything went ok.
					 *

					perror("Error");
					exit(1);
				}
				else
				{
					Parent *
					Check if we are running it as a job. *
					if (strcmp(args[argnum - 1], "&") != 0)
					{
						Not running as job. *
						// TODO: When we are running with semicolons, we shouldn't call this for each one.
						int status;
						waitpid(pid, &status, 0);
					}
					for debugging
					else
					{
						printf("Running as a job!");
					}
					*
				}
			}
			free(args);
			args = NULL;
		}

		free(input);
		input = NULL;
**/
		free(cmds);
		cmds = NULL;
	}

	printf("Goodbye!\n");

	if (opMode == BATCH)
	{
		// Close batch file
		fclose(batchFile);
	}
	return 0;
}


/**
 * Returns a list of commands (strings) represented in a single string
 *
 * @param input input string to parse
 *
 * @return pointer to a list of commands
 */
char ** getCommands(char *input)
{
    int size = strlen(input);
    int i = 0,count = 1;
    char *string = malloc(strlen(input) * sizeof(char));
	if (string == NULL)
	{
		perror("Couldn't malloc");
		return NULL;
	}

    for(; i < size; i++)
    {
        if(input[i] == ';')
        {
            count++;
        }
    }

	char **list = malloc((1 + count) * sizeof(char *));

    string = strtok(input,";");

    for(i = 0; string != NULL; i++)
    {
        list[i] = string;
        string = strtok(NULL,";");
    }

    list[i] = NULL;
    return list;
}

/**
 * This is a function that will split a string of commands into a single command and an array of arguments.
 *
 * @param line line to parse
 * @param numberArgs pointer to integer, will be overwritten with the number of arguments we parsed (1 for just a command, 2 for command + 1 argument, etc.
 *
 * @return array of strings, a[0] will be the command, a[1]...\0 are the arguments. This is ready for passing to execvp
 */
char ** splitCommandAndArgs(char * line, int * numberArgs)
{
	char ** ret = NULL;
	char state = CHARACTER;
	char c = 0x0;
	unsigned int i = 0;
	unsigned int numArgs = 0;

	if (line == NULL)
		return NULL;
	else
	{
		ret = malloc(sizeof(char *));
		if (ret == NULL)
		{
			perror("Couldn't malloc");
			return NULL;
		}
	}

	while(1)
	{
		c = line[i];
		// DEBUG:
		// printf("Looking at %c (searching for %s)\n",c,state == CHARACTER ? "character":"whitespace");
		if ((c == ';') || (c == 0x0))
			break;
		if (state == CHARACTER)
		{
			// In the middle of whitespace, searching for next character
			if (c == ' ')
			{
				// Found whitespace
				// Do Nothing
			}
			else
			{
				// Found character.
				// Grow array
				numArgs++;
				ret = realloc(ret,sizeof(char *) * numArgs);
				if (ret == NULL)
				{
					perror("Couldn't realloc");
					return NULL;
				}
				// Put the pointer to the first character in the array
				ret[numArgs-1] = line + i;
				// Switch states.
				state = WHITESPACE;
			}
			// Either way, we're done with this character, so advance.
			i++;
		}
		else
		{
			// In the middle of characters, searching for next whitespace
			if (c != ' ')
			{
				// Standard character found
				// Do Nothing
			}
			else
			{
				// Whitespace found.
				// Terminate the line.
				line[i] = 0x0;
				// Change state
				state = CHARACTER;
			}
			// Either way, we're done with this character, so advance.
			i++;
		}
	}
	*numberArgs = numArgs;
	ret = realloc(ret,sizeof(char *) * (numArgs+1));
	ret[numArgs] = 0x0;
	return ret;
}
