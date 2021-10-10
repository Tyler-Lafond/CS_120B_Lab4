/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # 4  Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char lock = 0x00;
unsigned char lockFlag = 0x00;
unsigned char state = 0x00;
unsigned char prev = 0x00;

enum Lock_States { Lock_SMStart, Lock_Init, Lock_Press_a2, Lock_Unpress_a2, Lock_Unlocked, Lock_Locked } Lock_State;

void TickLock()
{
	switch(Lock_State)
	{
		case Lock_SMStart:
			Lock_State = Lock_Init;
			break;
		case Lock_Init:
			if ((PINA & 0x87) == 0x04)
			{
				Lock_State = Lock_Press_a2;
			}
			else if ((PINA & 0x87) == 0x80)
			{
				Lock_State = Lock_Locked;
			}
			else
			{
				Lock_State = Lock_Init;
			}
			break;
		case Lock_Press_a2:
			if ((PINA & 0x87) == 0x00)
			{
				Lock_State = Lock_Unpress_a2;
			}
			else if ((PINA & 0x87) == 0x80)
			{
				Lock_State = Lock_Locked;
			}
			else if ((PINA & 0x87) == 0x04)
			{
				Lock_State = Lock_Press_a2;
			}
			else
			{
				Lock_State = Lock_Init;
			}
			break;
		case Lock_Unpress_a2:
			if ((PINA & 0x87) == 0x02)
			{
				if (lock == 0x00)
				{
					Lock_State = Lock_Unlocked;
				}
				else if (lock == 0x01)
				{
					lockFlag = 0x01;
					Lock_State = Lock_Locked;
				}
			}
			else if ((PINA & 0x87) == 0x80)
			{
				Lock_State = Lock_Locked;
			}
			else if ((PINA & 0x87) == 0x00)
			{
				Lock_State = Lock_Unpress_a2;
			}
			else
			{
				Lock_State = Lock_Init;
			}
			break;
		case Lock_Unlocked:
			lock = 0x01;
			PORTB = lock;
			Lock_State = Lock_Init;
			break;
		case Lock_Locked:
			lock = 0x00;
			PORTB = lock;
			if (prev == 0x02)
			{
				Lock_State = Lock_Press_a2;
			}
			else if (prev == 0x03 && lockFlag == 0x00)
			{
				Lock_State = Lock_Unpress_a2;
			}
			else if (prev == 0x03 && lockFlag == 0x01)
			{
				lockFlag = 0x00;
				Lock_State = Lock_Init;
			}
			else
			{
				Lock_State = Lock_Init;
			}
			break;
		default:
			Lock_State = Lock_SMStart;
			break;
	}

	switch(Lock_State)
	{
		case Lock_SMStart:
			prev = state;
			state = 0x00;
			PORTC = state;
			break;
		case Lock_Init:
			prev = state;
			state = 0x01;
			PORTC = state;
			break;
		case Lock_Press_a2:
			prev = state;
			state = 0x02;
			PORTC = state;
			break;
		case Lock_Unpress_a2:
			prev = state;
			state = 0x03;
			PORTC = state;
			break;
		case Lock_Unlocked:
			prev = state;
			state = 0x04;	
			PORTC = state;
			break;
		case Lock_Locked:
			prev = state;
			state = 0x05;
			PORTC = state;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
	PORTB = 0x00;
	PORTC = 0x00;
	Lock_State = Lock_SMStart;

    while (1) {
	TickLock();
    }
    return 1;
}
