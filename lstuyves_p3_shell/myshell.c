/******************************************************************
* Filename: myshell.c
* Description: This program implements process forking like a shell
* author: Stuyvesant, Lucas
* class: CSE 222
* instructor: Zheng
* assignment: Project #3
* assigned: March 25, 2013
* due: April 1, 2013
******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>   //needed on my system

#define MAX_LINE 80 /* The maximum length command */

void processManager(char **, int);

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */

    int should_run = 1; /* flag to determine when to exit */
	int i = 0;
	char buff[MAX_LINE];

    while(should_run){
        i = 0;

        printf("cse222>");
        fflush(stdout);
		
        fgets(buff, MAX_LINE, stdin);   //get command
        
        if((strcmp(buff, "exit\n")) == 0){  //check for shell termination
            should_run = 0;
        }
        else{
            args[i] = strtok(buff, " ");    //split command into arguments
            i++;
            while((args[i] = strtok(NULL, " ")) != NULL){
                i++;
            }
            args[i-1] = strtok(args[i-1], "\n");

            processManager(args, i);    //call function to fork processes
        }
    }
    return 0;
}

/*
 * Accepts i number of arguments for an command and forks the command
 * to execute in a child process
 */
void processManager(char **args, int i){

    int status;
    pid_t pid;
    int job = 0;

    if(strcmp(args[i-1], "&") == 0){    //run concurrently
        args[i-1] = NULL;
        if((pid = fork()) > 0){    //if process is parent
            job++;
            printf("[%i] %i\n", job, pid); //job and process ID of child, so this is different from no &
            wait(&status);  //in the Project description, you indicate to do this. 
                            //this is UNCONVENTIONAL for using &
        }
        else if(pid == 0){    //if process is child
            if(execvp(args[0], args) < 0){
                printf("Error: Executing command");
                exit(-1);
            }
        }
        else{
            printf("Error: Error while forking process\n");
        }
    }
    else{    //parent waits for child
        if((pid = fork()) > 0){    //if process is parent
            wait(&status);    //not specified otherwise, so wait for child
        }
        else if(pid == 0){    //if process is child
            if(execvp(args[0], args) < 0){
                printf("Error: Executing command");
                exit(-1);
            }
        }
        else{
            printf("Error: Error while forking process\n");
        }
    }
}

