/*
 * kernel.c
 *
 * Created: 6/7/2011 12:57:25 PM
 *  Author: dcstanc
 */ 

#define F_CPU	16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "kernel.h"
#include "contextMacros.h"

int timerCount = 0;
int currentTask = -1;
unsigned long pxCurrentTCB;
unsigned numTasks=0;
int adcUser = -1;
int adcInUse = 0;

int requestADC() {
	OSMakeAtomic();
	int returnVal = 0;
	if (!adcInUse)
	{
		adcInUse = 1;
		adcUser = currentTask;
		returnVal = 1;
	}
	else if (adcUser == currentTask)
		returnVal = 1;
	else returnVal = 0;	
	OSLeaveAtomic();
	return returnVal;	
}

void releaseADC() {
	OSMakeAtomic();
	adcInUse = 0;
	adcUser = -1;
	OSLeaveAtomic();
}

typedef struct ttcb
{
	int runCount;
	//int runflag; // Shows whether task has been run before or not. 0 = first time this task is being run, 
				// 1 = task has been run before and was pre-empted/surrendered control.
				 
	int prio; // Task Priority.
	void (*fptr)(void *); // Pointer to the task
	void *arg; // Argument to fptr
	unsigned long stack_ptr; // Stack pointer
	unsigned long base_ptr;
} TTaskBlock;

// Task Control Block (TCB), which contains information on all the tasks.
TTaskBlock* taskTable;
void OSMakeAtomic()
{
	// Disables interrupts to create an atomic section.
	cli();
}
void setupSchedulerTimer()
{
	TCNT0=0;//initial value
	OCR0A=156; //count value
	TCCR0A = 0b01000010;//WGM set to CTC mode. COMOA set to toggle
	TIMSK0 |= 0b10;
	
}
void startSchedulerTimer()
{
	TCCR0B= 0b00000101;//prescalar set to 1024
	sei();
}
void OSLeaveAtomic()
{
	sei();
	// Leaves atomic section by re-enabling interrupts.
}

// DO NOT modify this!
void OSSwapTask() __attribute__ ((naked));

int findNextTask()
{
	// Apply scheduling algorithm to find next task to run, and returns the index number of that task.
	// Note that OS_NUM_TASKS in kernel.h is the MAXIMUM number of tasks that can be created, not the actual number
	// created. So OS_NUM_TASKS can be 10 although in actual fact only 4 tasks were created. You should pick only amongst
	// these 4 tasks.
	currentTask = (currentTask+1)%numTasks;
}

void OSSwapTask()
 {
	 // Save the context of the current task, and copy the task stack pointer to the task's entry in the Task Control Block (tcb).

	// Call findNextTask to decide which task to run next.
	// Start up the next task.
	// Important: If you are starting up a task for the first time, you must set pxCurrentTCB
	// to be equal to the address of the stack allocated to the task in OSAddTask, then
	// call MOVSP to correctly set up SP_L and SP_H.


	// pxCurrentTCB = taskTable[currentTask].base_ptr;
	// MOVSP();

	portSAVE_CONTEXT();
	taskTable[currentTask].stack_ptr=pxCurrentTCB;
	currentTask = findNextTask();

	if (!taskTable[currentTask].runCount) {
		pxCurrentTCB = taskTable[currentTask].base_ptr;
		MOVSP();
		taskTable[currentTask].runCount++;
		taskTable[currentTask].fptr((void *)taskTable[currentTask].arg);
	}
	else{
		pxCurrentTCB = taskTable[currentTask].stack_ptr;
		portRESTORE_CONTEXT();
		// STRSP();
		// taskTable[currentTask].stack_ptr=pxCurrentTCB;
		// say("hallo");
	}
	
// Do not modify the line below!	
	asm("ret");
}

// ISR for the timer. IMPLEMENT ONLY IN PART 2!
ISR(TIMER0_COMPA_vect, ISR_NAKED)
{
	timerCount++;
	if(timerCount>=50)
	{
		OSSwapTask();
		timerCount=0;
	}		
	// Do not change this!
	asm("reti");
}
void OSInit()
{ 
	// Initialize all OS state variables, hardware used DIRECTLY by the OS, etc etc. Do not initialize variables or hardware
	// used by the application. Only those used by the OS.
	taskTable= (TTaskBlock*)malloc(OS_NUM_TASKS*sizeof(TTaskBlock));
}

// First argument is a pointer to the task's function code, the second is a priority number (0=highest priority)
// the 3rd argument is a pointer to the last element of integer array to be used as a stack 
// and the 4th argument is an argument that will be passed to the task function the first time it is run.
// E.g. 
// int task1Stack[32];
// OSAddTask(task1, 0, &task1Stack[31], (void *) 5) causes a task called task1 to be created with priority 0, and the integer
// 5 is passed to task1 when it is first called. See app.c to fully understand how OSAddTask is used.
 
int OSAddTask(void (*taskptr)(void *), int prio, int *stack, void* arg)
{
	// taskptr = Pointer to task code. Prio = task priority. stack=pointer to task stack. Arg=argument to pass to task when starting up.
	// Adds a new entry to the TCB. Returns a -1 if # of tasks > OS_NUM_TASKS. Note that OS_NUM_TASKS is the MAXIMUM
	// number of tasks that can be created, not the actual number.
	
	TTaskBlock tempTask;
	tempTask.fptr = taskptr;
	tempTask.arg = arg;
	tempTask.prio = prio;
	tempTask.stack_ptr = stack;
	tempTask.base_ptr = stack;
	tempTask.runCount=0;
	taskTable[numTasks] = tempTask;
	
	numTasks++;

	return 0;
}
void OSRun()
{
	setupSchedulerTimer();
	currentTask = 0;
	taskTable[0].runCount++;
	startSchedulerTimer();
	taskTable[0].fptr((void*)taskTable[0].arg);
}