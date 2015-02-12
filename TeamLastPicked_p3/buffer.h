/*
 * filename: buffer.h
 * description: This is the header file for buffer.c, our buffer functions.
 * authors: Salinas, Sean and Stuyvesant, Lucas
 * class: CSE 325, Spring 2015
 * instructor: Zheng
 * assignment: Project #3
 * assigned: February 11th, 2015
 * due: February 25th, 2015
 */

#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdio.h>

void printBuffer(int b[]);
void initializeBuffer(int b[]);
void push(int b[], int item);
int pop(int b[]);
void enqueue(int b[], int item);
int dequeue(int b[]);
int countItems(int b[]);

#endif // __BUFFER_H
