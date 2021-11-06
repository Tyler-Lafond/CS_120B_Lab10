/*	Author: tlafo001
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab # 10  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char tempB;

enum ThreeLED_states { ThreeLED_SMStart, ThreeLED_Zero, ThreeLED_One, ThreeLED_Two } ThreeLED_state;
void Tick_ThreeLEDs() {
	switch(ThreeLED_state)
	{
		case ThreeLED_SMStart:
			ThreeLED_state = ThreeLED_Zero;
			break;
		case ThreeLED_Zero:
			ThreeLED_state = ThreeLED_One;
			break;
		case ThreeLED_One:
			ThreeLED_state = ThreeLED_Two;
			break;
		case ThreeLED_Two:
			ThreeLED_state = ThreeLED_Zero;
			break;
		default:
			ThreeLED_state = ThreeLED_SMStart;
			break;
	}

	switch(ThreeLED_state)
	{
		case ThreeLED_Zero:
			threeLEDs = 0x01;
			break;
		case ThreeLED_One:
			threeLEDs = 0x02;
			break;
		case ThreeLED_Two:
			threeLEDs = 0x04;
			break;
		default:
			break;
	}
}

enum BlinkingLED_states { BlinkingLED_SMStart, BlinkingLED_Off, BlinkingLED_On } BlinkingLED_state;
void Tick_BlinkingLED() {
	switch(BlinkingLED_state)
	{
		case BlinkingLED_SMStart:
			BlinkingLED_state = BlinkingLED_Off;
			break;
		case BlinkingLED_Off:
			BlinkingLED_state = BlinkingLED_On;
			break;
		case BlinkingLED_On:
			BlinkingLED_state = BlinkingLED_Off;
			break;
		default:
			BlinkingLED_state = BlinkingLED_SMStart;
			break;
	}

	switch(BlinkingLED_state)
	{
		case BlinkingLED_Off:
			blinkingLED = 0x00;
			break;
		case BlinkingLED_On:
			blinkingLED = 0x08;
			break;
		default:
			break;
	}
}

enum CombineLED_states { CombineLED_SMStart, CombineLED_Output } CombineLED_state;
void Tick_CombineLEDs() {
	switch(CombineLED_state)
	{
		case CombineLED_SMStart:
			CombineLED_state = CombineLED_Output;
			break;
		case CombineLED_Output:
			CombineLED_state = CombineLED_Output;
			break;
		default:
			CombineLED_state = CombineLED_SMStart;
			break;
	}

	switch(CombineLED_state)
	{
		case CombineLED_Output:
			tempB = (threeLEDs | blinkingLED);
			PORTB = tempB;
			break;
		default:
			break;
	}
}

int main(void) {
	unsigned long ThreeLED_elapsedTime = 300;
	unsigned long BlinkingLED_elapsedTime = 1000;
	unsigned long CombineLED_elapsedTime = 100;
	const unsigned long timerPeriod = 50;
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	TimerSet(timerPeriod);
	TimerOn();
	ThreeLED_state = ThreeLED_SMStart;
	BlinkingLED_state = BlinkingLED_SMStart;
	CombineLED_state = CombineLED_SMStart;
    while (1) {
	if (ThreeLED_elapsedTime >= 300)
	{
		Tick_ThreeLEDs();
		ThreeLED_elapsedTime = 0;
	}
	if (BlinkingLED_elapsedTime >= 1000)
	{
		Tick_BlinkingLED();
		BlinkingLED_elapsedTime = 0;
	}
	if (CombineLED_elapsedTime >= 100)
	{
		Tick_CombineLEDs();
		CombineLED_elapsedTime = 0;
	}
	while (!TimerFlag){}
	TimerFlag = 0;
	ThreeLED_elapsedTime += timerPeriod;
	BlinkingLED_elapsedTime += timerPeriod;
	CombineLED_elapsedTime += timerPeriod;
    }
    return 1;
}
