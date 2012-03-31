/*
 * lab 4.c
 *
 * Created: 6/7/2011 12:57:25 PM
 *  Author: dcstanc
 */ 

#define F_CPU	16000000
#include <util/delay.h>
#include <avr/io.h>
#include "kernel.h"

#define NULL (void *) 0


// Base frequency for the buzzer
#define BASE_FREQ	200
int count1 =0;
int count2 =0;
void task1(void *p)
{
	// Operates the buzzer
	// The base frequency is passed in through p.
	int base=(int) p;
	count1++;
	while(1)
	{
		count2++;

		// Implement Task 1 logic below
		
		// Implement Task 1 logic above
		
		// Hand over control to the OS.
		say("task 1 running ...\r\n");
		say1("%d\r\n",count1);
		say1("%d\r\n",count2);
		OSSwapTask();
	}		
}

void task2(void *p)
{
	int c1=0, c2=0;
	say1("task2, init, c1: %d", c1++);
	while(1)
	{
		say1("task 2 running ... %d\r\n",c2++);
		// Implement Task 2 logic below
		
		// Implement Task 2 logic above
		
		// Hand over control to the OS.
		OSSwapTask();
	}		
}

void task3(void *p)
{
	say("task 3 initializing...\r\n");
	while(1)
	{
		say("task 3 running ...\r\n");
		// Implement Task 3 logic below
		
		// Implement Task 3 logic above
		
		// Hand over control to the OS.
		OSSwapTask();
	}		
}

void task4(void *ptr)
{
	while(1)
	{
		// Implement Task 4 logic below
		
		// Implement Task 4 logic above
		
		// Hand over control to the OS.
		OSSwapTask();
	}		
	
}

// Declare all other ISRs, etc below this line.

// Declare all other ISRs, etc above this line.

void setup()
{
	// Set up the hardware except those used directly by the OS (e.g. Timer 0).
	setupSerial();
}

// The stack sizes may need adjusting!!
int *task1Stack, *task2Stack, *task3Stack;

int main()
{
	task1Stack = (int*)malloc(100 * sizeof(int));
	memset(task1Stack, 0, 100);
	task2Stack = (int*)malloc(100 * sizeof(int));
	memset(task2Stack, 0, 100);
	task3Stack = (int*)malloc(100 * sizeof(int));
	memset(task3Stack, 0, 100);
	setup();
	say("hi fuckers\r\n");
	OSInit();

	// Create the tasks. Task1 is for the buzzer and we pass in the base frequency as an argument.
	OSAddTask(task1, 0, &task1Stack[50], (void *) BASE_FREQ);
	OSAddTask(task2, 1, &task2Stack[50], NULL);
	 OSAddTask(task3, 2, &task3Stack[50], NULL);
	
	// Insert code to create task4 below.
	
	// And start the OS!
	OSRun();
}
