/*
 * File: HD44780_I2C_lcd.cpp
 * Description: 
 * HD44780-based character LCD 16x02 I2C(PCF8574) library source file for RPI
 * Author: Gavin Lyons.
 * Compiler: C++ g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
 * Tested: Raspbian 10, armv7l Linux 5.10.63-v7+ , RPI M3B Rev 1.2
 * Created : Feb 2022
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/HD44780_LCD_RPI
 */

// Section : Includes
#include "HD44780_LCD.h"

// Section : constructor

HD44780LCD  :: HD44780LCD()
{}

// Section : Functions

//  Func Desc: Send data byte to  LCD via I2C
//  Param1: data byte
void HD44780LCD::PCF8574_LCDSendData(unsigned char data) {
	unsigned char data_l, data_u;
	char data_I2C[4];
	int txCount=0;
	
	data_l = (data << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	data_u = data & 0xf0; //select upper nibble
	data_I2C[0] = data_u | (DATA_BYTE_ON & LCDBACKLIGHT); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	data_I2C[1] = data_u | (DATA_BYTE_OFF & LCDBACKLIGHT); //enable=0 and rs =1 1001 YYYY-X-en-X-rs
	data_I2C[2] = data_l | (DATA_BYTE_ON & LCDBACKLIGHT); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	data_I2C[3] = data_l | (DATA_BYTE_OFF &  LCDBACKLIGHT); //enable=0 and rs =1 1001 YYYY-X-en-X-rs
	
	// bcm2835I2CReasonCodes , BCM2835_I2C_REASON_OK 0x00 = Success
	uint8_t ReasonCodes = bcm2835_i2c_write(data_I2C, 4);
	
	while(ReasonCodes != 0)  
	{
		if (DEBUGON == true)
		{
			printf("I2C Error bcm2835I2CReasonCodes %d : %d \n", ReasonCodes,txCount++);
		}
		ReasonCodes = bcm2835_i2c_write(data_I2C, 4);
		if(ReasonCodes == 0) break;
	}

}

//  Func Desc: Send command byte to  lcd
//  Param1: command byte

void HD44780LCD::PCF8574_LCDSendCmd(unsigned char cmd) {
	unsigned char cmd_l, cmd_u;
	char cmd_I2C[4];
	int txCount=0;
		
	cmd_l = (cmd << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	cmd_u = cmd & 0xf0; //select upper nibble
	cmd_I2C[0] = cmd_u | (CMD_BYTE_ON & LCDBACKLIGHT); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmd_I2C[1] = cmd_u | (CMD_BYTE_OFF & LCDBACKLIGHT); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
	cmd_I2C[2] = cmd_l | (CMD_BYTE_ON & LCDBACKLIGHT); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmd_I2C[3] = cmd_l | (CMD_BYTE_OFF & LCDBACKLIGHT); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
	
	// bcm2835I2CReasonCodes, BCM2835_I2C_REASON_OK 0x00 = Success
	uint8_t ReasonCodes = bcm2835_i2c_write(cmd_I2C, 4);

	while(ReasonCodes != 0)
	{
		if (DEBUGON == true)
		{
			printf("I2C Error bcm2835I2CReasonCodes %d : %d \n", ReasonCodes,txCount++);
			bcm2835_delay(100);
		}
		ReasonCodes = bcm2835_i2c_write(cmd_I2C, 4);
		if(ReasonCodes == 0) break;
	}
}

// Func Desc: Clear a line by writing spaces to every position
// Param1: uint8_t lineNo, row number 1-2

void HD44780LCD::PCF8574_LCDClearLine(uint8_t lineNo) {
	if (lineNo == 1) {
		PCF8574_LCDSendCmd(LCD_LINE_ADR1);
	} else if (lineNo == 2) {
		PCF8574_LCDSendCmd(LCD_LINE_ADR2);
	} else {
		return;
	}

	for (uint8_t i = 0; i < 16; i++) {
		PCF8574_LCDSendData(' ');
	}
}

// Func Desc: Clear screen by writing spaces to every position

void HD44780LCD::PCF8574_LCDClearScreen(void) {
	PCF8574_LCDClearLine(1);
	PCF8574_LCDClearLine(2);
}


// Func Desc: Reset screen 
// Param1: enum LCD_CURSOR_TYPE_e cursor type, 4 choices

void HD44780LCD::PCF8574_LCDResetScreen(LCD_CURSOR_TYPE_e CursorType) {
	PCF8574_LCDSendCmd(LCD_MODE_4BIT);
	PCF8574_LCDSendCmd(LCD_DISPLAY_ON);
	PCF8574_LCDSendCmd(CursorType);
	PCF8574_LCDSendCmd(LCD_CLRSCR);
	PCF8574_LCDSendCmd(LCD_INC_MODE);
}


// Func Desc: Turn Screen on and off 
// Param1: passed bool, True = display on , false = display off

void HD44780LCD::PCF8574_LCDDisplayON(bool OnOff) {
	OnOff ? PCF8574_LCDSendCmd(LCD_DISPLAY_ON) : PCF8574_LCDSendCmd(LCD_DISPLAY_OFF);

}


// Func Desc: Initialise LCD
// Param1: enum LCD_CURSOR_TYPE_e cursor type, 4 choices. 

void HD44780LCD::PCF8574_LCDInit(LCD_CURSOR_TYPE_e CursorType) {

	bcm2835_delay(15);
	PCF8574_LCDSendCmd(LCD_HOME);
	bcm2835_delay(5);
	PCF8574_LCDSendCmd(LCD_HOME);
	bcm2835_delay(5);
	PCF8574_LCDSendCmd(LCD_HOME);
	bcm2835_delay(5);
	PCF8574_LCDSendCmd(LCD_MODE_4BIT);
	PCF8574_LCDSendCmd(LCD_DISPLAY_ON);
	PCF8574_LCDSendCmd(CursorType);
	PCF8574_LCDSendCmd(LCD_INC_MODE);
	PCF8574_LCDSendCmd(LCD_CLRSCR);
   
}

// Func Desc: Send string to LCD
// Param1: Pointer to the char array

void HD44780LCD::PCF8574_LCDSendString(char *str) {
	while (*str) PCF8574_LCDSendData(*str++);
}


// Func Desc: Sends a character to screen , simply wraps SendData command.
// Param1: Character to display
void HD44780LCD::PCF8574_LCDSendChar(char data) {
	PCF8574_LCDSendData(data);
}

// Func Desc: Moves cursor 
// Param1. enum LCD_DIRECTION_TYPE_e left or right 
// Param2. uint8_t number of spaces to move

void HD44780LCD::PCF8574_LCDMoveCursor(LCD_DIRECTION_TYPE_e direction, uint8_t moveSize) {
	uint8_t i = 0;
	if (direction == 1) {
		for (i = 0; i < moveSize; i++) {
			PCF8574_LCDSendCmd(LCD_MOV_CURSOR_RIGHT);
		}
	} else {
		for (i = 0; i < moveSize; i++) {
			PCF8574_LCDSendCmd(LCD_MOV_CURSOR_LEFT);
		}
	}

}

// Func Desc: Scrolls screen 
// Param1. enum LCD_DIRECTION_TYPE_e , left or right 
// Param2. uint8_t number of spaces to scroll

void HD44780LCD::PCF8574_LCDScroll(LCD_DIRECTION_TYPE_e direction, uint8_t ScrollSize) {
	uint8_t i = 0;
	if (direction == 1) {
		for (i = 0; i < ScrollSize; i++) {
			PCF8574_LCDSendCmd(LCD_SCROLL_RIGHT);
		}
	} else {
		for (i = 0; i < ScrollSize; i++) {
			PCF8574_LCDSendCmd(LCD_SCROLL_LEFT);
		}
	}

}
// Func Desc: moves cursor
// Param1: uint8_t row 1-2 
// Param2: uint8_t col 0-15
void HD44780LCD::PCF8574_LCDGOTO(uint8_t row, uint8_t col) {
	switch (row) {
		case 1:
			PCF8574_LCDSendCmd(LCD_LINE_ADR1 | col);
			break;

		case 2:
			PCF8574_LCDSendCmd(LCD_LINE_ADR2 | col);
			break;

		default:
			asm ("nop");
	}
}

// Func Desc: Saves a custom character to a location in CG_RAM
// Param1: CG_RAM location 0-7 we only have 8 locations 0-7
// Param2: An array of 8 bytes representing a custom character data
void HD44780LCD::PCF8574_LCDCreateCustomChar(uint8_t location, uint8_t * charmap)
{
	PCF8574_LCDSendCmd(LCD_CG_RAM | (location<<3));
	for (uint8_t i=0; i<8; i++) {
		PCF8574_LCDSendData(charmap[i]);
	}
}

// Func Desc: Turn LED backlight on and off 
// Param1: passed bool True = LED on , false = display LED off
// Note: another command must be issued before takes effect.
void HD44780LCD::PCF8574_LCDBackLightSet(bool OnOff)
{
	 OnOff ? (LCDBACKLIGHT = LCD_BACKLIGHTON_MASK) : (LCDBACKLIGHT = LCD_BACKLIGHTOFF_MASK);
}

// Func Desc: Turn DEBUG mode on 
// Param1: passed bool True = debug on , false = debug off
// Note: prints out printf statements, if ON and if errors occur
void HD44780LCD::PCF8574_DebugSet(bool OnOff)
{
	 OnOff ? (DEBUGON  = true) : (DEBUGON  = false);
}

// Func Desc: Setup I2C settings 
void HD44780LCD::PCF8574_LCD_I2C_ON()
{
	// Start I2C operations. Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL) 
	// to alternate function ALT0, which enables those pins for I2C interface. 
	if (!bcm2835_i2c_begin())
	{
		if (DEBUGON == true)
		{
			printf("LCD Error: Cannot start bcm2835 I2C \n");
		}
		return;
	}
	//i2c address
	bcm2835_i2c_setSlaveAddress(LCD_SLAVE_ADDRESS_I2C);  
	
	// (1) BCM2835_I2C_CLOCK_DIVIDER_626  ：622 = 2.504us = 399.3610 kHz
	//Clock divided is based on nominal base clock rate of 250MHz
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
	
	// (2) or use set_baudrate instead of clockdivder
	//bcm2835_i2c_set_baudrate(100000); //100k baudrate
}

// Func Desc: End I2C operations. 
// I2C pins P1-03 (SDA) and P1-05 (SCL) 
// are returned to their default INPUT behaviour. 
void HD44780LCD::PCF8574_LCD_I2C_OFF(void)
{
	bcm2835_i2c_end();
}

// **** EOF ****
