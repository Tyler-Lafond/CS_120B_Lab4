/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # 4  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Light_States { Light_SMStart, Light_Unpress_b0, Light_Press_b0, Light_Unpress_b1, Light_Press_b1 } Light_State;

void TickLight()
{
	switch(Light_State)
	{
		case Light_SMStart:
			Light_State = Light_Unpress_b0;
			break;
		case Light_Unpress_b0:
			if (PINA == 0x01)
			{
				Light_State = Light_Press_b1;
			}
			else if (PINA == 0x00)
			{
				Light_State = Light_Unpress_b0;
			}
			break;
		case Light_Press_b1:
			if (PINA == 0x00)
			{
				Light_State = Light_Unpress_b1;
			}
			else if (PINA == 0x01)
			{
				Light_State = Light_Press_b1;
			}
			break;
		case Light_Unpress_b1:
			if (PINA == 0x01)
			{
				Light_State = Light_Press_b0;
			}
			else if (PINA == 0x00)
			{
				Light_State = Light_Unpress_b1;
			}
			break;
		case Light_Press_b0:
			if (PINA == 0x00)
			{
				Light_State = Light_Unpress_b0;
			}
			else if (PINA == 0x00)
			{
				Light_State = Light_Press_b0;
			}
			break;
		default:
			Light_State = Light_SMStart;
			break;
	}

	switch(Light_State)
	{
		case Light_Unpress_b0:
			PORTB = 0x01;
			break;
		case Light_Press_b1:
			PORTB = 0x02;
			break;
		case Light_Unpress_b1:
			PORTB = 0x02;
			break;
		case Light_Press_b0:
			PORTB = 0x01;
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PORTB = 0x01;
	Light_State = Light_SMStart;

    while (1) {
	TickLight();
    }
    return 1;
}
