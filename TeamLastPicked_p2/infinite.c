/*
 * filename: infinite.c
 * description: This is a simple program that overflows ints just to take up
 *  some time. Used for testing backgrounding in the shell I wrote.
 * author: Salinas, Sean
 * class: CSE 222 S14
 * instructor: Zheng
 * assignment: Project #3
 * assigned: March 3rd, 2014
 * due: March 10th, 2014
 */

#include <stdio.h>

int main(void)
{
	int a = 0;
	while (a >= 0)
	{
		a++;
	}
	printf("Infinite Finished! ");
	return 0;
}
