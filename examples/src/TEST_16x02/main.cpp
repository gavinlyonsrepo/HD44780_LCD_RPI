/*!
	@file    main.cpp
	@author   Gavin Lyons
	@brief 
		 This file contains the "main" function for  project, a set of test sequence
 		 to test the HD44780_LCD_PCF8574 pico rp2040  library. 16X02 display.
 		-# set PCF8574_DebugSet(false) to true in Setup() for debug
 		-# Tested: Raspbian 10, armv7l Linux 5.10.63-v7+ , RPI M3B Rev 1.2
 		-# Compiler: C++ g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
	@note 
		-# Test 1 :: Hello world
		-# Test 2 :: Move the cursor test
		-# Test 3 :: Scroll the display test
		-# Test 4 :: Test GOTO method
		-# Test 5 :: Test clear a line method
		-# Test 6 :: Cursor type (4 off) and screen reset test, Cursor mode is changed with a reset.
		-# Test 7 :: Text entry mode (4 off) if screen is reset the entry mode will be reset to default
		-# Test 8 :: Print numerical data using print() method
		-# Test 9 :: Custom character's from the CGRAM test
		-# Test 10 :: Backlight test.
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
HD44780LCD myLCD(2, 16, 0x27, 0); // instantiate an object

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
	cursorTest();
	entryModeTest();
	writeNumTest();
	customChar();
	backLightTest();

	endTest();

	return 0;
} // END of main

// Section :  Functions

void setup(void) {
	std::cout <<  "LCD Begin" << std::endl;
	myLCD.PCF8574_LCD_I2C_ON();
	bcm2835_delay(DISPLAY_DELAY_1);
	myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn);
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_DebugSet(false); // Set to true to turn on debug mode
	
	// print out  flag status( Note optional)
	std::cout << "Debug status is : " << (myLCD.PCF8574_DebugGet() ? "On" : "Off") << std::endl ;
	std::cout <<  "Backlight status is : " << (myLCD.PCF8574_LCDBackLightGet() ? "On" : "Off")<< std::endl ;
}

void helloWorld(void) {
	char teststr1[] = "Hello";
	char teststr2[] = "World";
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.PCF8574_LCDSendString(teststr2); // Display a string
	myLCD.PCF8574_LCDSendChar('!'); // Display a single character
	bcm2835_delay(DISPLAY_DELAY_1);
}

void cursorMoveTest(void) {
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 3);
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveLeft, 1);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveLeft, 2);
	bcm2835_delay(DISPLAY_DELAY_2);
}

void scrollTest(void) {
	for (uint8_t i = 0; i < 5; i++) {
		myLCD.PCF8574_LCDScroll(myLCD.LCDMoveRight, 1);
		bcm2835_delay(DISPLAY_DELAY_2);
	}
	myLCD.PCF8574_LCDScroll(myLCD.LCDMoveLeft, 5);
	bcm2835_delay(DISPLAY_DELAY_2);
}

void gotoTest(void) {
	char teststr3[] = "Line 2";
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 10);
	myLCD.PCF8574_LCDSendChar('A');
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 2);
	myLCD.PCF8574_LCDSendString(teststr3);
	bcm2835_delay(DISPLAY_DELAY);
}

void clearLineTest(void)
{
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo );
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberOne);
	bcm2835_delay(DISPLAY_DELAY_2);
}

void cursorTest(void) {
	char teststr1[] = "Cursor no 4";
	char teststr2[] = "Cursor no 1";
	char teststr3[] = "Cursor no 2";
	char teststr4[] = "Cursor no 3";

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeOnBlink); //type 4 cursor
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeOff); //type 1 cursor
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr2);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeBlink); //type 2 cursor
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr3);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeOn); // Back to initial state , type 3
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr4);
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);
}

void writeNumTest()
{
	int numPos = 193;
	int numNeg = -8582;
	double myPI = 3.1456;

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(numPos);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.print(numNeg);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2);
	myLCD.print(myPI,3);

	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0); // 11
	myLCD.print(11);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2);  // 13
	myLCD.print(11,OCT);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0); // B
	myLCD.print(11, HEX);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2); // 1011
	myLCD.print(11,BIN);

	bcm2835_delay(DISPLAY_DELAY);
}

void customChar(void) {

	uint8_t index = 0; //  Character generator RAM location ,0-7 ,64 bytes
	
	// custom characters data to test custom character function
	uint8_t symbolData[8][8] = {
		{0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00}, // bell
		{0x02, 0x03, 0x02, 0x0E, 0x1E, 0x0C, 0x00, 0x00},  // Note
		{0x00, 0x0E, 0x15, 0x17, 0x11, 0x0E, 0x00, 0x00},  // clock
		{0x00, 0x0C, 0x1D, 0x0F, 0x0F, 0x06, 0x00, 0x00},  // duck
		{0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00, 0x00},  // check
		{0x00, 0x1B, 0x0E, 0x04, 0x0E, 0x1B, 0x00, 0x00},  // cross
		{0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00},  // heart
		{0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04, 0x00}   // return arrow
	};

	myLCD.PCF8574_LCDClearScreen();

	// Load the CGRAM with the data , custom characters
	// location argument must be 0 to 7  , load the data into LCD memory
 	 for (uint8_t index  = 0; index  < 8; index++) {
	 	 myLCD.PCF8574_LCDCreateCustomChar(index , symbolData[index]);
	}
 	 myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);

	// Print out custom characters from 
	// CGRAM locations 0-7 , location argument must be 0 to 7 
	for (index = 0; index < 8; index ++) {
		myLCD.PCF8574_LCDPrintCustomChar(index);
		myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 1);
		bcm2835_delay(500);
	}

	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();
}

void backLightTest(void)
{
	char teststr4[] = "Back Light";
	// Needs another command/data before it changes Light
	myLCD.PCF8574_LCDBackLightSet(false);

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 1);
	myLCD.PCF8574_LCDSendString(teststr4);
	bcm2835_delay(DISPLAY_DELAY);
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_LCDClearScreen();
}

void entryModeTest(void) {

	char teststr8[] = "1234";

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeOne);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // <-C4321
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeTwo);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // C4321->
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeFour);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // <-1234C
	bcm2835_delay(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeThree); // Set back to default entry mode
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

