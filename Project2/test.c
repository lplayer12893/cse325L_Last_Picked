#include <stdio.h>
#include <stdlib.h>

char * getStringFromFile(FILE *);
/**
 * This is a function that gets a string of infinite length from stdin. This string should be freed after use.
 *
 * @return pointer to new string.
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

int main(int argc, char ** argv)
{
	FILE * f = NULL;
	char * str = NULL;
	if (argc == 2)
	{
		f = fopen(argv[1],"r");
		if (f == NULL)
		{
			perror("Couldn't open file");
			return 1;
		}
		str = getStringFromFile(f);
	}
	else
	{
		printf("Enter text: ");
		fflush(stdout);
		str = getString();
	}
	
	printf("You entered >%s<\n",str);
	
	if (f != NULL)
	{
		fclose(f);
		f = NULL;
	}
	free(str);
	
	return 0;
}