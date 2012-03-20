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

void task1(void *p)
{
	// Operates the buzzer
	// The base frequency is passed in through p.
	int base=(int) p;
	
	while(1)
	{

		// Implement Task 1 logic below
		
		// Implement Task 1 logic above
		
		// Hand over control to the OS.
		OSSwapTask();
	}		
}

void task2(void *p)
{
	while(1)
	{
		// Implement Task 2 logic below
		
		// Implement Task 2 logic above
		
		// Hand over control to the OS.
		OSSwapTask();
	}		
}

void task3(void *p)
{
	while(1)
	{
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
}

// The stack sizes may need adjusting!!
int task1Stack[32], task2Stack[32], task3Stack[32];

int main()
{
	setup();
	
	OSInit();

	// Create the tasks. Task1 is for the buzzer and we pass in the base frequency as an argument.
	OSAddTask(task1, 0, &task1Stack[31], (void *) BASE_FREQ);
	OSAddTask(task2, 1, &task2Stack[31], NULL);
	OSAddTask(task3, 2, &task3Stack[31], NULL);
	
	// Insert code to create task4 below.
	
	// And start the OS!
	OSRun();
}
