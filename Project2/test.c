#include <stdio.h>
#include <stdlib.h>
#include "getStrings.h"

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