/*
 * File: main.c
 * Description: 
 * This file contains the "main" function for  project, a set of test sequence
 * to test the HD44780 LCD library
 * Author: Gavin Lyons.
 * Complier: xc32 v4.00 compiler
 * PIC: PIC32CM1216CM00032
 * IDE:  MPLAB X v6.00
 * Created : Feb 2022
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/pic_32_projects
 */

// Section: Included Files
#include <bcm2835.h>
#include "HD44780_LCD_RPI.h"
//#include <time.h>
#include <stdio.h>

// Section: Defines
#define INIT_DELAY 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000

HD44780_LCD_RPI myLCD() ; // instantiate  an object

// Section: Function Prototypes
void Setup(void);
void HelloWorld(void);
void CursorMoveTest(void);
void ScrollTest(void);
void gotoTest(void);
void resetTest(void);
void customChar(void);
void backLightTest(void);

// Section: Main Loop

int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		printf("Library bcm2835 failed init.\r\n");
		return -1;
	}
	
	Setup();

	while (true) {  
		HelloWorld();
		CursorMoveTest();
		ScrollTest();
		gotoTest();
		resetTest();
		customChar();
		backLightTest();
	}
	
	EndTest();
	return 0;
}

// Section :  Functions

void Setup(void) {
	printf("LCD Begin\r\n");
	bcm2835_delay(INIT_DELAY);
	myLCD.PCF8574_LCDInit(CURSOR_ON);
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDBackLightSet(true);
}

void HelloWorld(void) {
	myLCD.PCF8574_LCDGOTO(1, 0);
	myLCD.PCF8574_LCDSendString("Hello");
	myLCD.PCF8574_LCDGOTO(2, 0);
	myLCD.PCF8574_LCDSendString("World"); // Display a string
	myLCD.PCF8574_LCDSendChar('!'); // Display a single character
	bcm2835_delay(DISPLAY_DELAY);
}

void CursorMoveTest(void) {
	myLCD.PCF8574_LCDMoveCursor(MOVE_RIGHT, 2);
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDMoveCursor(MOVE_LEFT, 2);
}

void ScrollTest(void) {
	for (uint8_t i = 0; i < 5; i++) {
		myLCD.PCF8574_LCDScroll(MOVE_RIGHT, 1);
		bcm2835_delay(DISPLAY_DELAY_2);
	}
	myLCD.PCF8574_LCDScroll(MOVE_LEFT, 5);
	bcm2835_delay(DISPLAY_DELAY_2);
}

void gotoTest(void) {
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDGOTO(1, 10);
	myLCD.PCF8574_LCDSendChar('A');
	myLCD.PCF8574_LCDGOTO(2, 2);
	myLCD.PCF8574_LCDSendString("Line");
	bcm2835_delay(DISPLAY_DELAY);
}

void resetTest(void) {
	myLCD.PCF8574_LCDResetScreen(CURSOR_BLINK);
	bcm2835_delay(DISPLAY_DELAY);
}

void customChar(void) {

	// Data to test custom function
	uint8_t bell[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
	uint8_t note[8] = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
	uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
	uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
	uint8_t duck[8] = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
	uint8_t check[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
	uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
	uint8_t retarrow[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

	myLCD.PCF8574_LCDClearScreen();

	// Load the CGRAM with the data , custom characters
	myLCD.PCF8574_LCDCreateCustomChar(0, bell);
	myLCD.PCF8574_LCDCreateCustomChar(1, note);
	myLCD.PCF8574_LCDCreateCustomChar(2, clock);
	myLCD.PCF8574_LCDCreateCustomChar(3, heart);
	myLCD.PCF8574_LCDCreateCustomChar(4, duck);
	myLCD.PCF8574_LCDCreateCustomChar(5, check);
	myLCD.PCF8574_LCDCreateCustomChar(6, cross);
	myLCD.PCF8574_LCDCreateCustomChar(7, retarrow);

	myLCD.PCF8574_LCDGOTO(1, 0);

	// Print out custom characters

	for (uint8_t i = 0; i < 8; i++) {
		myLCD.PCF8574_LCDSendData(i);
	}

	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();
}

void backLightTest(void)
{
	myLCD.PCF8574_LCDBackLightSet(false);
	myLCD.PCF8574_LCDGOTO(2, 1);
	myLCD.PCF8574_LCDSendString("Backlight test");
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_LCDClearScreen();
}

void EndTest()
{
	myLCD.myLCD.PCF8574_LCDDisplayON(false); //Switch off display
	bcm2835_close(); // Close the library
	printf("LCD End\r\n");
}

// EOF

