/*
* lab 4.c
*
* Created: 6/7/2011 12:57:25 PM
*  Author: dcstanc
*/ 


// Steal from the Linux kernel
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

//BUILD_BUG_ON(sizeof(int) > sizeof(void*));

#define F_CPU	16000000
#include <util/delay.h>
#include <avr/io.h>
#include "kernel.h"
#include <avr/interrupt.h>

#define NULL (void *) 0


// Base frequency for the buzzer
#define SPEAKER_HIGH 0x20
#define SPEAKER_LOW 0xDF

#define LDR_RANGE 890-620
#define BUZZER_RANGE 500-200

int buzzerFreq = 200;

/*
* Parameter is base frequency in Hertz.
*
*/

int t1init();

void task1(void* p) {
	t1init();
	int FREQ = (int)p;
	float DELAY = 1.0/(1.0*FREQ);
	int count1=0,count2=0;
	count1++;
	say("task 1 initing\r\n");
	while (1) {
		//FREQ = buzzerFreq;
		FREQ = 200;
		DELAY = 1.0/(1.0*FREQ);
		//DELAY = 0.01;
		//say("task 1 runnig\r\n");
		PORTB |= SPEAKER_HIGH;
		
		_delay_ms((DELAY*1.0/2.0)*1000);
				
		PORTB &= SPEAKER_LOW;
		_delay_ms((DELAY*1.0/2.0)*1000);
		//count2++;
		 //OSSwapTask();
		//count1++;
		//say1("t1:: %d\r\n", count1);
	}
}

int t1init() {
	DDRB |= SPEAKER_HIGH;
	return 0;
}

int t2init() {
	//Power on the ADC unit
	PRR &= 0xFE;

	//Enable the ADC, choose prescale value of 128 == 125KHz sampling
	//rate. Set ADIE to enable interrupts.
	ADCSRA = 0x87;//0b10000111

	//Choose channel 1, and V_cc reference voltage
	ADMUX = 0x41;//0100 0001
}

unsigned ledFreq=1;

int ledRemap(int adcinput) {
	double temp = 1.0*(adcinput - 620 )/(1.0*(LDR_RANGE))*255.0;
	if(temp>255)
	{
		temp= 255;
	}
	else if(temp < 0)
	{
		temp=0;
	}
	double frequency=1+20*temp/255.0;
	return (int)frequency;
}

void task2(void* p) {
	int tmp;
	int count1=0,count2=0;
	unsigned l, h;
	say("task 2 initing\r\n");
	//count1++;
	while (1) {
		while (!requestADC());
		t2init();
		ADCSRA |= 0x40;//0b0100 0000

		//remap adc val

		//wait for ADC conversion to finish
		while (ADCSRA & 0x40);
		//Clear ADIF & read input value
		ADCSRA |= 0x10;//0001 0000

		l = ADCL;
		h = ADCH;
		tmp = h*256 + l;
		//ledFreq = ledRemap(tmp);
		ledFreq= 1+9*(float)tmp/1023.0;
		
		say1("task2::%d\r\n", tmp);
		
		//restart ADC conversion
		//ADCSRA |= 0x40;
		releaseADC();
		// OSSwapTask();
		//say("task 2 running\r\n");

	}
}

void t3init() {
	//Power on the ADC unit
	PRR &= 0xFE;

	//Enable the ADC, choose prescale value of 128 == 125KHz sampling
	//rate. Set ADIE to enable interrupts.
	ADCSRA = 0x87;//0b10000111

	//Choose channel 1, and V_cc reference voltage
	ADMUX = 0x40;//0100 0000
}
int remapBuzzerFreq(int adcinput)
{
	adcinput = 200+1.0*adcinput/(1023)*(BUZZER_RANGE);
	return adcinput;
}
void task3(void *p)
{
	int tmp;
	unsigned l, h;
	say("task 3 initing\r\n");
	while (1) {
		while (!requestADC());

		t3init();
		ADCSRA |= 0x40;//0b0100 0000

		//remap adc val


		//wait for ADC conversion to finish
		while (ADCSRA & 0x40);
		//Clear ADIF & read input value
		ADCSRA |= 0x10;//0001 0000

		l = ADCL;
		h = ADCH;
		tmp = h*256 + l;
		
		buzzerFreq = remapBuzzerFreq(tmp);
		say1("task3::%d\r\n", tmp);
		//restart ADC conversion
		//ADCSRA |= 0x40;
		releaseADC();
		// OSSwapTask();
		//say("task 3 running\r\n");
	}		
}
void ledUP()
{
	PORTB |= 0b00010000;	
}
void ledDown()
{
	PORTB &= 0b11101111;
}
void task4()
{
	
	DDRB |= 0b00010000;
	int FREQ = ledFreq;
	float DELAY = 1.0/(1.0*FREQ);
	//	cli();
	//   ledUP();
	// sei();
	int count1=0,count2=0;
	count1++;
	say("task 4 initing\r\n");
	while (1) {
		FREQ = ledFreq;
		DELAY = 1.0/(1.0*FREQ);
	//say("task 4 running\r\n");
		ledDown();
		double tmp = (DELAY*1.0/2.0)*1000;
		if (tmp <= 0)
			tmp = 500;
		//say1("delay val is %d \r\n", (int)tmp);
		_delay_ms(tmp);
		//
		count2++;
		ledUP();
		_delay_ms(tmp);
		// OSSwapTask();

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
int *task1Stack, *task2Stack, *task3Stack,*task4Stack;

int main()
{
	task1Stack = (int*)malloc(100 * sizeof(int));
	memset(task1Stack, 0, 100);
	task2Stack = (int*)malloc(100 * sizeof(int));
	memset(task2Stack, 0, 100);
	task3Stack = (int*)malloc(100 * sizeof(int));
	memset(task3Stack, 0, 100);
	task4Stack = (int*)malloc(100 * sizeof(int));
	memset(task4Stack, 0, 100);
	setup();
	
	OSInit();

	// Create the tasks. Task1 is for the buzzer and we pass in the base frequency as an argument.

	OSAddTask(task1, 0, &task1Stack[50], NULL);
	OSAddTask(task2, 1, &task2Stack[50], NULL);
	OSAddTask(task3, 2, &task3Stack[50], NULL);
	OSAddTask(task4,3,&task4Stack[50], NULL);
	// Insert code to create task4 below.
	
	// And start the OS!
	OSRun();
}
