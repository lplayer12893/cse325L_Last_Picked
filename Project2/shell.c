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
	char * input = NULL;
	int argnum = 0x0;
	FILE * batchFile = NULL;

	// First, check for interactive or batch mode
	if (argc == 1)
	{
		// Interactive mode
		opMode = INTERACTIVE;
		printf("Welcome to interactive mode. Type 'quit' to quit.");
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
		}

		// TODO: Tokenize on the semicolon
		// Repeat this loop for every command separated by semicolon, but they should not include the wait command, IE they should run concurrently (except quit)
		if (input[0] != 0)
		{
			// int i = 0;
			argnum = 0;
			args = splitCommandAndArgs(input, &argnum);

			/* Check for quit */
			// TODO: Check for end of string, should also force quit (Ctrl-D or end of batch argument)
			if (strcmp(args[0], "quit") == 0)
			{
				/* Received quit command. */
				printf("Goodbye!\n");
				keep_running = 0;
			}
			else
			{
				/* Not exit command. Run as normal. */
				int pid;

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
			free(args);
			args = NULL;
		}
		free(input);
		input = NULL;
	}

	if (opMode == BATCH)
	{
		// TODO: Close file
	}
	return 0;
}


/**
 * This is a function that gets a string of infinite length from stdin.
 *
 * @return pointer to new string. This should be freed after use.
 */
char * getString()
{
	return getStringFromFile(stdin);
}

/**
 * This is a function that gets a string from a file. Note, this file should be open already.
 *
 * @param f File to use
 * @return pointer to a new string. This should be freed after use.
 */
char * getStringFromFile(FILE * f)
{
	char * str = malloc(sizeof(char));
	unsigned int len = 0;
	char c = 0x0;
	if (str == NULL)
	{
		perror("Cannot malloc");
		return NULL;
	}
	while (((c = fgetc(f)) != '\n') && (c != EOF))
	{
		str[len] = c;
		len++;
		str = realloc(str,(len+1)*sizeof(char));
	}
	str[len] = 0x0;
	return str;
}

/**
 * Returns a list of commands (strings) represented in a single string
 *
 * @param input input string to parse
 *
 * @return pointer to a list of commands
 */
char ** getCommands(char *input){
    //char *list[] = malloc(strlen(input) * sizeof(char));
    int size = strlen(input);
    int i = 0,count = 1;
    char *string = malloc(strlen(input) * sizeof(char));
	 if (string == NULL)
	 {
		 perror("Couldn't malloc");
		 return NULL;
	 }

    for(i; i < size; i++)
    {
        if(input[i] == ';'){
            count++;
        }
    }

    char *list[count];

    string = strtok(input,";");

    for(i = 0; string != NULL; i++)
    {
        list[i] = string;
        string = strtok(NULL,";");
    }

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
	return ret;
}
