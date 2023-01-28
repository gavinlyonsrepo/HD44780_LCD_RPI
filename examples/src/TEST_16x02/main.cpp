/*
 * File: main.cpp
 * Description: 
 * This file contains the "main" function for  project, a set of test sequence
 * to test the HD44780 LCD library (16x02) 16 columns 2 lines 
 * Author: Gavin Lyons.
 * Compiler: C++ g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
 * Tested: Raspbian 10, armv7l Linux 5.10.63-v7+ , RPI M3B Rev 1.2
 * Created : Feb 2022
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/HD44780_LCD_RPI
 * 
 * Notes: 
 * 
 * (1) set PCF8574_DebugSet(false) to true in Setup() for debug
 * 
 * (2) For description of entry modes , cursor types, custom characters 
 * and more see here http://dinceraydin.com/lcd/commands.htm
 */

// Section: Included library 
#include <iostream>
#include <bcm2835.h>
#include "HD44780_LCD.hpp"

// Section: Defines
#define DISPLAY_DELAY_1 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000

// Section: Globals
// myLCD(rows , cols , PCF8574 I2C address, I2C speed)
HD44780LCD myLCD(2, 16, 0x27, 626); // instantiate an object

// Section: Function Prototypes
void setup(void);
void helloWorld(void);
void cursorMoveTest(void);
void scrollTest(void);
void gotoTest(void);
void clearLineTest(void); 
void cursorTest(void);
void entryModeTest(void);
void writeNumTest(void);
void customChar(void);
void backLightTest(void);
void endTest(void);

// Section: Main Loop

int main(int argc, char **argv)
{
	if(!bcm2835_init())
	{
		std::cout << "Library bcm2835 failed init" << std::endl ;
		return -1;
	}
	
	setup();
	
	helloWorld(); 
	cursorMoveTest(); 
	scrollTest();
	gotoTest();
	clearLineTest();
	cursorTest(); // Cursor type and screen reset test(4 off ) check, 
				  //   cursor mode is changed by calling reset. 
	entryModeTest(); // Text entry mode (4 off ) Note: if screen is reset
					// non default entry mode setting will also be reset
	writeNumTest(); // Print numerical data using print() test
	customChar();   // Custom character from CGRAM test
	backLightTest();
	
	endTest();
	
	return 0;
} // END of main

// Section :  Functions

void setup(void) {
	std::cout <<  "LCD Begin" << std::endl;  
	myLCD.PCF8574_LCD_I2C_ON();
	bcm2835_delay(DISPLAY_DELAY_1);
	myLCD.PCF8574_LCDInit(LCDCursorTypeOn);
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_DebugSet(false); // Set to true to turn on debug mode
}

void helloWorld(void) {
	char teststr1[] = "Hello";
	char teststr2[] = "World";
	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo , 0);
	myLCD.PCF8574_LCDSendString(teststr2); // Display a string
	myLCD.PCF8574_LCDSendChar('!'); // Display a single character
	bcm2835_delay(DISPLAY_DELAY_1);
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
	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 10);
	myLCD.PCF8574_LCDSendChar('A');
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo , 2);
	myLCD.PCF8574_LCDSendString(teststr3);
	bcm2835_delay(DISPLAY_DELAY);
}

void clearLineTest(void)
{
	myLCD.PCF8574_LCDClearLine(LCDLineNumberTwo );
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(LCDLineNumberOne);
	bcm2835_delay(DISPLAY_DELAY_2);
}

void cursorTest(void) {
	char teststr1[] = "Cursor no 4";
	char teststr2[] = "Cursor no 1";
	char teststr3[] = "Cursor no 2";
	char teststr4[] = "Cursor no 3";
	
	myLCD.PCF8574_LCDResetScreen(LCDCursorTypeOnBlink); //type 4 cursor
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(LCDCursorTypeOff); //type 1 cursor
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr2);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(LCDCursorTypeBlink); //type 2 cursor
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr3);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(LCDCursorTypeOn); // Back to initial state , type 3
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr4);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(LCDLineNumberTwo);
}

void writeNumTest()
{
	int numPos = 193;
	int numNeg = -8582;
	double myPI = 3.1456;
	
	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
	myLCD.print(numPos);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo , 0);
	myLCD.print(numNeg);
	myLCD.PCF8574_LCDMoveCursor(LCDMoveRight, 2);
	myLCD.print(myPI,3);  
	
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();
	
	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0); // 11
	myLCD.print(11);
	myLCD.PCF8574_LCDMoveCursor(LCDMoveRight, 2);  // 13
	myLCD.print(11,OCT);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo , 0); // B
	myLCD.print(11, HEX);
	myLCD.PCF8574_LCDMoveCursor(LCDMoveRight, 2); // 1011
	myLCD.print(11,BIN);
	
	bcm2835_delay(DISPLAY_DELAY);
}

void customChar(void) {

	// Data to test custom function
	uint8_t bellChar[8] = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
	uint8_t noteChar[8] = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
	uint8_t clockChar[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
	uint8_t heartChar[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
	uint8_t duckChar[8] = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
	uint8_t checkChar[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
	uint8_t crossChar[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
	uint8_t enterChar[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

	myLCD.PCF8574_LCDClearScreen();

	// Load the CGRAM with the data , custom characters
	// location argument must be 0 to 7 
	myLCD.PCF8574_LCDCreateCustomChar(0, bellChar); // Location 0
	myLCD.PCF8574_LCDCreateCustomChar(1, noteChar);
	myLCD.PCF8574_LCDCreateCustomChar(2, clockChar);
	myLCD.PCF8574_LCDCreateCustomChar(3, heartChar);
	myLCD.PCF8574_LCDCreateCustomChar(4, duckChar);
	myLCD.PCF8574_LCDCreateCustomChar(5, checkChar);
	myLCD.PCF8574_LCDCreateCustomChar(6, crossChar);
	myLCD.PCF8574_LCDCreateCustomChar(7, enterChar); // Location 7

	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);

	// Print out custom characters from 
	// CGRAM locations 0-7
	// location argument must be 0 to 7 
	for (uint8_t i = 0; i < 8; i++) {
		myLCD.PCF8574_LCDPrintCustomChar(i);
		myLCD.PCF8574_LCDMoveCursor(LCDMoveRight, 1);
	}

	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();
}

void backLightTest(void)
{
	char teststr4[] = "Back Light";
	// Needs another command/data before it changes Light
	myLCD.PCF8574_LCDBackLightSet(false); 
	
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo , 1);
	myLCD.PCF8574_LCDSendString(teststr4);
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_LCDClearScreen();
}

void entryModeTest(void) {

	char teststr8[] = "1234";
	
	myLCD.PCF8574_LCDChangeEntryMode(LCDEntryModeOne);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // <-C4321
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(LCDEntryModeTwo);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // C4321->
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(LCDEntryModeFour);
	myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // <-1234C
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(LCDEntryModeThree); // Set back to default entry mode
	myLCD.PCF8574_LCDClearScreenCmd();
	bcm2835_delay(DISPLAY_DELAY_1);
}

void endTest()
{
	myLCD.PCF8574_LCDDisplayON(false); //Switch off display
	myLCD.PCF8574_LCD_I2C_OFF();
	bcm2835_close(); // Close the library
	std::cout << "LCD End" << std::endl ;
}

// *** EOF ***

