/*
 * File: main.cpp
 * Description: 
 * This file contains the "main" function for  project, a set of test sequence
 * to test the HD44780 LCD library
 * Author: Gavin Lyons.
 * Compiler: C++ g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
 * Tested: Raspbian 10, armv7l Linux 5.10.63-v7+ , RPI M3B Rev 1.2
 * Created : Feb 2022
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/HD44780_LCD_RPI
 * 
 * Note: set PCF8574_DebugSet(false) to true in Setup() for debug
 */

// Section: Included library 
#include <bcm2835.h>
#include "HD44780_LCD.h"

// Section: Defines
#define INIT_DELAY 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000

// Section: Globals
// myLCD(rows , cols , I2C address)
HD44780LCD myLCD( 2, 16, 0x27); // instantiate an object

// Section: Function Prototypes
void setup(void);
void helloWorld(void);
void cursorMoveTest(void);
void scrollTest(void);
void gotoTest(void);
void clearLineTest(void);
void resetTest(void);
void writeNumTest(void);
void customChar(void);
void backLightTest(void);
void endTest(void);

// Section: Main Loop

int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		printf("Library bcm2835 failed init.\r\n");
		return -1;
	}
	
	setup();
	
	helloWorld();
	cursorMoveTest();
	scrollTest();
	gotoTest();
	clearLineTest();
	resetTest();
	writeNumTest();
	customChar();
	backLightTest();
	endTest();
	
	return 0;
} // END of main

// Section :  Functions

void setup(void) {
	printf("LCD Begin\r\n");
	myLCD.PCF8574_LCD_I2C_ON();
	bcm2835_delay(INIT_DELAY);
	myLCD.PCF8574_LCDInit(LCDCursorTypeOn);
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_DebugSet(false); // Set to true to turn on debug mode
}

void helloWorld(void) {
	char teststr1[] = "Hello";
	char teststr2[] = "World";
	myLCD.PCF8574_LCDGOTO(1, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	myLCD.PCF8574_LCDGOTO(2, 0);
	myLCD.PCF8574_LCDSendString(teststr2); // Display a string
	myLCD.PCF8574_LCDSendChar('!'); // Display a single character
	bcm2835_delay(DISPLAY_DELAY);
}

void cursorMoveTest(void) {
	myLCD.PCF8574_LCDMoveCursor(LCDMoveRight, 2);
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDMoveCursor(LCDMoveLeft, 2);
}

void scrollTest(void) {
	for (uint8_t i = 0; i < 5; i++) {
		myLCD.PCF8574_LCDScroll(LCDMoveRight, 1);
		bcm2835_delay(DISPLAY_DELAY_2);
	}
	myLCD.PCF8574_LCDScroll(LCDMoveLeft, 5);
	bcm2835_delay(DISPLAY_DELAY_2);
}

void gotoTest(void) {
	char teststr3[] = "Line 2";
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDGOTO(1, 10);
	myLCD.PCF8574_LCDSendChar('A');
	myLCD.PCF8574_LCDGOTO(2, 2);
	myLCD.PCF8574_LCDSendString(teststr3);
	bcm2835_delay(DISPLAY_DELAY);
}

void clearLineTest(void)
{
myLCD.PCF8574_LCDClearLine(2);
bcm2835_delay(DISPLAY_DELAY_2);
myLCD.PCF8574_LCDClearLine(1);
bcm2835_delay(DISPLAY_DELAY_2);
}

void resetTest(void) {
	myLCD.PCF8574_LCDResetScreen(LCDCursorTypeBlink);
	bcm2835_delay(DISPLAY_DELAY);
}

void writeNumTest()
{
	int numPos = 193;
	int numNeg = -8582;
	double myPI = 3.1456;
	
	myLCD.PCF8574_LCDGOTO(1, 0);
	myLCD.print(numPos);
	myLCD.PCF8574_LCDGOTO(2, 0);
	myLCD.print(numNeg);
	myLCD.PCF8574_LCDMoveCursor(LCDMoveRight, 2);
	myLCD.print(myPI,4);
	
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
	myLCD.PCF8574_LCDCreateCustomChar(0, bell); // Location 0
	myLCD.PCF8574_LCDCreateCustomChar(1, note);
	myLCD.PCF8574_LCDCreateCustomChar(2, clock);
	myLCD.PCF8574_LCDCreateCustomChar(3, heart);
	myLCD.PCF8574_LCDCreateCustomChar(4, duck);
	myLCD.PCF8574_LCDCreateCustomChar(5, check);
	myLCD.PCF8574_LCDCreateCustomChar(6, cross);
	myLCD.PCF8574_LCDCreateCustomChar(7, retarrow); // Location 7

	myLCD.PCF8574_LCDGOTO(1, 0);

	// Print out custom characters from 
	// CGRAM locations 0-7

	for (uint8_t i = 0; i < 8; i++) {
		myLCD.PCF8574_LCDPrintCustomChar(i);
	}

	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();
}

void backLightTest(void)
{
	char teststr4[] = "Back Light";
	// Needs another command/data before it changes Light
	myLCD.PCF8574_LCDBackLightSet(false); 
	myLCD.PCF8574_LCDGOTO(2, 1);
	myLCD.PCF8574_LCDSendString(teststr4);
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_LCDClearScreen();
}

void endTest()
{
	myLCD.PCF8574_LCDDisplayON(false); //Switch off display
	myLCD.PCF8574_LCD_I2C_OFF();
	bcm2835_close(); // Close the library
	printf("LCD End\r\n");
}

// *** EOF ***

