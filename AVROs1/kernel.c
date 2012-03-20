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

unsigned long pxCurrentTCB;
unsigned numTasks=0;
// Task Control Block
typedef struct ttcb
{
	int runflag; // Shows whether task has been run before or not. 0 = first time this task is being run, 
				// 1 = task has been run before and was pre-empted/surrendered control.
				 
	int prio; // Task Priority.
	void (*fptr)(void *); // Pointer to the task
	void *arg; // Argument to fptr
	long stack_ptr; // Stack pointer
} TTaskBlock;

// Task Control Block (TCB), which contains information on all the tasks.
TTaskBlock taskTable[OS_NUM_TASKS];

// Macros to save and restore context. DO NOT CHANGE THESE!
#define portSAVE_CONTEXT()\
asm volatile (\
	"push r0	\n\t"\
	"in r0, __SREG__	\n\t"\
	"cli	\n\t"\
	"push r0	\n\t"\
	"push r1	\n\t"\
	"clr r1	\n\t"\
	"push r2	\n\t"\
	"push r3	\n\t"\
	"push r4	\n\t"\
	"push r5	\n\t"\
	"push r6	\n\t"\
	"push r7	\n\t"\
	"push r8	\n\t"\
	"push r9	\n\t"\
	"push r10	\n\t"\
	"push r11	\n\t"\
	"push r12	\n\t"\
	"push r13	\n\t"\
	"push r14	\n\t"\
	"push r15	\n\t"\
	"push r16	\n\t"\
	"push r17	\n\t"\
	"push r18	\n\t"\
	"push r19	\n\t"\
	"push r20	\n\t"\
	"push r21	\n\t"\
	"push r22	\n\t"\
	"push r23	\n\t"\
	"push r24	\n\t"\
	"push r25	\n\t"\
	"push r26	\n\t"\
	"push r27	\n\t"\
	"push r28	\n\t"\
	"push r29	\n\t"\
	"push r30	\n\t"\
	"push r31	\n\t"\
	"in r26, __SP_L__	\n\t"\
	"in r27, __SP_H__	\n\t"\
	"sts pxCurrentTCB+1, r27	\n\t"\
	"sts pxCurrentTCB, r26	\n\t"\
	"sei	\n\t" : : :\
);

#define portRESTORE_CONTEXT()\
asm volatile (\
	"out __SP_L__, %A0	\n\t"\
	"out __SP_H__, %B0	\n\t"\
	"pop r31	\n\t"\
	"pop r30	\n\t"\
	"pop r29	\n\t"\
	"pop r28	\n\t"\
	"pop r27	\n\t"\
	"pop r26	\n\t"\
	"pop r25	\n\t"\
	"pop r24	\n\t"\
	"pop r23	\n\t"\
	"pop r22	\n\t"\
	"pop r21	\n\t"\
	"pop r20	\n\t"\
	"pop r19	\n\t"\
	"pop r18	\n\t"\
	"pop r17	\n\t"\
	"pop r16	\n\t"\
	"pop r15	\n\t"\
	"pop r14	\n\t"\
	"pop r13	\n\t"\
	"pop r12	\n\t"\
	"pop r11	\n\t"\
	"pop r10	\n\t"\
	"pop r9	\n\t"\
	"pop r8	\n\t"\
	"pop r7	\n\t"\
	"pop r6	\n\t"\
	"pop r5	\n\t"\
	"pop r4	\n\t"\
	"pop r3	\n\t"\
	"pop r2	\n\t"\
	"pop r1	\n\t"\
	"pop r0	\n\t"\
	"out __SREG__, r0\n\t"\
	"pop r0	\n\t": : "r" (pxCurrentTCB):\
	);

#define MOVSP()\
asm volatile ("cli \n\t"\
				"lds r26, pxCurrentTCB \n\t"\
				"lds r27, pxCurrentTCB+1 \n\t"\
				"out __SP_L__, r26 \n\t"\
				"out __SP_H__, r27 \n\t"\
				"sei");

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
}
void OSSwapTask()
 {
	 // Save the context of the current task, and copy the task stack pointer to the task's entry in the Task Control Block (tcb).

	// Call findNextTask to decide which task to run next.
	// Start up the next task.
	// Important: If you are starting up a task for the first time, you must set pxCurrentTCB
	// to be equal to the address of the stack allocated to the task in OSAddTask, then
	// call MOVSP to correctly set up SP_L and SP_H.
		

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
	
	TTaskBlock *tempTask;
	tempTask->fptr()= taskptr;
	tempTask->arg = arg;
	tempTask->prio = prio;
	tempTask->stack_ptr = stack;
	tempTask->runflag=0;
	taskTable[numTasks] = tempTask;
	
	numTasks++;

	return 0;	
}

void OSRun()
{
	// Starts by executing the first task
	int i= 0;
	taskTable[0]->runflag=0;
	taskTable[0]->fptr(taskTable[0]->arg);
	while(1){
		
		OSSwapTask()
		i=findNextTask()
		taskTable[i]->fptr(taskTable[i]->arg);
		
	}
	
}