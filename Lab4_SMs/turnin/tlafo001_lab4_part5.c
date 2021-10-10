/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # 4  Exercise # 5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char combo[7] = {0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01};
unsigned char lock = 0x00;
unsigned char lockMarker = 0;
unsigned char lockFlag = 0x00;
unsigned char state = 0x00;

enum Lock_States { Lock_SMStart, Lock_Init, Lock_Combo, Lock_Unlocked, Lock_Locked } Lock_State;

void TickLock()
{
	switch(Lock_State)
	{
		case Lock_SMStart:
			Lock_State = Lock_Init;
			break;
		case Lock_Init:
			if ((PINA & 0x87) == combo[0])
			{
				lockMarker++;
				Lock_State = Lock_Combo;
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
		case Lock_Combo:
			if ((PINA & 0x87) == combo[lockMarker])
			{
				if (lockMarker == 6 && lock == 0x00)
				{
					Lock_State = Lock_Unlocked;
				}
				else if (lockMarker == 6 && lock == 0x01)
				{
					lockFlag = 0x01;
					Lock_State = Lock_Locked;
				}
				else
				{
					lockMarker++;
					Lock_State = Lock_Combo;
				}
			}
			else if ((PINA & 0x87) == 0x80)
			{
				Lock_State = Lock_Locked;
			}
			else if ((PINA & 0x87) == combo[lockMarker-1])
			{
				Lock_State = Lock_Combo;
			}
			else
			{
				lockMarker = 0;
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
			if (lockMarker == 0)
			{
				Lock_State = Lock_Init;
			}
			else if (lockFlag == 0x00)
			{
				Lock_State = Lock_Combo;
			}
			else if (lockFlag == 0x01)
			{
				lockFlag = 0x00;
				lockMarker = 0;
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
			state = 0x00;
			PORTC = state;
			break;
		case Lock_Init:
			state = 0x01;
			PORTC = state;
			break;
		case Lock_Combo:
			state = 0x02;
			PORTC = state;
			break;
		case Lock_Unlocked:
			state = 0x03;	
			PORTC = state;
			break;
		case Lock_Locked:
			state = 0x04;
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
