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

int currentTask = -1;
unsigned long pxCurrentTCB;
unsigned numTasks=0;

typedef struct ttcb
{
	int runCount;
	//int runflag; // Shows whether task has been run before or not. 0 = first time this task is being run, 
				// 1 = task has been run before and was pre-empted/surrendered control.
				 
	int prio; // Task Priority.
	void (*fptr)(void *); // Pointer to the task
	void *arg; // Argument to fptr
	long stack_ptr; // Stack pointer
} TTaskBlock;

// Task Control Block (TCB), which contains information on all the tasks.
TTaskBlock* taskTable;
void OSMakeAtomic()
{
	// Disables interrupts to create an atomic section.
}

void OSLeaveAtomic()
{
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
	say1("kernel::OSSwapTask:Entering Swap Task, value of currentTask is %d", currentTask);
	portSAVE_CONTEXT();
	currentTask = findNextTask();
	say1("kernel::OSSwapTask:New currentTask is %d", currentTask);
	if (!taskTable[currentTask].runCount) {
		taskTable[currentTask].fptr((void *)taskTable[currentTask].arg);
		
	}
	else{
		pxCurrentTCB = taskTable[currentTask].stack_ptr;
		MOVSP();
		portRESTORE_CONTEXT();
		taskTable[currentTask].fptr((void *)taskTable[currentTask].arg);
		
	}
	
// Do not modify the line below!	
	asm("ret");
}

// ISR for the timer. IMPLEMENT ONLY IN PART 2!
ISR(TIMER0_COMPA_vect, ISR_NAKED)
{
	
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
	tempTask.runCount=0;
	taskTable[numTasks] = tempTask;
	
	numTasks++;

	return 0;
}

void OSRun()
{
	currentTask = 0;
	taskTable[0].runCount++;
	taskTable[0].fptr((void*)taskTable[0].arg);
}