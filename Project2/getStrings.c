/*
 * filename: getStrings.c
 * description: These are functions that return dynamically allocated strings from files or stdin.
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #2
 * assigned: January 28th, 2015
 * due: February 11th, 2015
 */

#include "getStrings.h"

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
	if ((c == EOF) && (len == 0))
		return NULL;
	else
		return str;
}
