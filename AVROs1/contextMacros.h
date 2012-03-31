/*
 * contextMacros.h
 *
 * Created: 3/24/2012 7:12:09 PM
 *  Author: Omer the king. Bow to thy king
 */ 


#ifndef CONTEXTMACROS_H_
#define CONTEXTMACROS_H_


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





//r26, 27, ...
// r26 = sp_l
// r27 = sp_h
#define STRSP()\
asm volatile ("cli \n\t"\
				"in r26, __SP_L__	\n\t"\
				"in r27, __SP_H__	\n\t"\
				"sts pxCurrentTCB+1, r27	\n\t"\
				"sts pxCurrentTCB, r26	\n\t"\
				"sei");

#endif /* CONTEXTMACROS_H_ */