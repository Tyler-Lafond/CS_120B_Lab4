/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # 4  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char cnt;
unsigned char prev;

enum Cnt_States { Cnt_SMStart, Cnt_Unpress, Cnt_Press_a0, Cnt_Press_a1, Cnt_Press_Both, Cnt_Unpress_One } Cnt_State;

void TickCnt()
{
	switch(Cnt_State)
	{
		case Cnt_SMStart:
			cnt = 0x07;
			PORTC = cnt;
			Cnt_State = Cnt_Unpress;
			break;
		case Cnt_Unpress:
			if ((PINA & 0x03) == 0x01)
			{
				if (cnt != 0x09)
				{
					cnt++;
					PORTC = cnt;
				}
				Cnt_State = Cnt_Press_a0;
			}
			else if ((PINA & 0x03) == 0x02)
			{
				if (cnt != 0x00)
				{
					cnt--;
					PORTC = cnt;
				}
				Cnt_State = Cnt_Press_a1;
			}
			else if ((PINA & 0x03) == 0x03)
			{
				cnt = 0x00;
				PORTC = cnt;
				Cnt_State = Cnt_Press_Both;
			}
			else if ((PINA & 0x03) == 0x00)
			{
				Cnt_State = Cnt_Unpress;
			}
			break;
		case Cnt_Press_a0:
			if ((PINA & 0x03) == 0x00)
			{
				Cnt_State = Cnt_Unpress;
			}
			else if ((PINA & 0x03) == 0x03)
			{
				cnt = 0x00;
				PORTC = cnt;
				Cnt_State = Cnt_Press_Both;
			}
			else if ((PINA & 0x03) == 0x02)
			{
				if (cnt != 0x00)
				{
					cnt--;
					PORTC = cnt;
				}
				Cnt_State = Cnt_Press_a1;
			}
			else if ((PINA & 0x03) == 0x01)
			{
				Cnt_State = Cnt_Press_a0;
			}
			break;
		case Cnt_Press_a1:
			if ((PINA & 0x03) == 0x00)
			{
				Cnt_State = Cnt_Unpress;
			}
			else if ((PINA & 0x03) == 0x03)
			{
				cnt = 0x00;
				PORTC = cnt;
				Cnt_State = Cnt_Press_Both;
			}
			else if ((PINA & 0x03) == 0x01)
			{
				if (cnt != 0x09)
				{
					cnt++;
					PORTC = cnt;
				}
				Cnt_State = Cnt_Press_a0;
			}
			else if ((PINA & 0x03) == 0x02)
			{
				Cnt_State = Cnt_Press_a1;
			}
			break;
		case Cnt_Press_Both:
			if ((PINA & 0x03) == 0x00)
			{
				Cnt_State = Cnt_Unpress;
			}
			else if ((PINA & 0x03) == 0x01 || (PINA & 0x03) == 0x02)
			{
				Cnt_State = Cnt_Unpress_One;
			}
			else if ((PINA & 0x03) == 0x03)
			{
				Cnt_State = Cnt_Press_Both;
			}
			break;
		case Cnt_Unpress_One:
			if ((PINA & 0x03) == 0x00)
			{
				Cnt_State = Cnt_Unpress;
			}
			else if ((PINA & 0x03) == 0x03)
			{
				cnt = 0x00;
				PORTC = cnt;
				Cnt_State = Cnt_Press_Both;
			}
			else if (prev != (PINA & 0x03))
			{
				if (prev == 0x02)
				{
					if (cnt != 0x09)
					{
						cnt++;
						PORTC = cnt;
					}
					Cnt_State = Cnt_Press_a0;
				}
				else if (prev == 0x01)
				{
					if (cnt != 0x00)
					{
						cnt--;
						PORTC = cnt;
					}
					Cnt_State = Cnt_Press_a1;
				}
			}
			else if (prev == (PINA & 0x03))
			{
				Cnt_State = Cnt_Unpress_One;
			}
			break;
		default:
			Cnt_State = Cnt_SMStart;
			break;
	}

	switch(Cnt_State)
	{
		case Cnt_Unpress:
			break;
		case Cnt_Press_a0:
			break;
		case Cnt_Press_a1:
			break;
		case Cnt_Press_Both:
			break;
		case Cnt_Unpress_One:
			prev = (PINA & 0x03);
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
	PORTC = 0x07;
	Cnt_State = Cnt_SMStart;

    while (1) {
	TickCnt();
    }
    return 1;
}
