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
#include "HD44780_LCD.hpp"

// Section : constructor
// Param 1:: number of rows in LCD
// Param 2:: number of columns in LCD
// Param 3:: I2C address 
// Param 4:: I2C speed default = 0 
// 0 = //bcm2835_i2c_set_baudrate(100000); //100k baudrate
// > 0 = BCM2835_I2C_CLOCK_DIVIDER, choices = 2500 , 622 , 150 , 148

HD44780LCD::HD44780LCD(uint8_t NumRow, uint8_t NumCol, uint8_t I2Caddress, uint16_t I2Cspeed)
{
	_NumRowsLCD = NumRow;
	_NumColsLCD = NumCol;
	_LCDSlaveAddresI2C  = I2Caddress;
	_LCDSpeedI2C = I2Cspeed;
}

// Section : Functions

//  Func Desc: Send data byte to  LCD via I2C
//  Param1: data byte
void HD44780LCD::PCF8574_LCDSendData(unsigned char data) {
	unsigned char data_l, data_u;
	char data_I2C[4];
	int txCount=0;

	data_l = (data << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	data_u = data & 0xf0; //select upper nibble
	data_I2C[0] = data_u | (LCD_DATA_BYTE_ON & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	data_I2C[1] = data_u | (LCD_DATA_BYTE_OFF & _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs
	data_I2C[2] = data_l | (LCD_DATA_BYTE_ON & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	data_I2C[3] = data_l | (LCD_DATA_BYTE_OFF &  _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs

	// bcm2835I2CReasonCodes , BCM2835_I2C_REASON_OK 0x00 = Success
	uint8_t ReasonCodes = bcm2835_i2c_write(data_I2C, 4);

	while(ReasonCodes != 0)
	{
		if (_DebugON == true)
		{
			std::cout << "I2C Error Data bcm2835I2CReasonCodes : " << +ReasonCodes << std::endl;
			std::cout << txCount++ << std::endl;
			bcm2835_delay(100);
		}
		ReasonCodes = bcm2835_i2c_write(data_I2C, 4); // retransmit
		bcm2835_delay(25);
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
	cmd_I2C[0] = cmd_u | (LCD_CMD_BYTE_ON & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmd_I2C[1] = cmd_u | (LCD_CMD_BYTE_OFF & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
	cmd_I2C[2] = cmd_l | (LCD_CMD_BYTE_ON & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmd_I2C[3] = cmd_l | (LCD_CMD_BYTE_OFF & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0

	// bcm2835I2CReasonCodes, BCM2835_I2C_REASON_OK 0x00 = Success
	uint8_t ReasonCodes = bcm2835_i2c_write(cmd_I2C, 4);

	while(ReasonCodes != 0)
	{
		if (_DebugON == true)
		{
			std::cout << "I2C Error Data bcm2835I2CReasonCodes : " << +ReasonCodes << std::endl;
			std::cout << txCount++ << std::endl;
			bcm2835_delay(100);
		}
		ReasonCodes = bcm2835_i2c_write(cmd_I2C, 4); // retransmit
		bcm2835_delay(25);
		if(ReasonCodes == 0) break;
	}
}

// Func Desc: Clear a line by writing spaces to every position
// Param1: enum LCDLineNumber_e lineNo  1-4

void HD44780LCD::PCF8574_LCDClearLine(LCDLineNumber_e lineNo) {

	switch (lineNo) {
		case LCDLineNumberOne:
			PCF8574_LCDSendCmd(LCD_LINE_ADR1);
			break;
		case LCDLineNumberTwo:
			PCF8574_LCDSendCmd(LCD_LINE_ADR2);
			break;
		case LCDLineNumberThree:
			if (_NumColsLCD == 20)
				PCF8574_LCDSendCmd(LCD_LINE_ADR3_20);
			else
				PCF8574_LCDSendCmd(LCD_LINE_ADR3_16);
			break;
		case LCDLineNumberFour:
			if (_NumColsLCD == 20)
				PCF8574_LCDSendCmd(LCD_LINE_ADR4_20);
			else
				PCF8574_LCDSendCmd(LCD_LINE_ADR4_16);
			break;
	}

	for (uint8_t i = 0; i < _NumColsLCD; i++) {
		PCF8574_LCDSendData(' ');
	}
}

// Func Desc: Clear screen by writing spaces to every position
// Note : See also LCDClearScreenCmd for software command  clear alternative.

void HD44780LCD::PCF8574_LCDClearScreen(void) {
	if (_NumRowsLCD < 1 || _NumRowsLCD >4)
	{
			if (_DebugON == true)
			{
				std::cout << "Number of rows invalid, must be 1-4" << _NumRowsLCD << std::endl;
				return;
			}
	}

	PCF8574_LCDClearLine(LCDLineNumberOne);
	
	if (_NumRowsLCD >= 2)
		PCF8574_LCDClearLine(LCDLineNumberTwo);
	if (_NumRowsLCD >= 3)
		PCF8574_LCDClearLine(LCDLineNumberThree);
	if (_NumRowsLCD == 4)
		PCF8574_LCDClearLine(LCDLineNumberFour);
}


// Func Desc: Reset screen
// Param1: enum LCDCursorType_e cursor type, 4 choices

void HD44780LCD::PCF8574_LCDResetScreen(LCDCursorType_e CursorType) {
	PCF8574_LCDSendCmd(LCD_MODE_4BIT);
	PCF8574_LCDSendCmd(LCD_DISPLAY_ON);
	PCF8574_LCDSendCmd(CursorType);
	PCF8574_LCDSendCmd(LCD_CLRSCR);
	PCF8574_LCDSendCmd(LCDEntryModeThree);
	bcm2835_delay(5);
}


// Func Desc: Turn Screen on and off
// Param1: passed bool, True = display on , false = display off

void HD44780LCD::PCF8574_LCDDisplayON(bool OnOff) {
	OnOff ? PCF8574_LCDSendCmd(LCD_DISPLAY_ON) : PCF8574_LCDSendCmd(LCD_DISPLAY_OFF);
	bcm2835_delay(5);
}


// Func Desc: Initialise LCD
// Param1: enum LCDCursorType_e cursor type, 4 choices.

void HD44780LCD::PCF8574_LCDInit(LCDCursorType_e CursorType) {

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
	PCF8574_LCDSendCmd(LCDEntryModeThree);
	PCF8574_LCDSendCmd(LCD_CLRSCR);
	bcm2835_delay(5);
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
// Param1. enum LCDDirectionType_e left or right
// Param2. uint8_t number of spaces to move

void HD44780LCD::PCF8574_LCDMoveCursor(LCDDirectionType_e direction, uint8_t moveSize) {
	uint8_t i = 0;
	if (direction == LCDMoveRight) {
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
// Param1. enum LCDDirectionType_e , left or right
// Param2. uint8_t number of spaces to scroll

void HD44780LCD::PCF8574_LCDScroll(LCDDirectionType_e direction, uint8_t ScrollSize) {
	uint8_t i = 0;
	if (direction == LCDMoveRight) {
		for (i = 0; i < ScrollSize; i++) {
			PCF8574_LCDSendCmd(LCD_SCROLL_RIGHT);
		}
	} else {
		for (i = 0; i < ScrollSize; i++) {
			PCF8574_LCDSendCmd(LCD_SCROLL_LEFT);
		}
	}

}
// Func Desc: moves cursor to an x , y position on display.
// Param1: enum LCDLineNumber_e  row 1-4
// Param2: uint8_t col 0-15
void HD44780LCD::PCF8574_LCDGOTO(LCDLineNumber_e line, uint8_t col) {
	switch (line) {
		case LCDLineNumberOne:
			PCF8574_LCDSendCmd(LCD_LINE_ADR1 | col);
			break;
		case LCDLineNumberTwo:
			PCF8574_LCDSendCmd(LCD_LINE_ADR2 | col);
			break;
		case LCDLineNumberThree:
			if (_NumColsLCD == 20)
				PCF8574_LCDSendCmd(LCD_LINE_ADR3_20 + col);
			else
				PCF8574_LCDSendCmd(LCD_LINE_ADR3_16 | col);
			break;
		case LCDLineNumberFour:
			if (_NumColsLCD == 20)
				PCF8574_LCDSendCmd(LCD_LINE_ADR4_20 + col);
			else
				PCF8574_LCDSendCmd(LCD_LINE_ADR4_16 | col);
			;
			break;
	}
}

// Func Desc: Saves a custom character to a location in CG_RAM
// Param1: CG_RAM location 0-7 we only have 8 locations 0-7
// Param2: An array of 8 bytes representing a custom character data
void HD44780LCD::PCF8574_LCDCreateCustomChar(uint8_t location, uint8_t * charmap)
{
	 if (location >= 8) {return;}
	PCF8574_LCDSendCmd(LCD_CG_RAM | (location<<3));
	for (uint8_t i=0; i<8; i++) {
		PCF8574_LCDSendData(charmap[i]);
	}
}

// Func Desc: Turn LED backlight on and off
// Param1: passed bool True = LED on , false = display LED off
// Note: another data or command must be issued before it takes effect.
void HD44780LCD::PCF8574_LCDBackLightSet(bool OnOff)
{
	 OnOff ? (_LCDBackLight= LCD_BACKLIGHTON_MASK) : (_LCDBackLight= LCD_BACKLIGHTOFF_MASK);
}

// Func Desc: Turn DEBUG mode on
// Param1: passed bool True = debug on , false = debug off
// Note: prints out printf statements, if ON and if errors occur
void HD44780LCD::PCF8574_DebugSet(bool OnOff)
{
	 OnOff ? (_DebugON  = true) : (_DebugON  = false);
}

// Func Desc: Setup I2C settings
void HD44780LCD::PCF8574_LCD_I2C_ON()
{
	// Start I2C operations. Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL)
	// to alternate function ALT0, which enables those pins for I2C interface.
	if (!bcm2835_i2c_begin())
	{
		if (_DebugON == true)
		{
			std::cout << "LCD Error: Cannot start bcm2835 I2C " << std::endl;
		}
		return;
	}
	bcm2835_i2c_setSlaveAddress(_LCDSlaveAddresI2C);  //i2c address
	
	if ( _LCDSpeedI2C > 0)  
	{
		// BCM2835_I2C_CLOCK_DIVIDER enum choice 2500 622 150 148
		// Clock divided is based on nominal base clock rate of 250MHz
		bcm2835_i2c_setClockDivider(_LCDSpeedI2C);
	} else{
		// default or use set_baudrate instead of clockdivder 100k
		bcm2835_i2c_set_baudrate(100000); //100k baudrate
	}
}

// Func Desc: End I2C operations.
// I2C pins P1-03 (SDA) and P1-05 (SCL)
// are returned to their default INPUT behaviour.
void HD44780LCD::PCF8574_LCD_I2C_OFF(void)
{
	bcm2835_i2c_end();
}

// Print out a customer character from CGRAM
// Param1 CGRAM location 0-7
void HD44780LCD::PCF8574_LCDPrintCustomChar(uint8_t location)
{
	if (location >= 8) {return;}
	PCF8574_LCDSendData(location);
}

// Called by print class, used to print out numerical data types etc
size_t HD44780LCD::write(uint8_t c)
{
	PCF8574_LCDSendChar(c) ;
	return 1;
}

// Clear display using software command , set cursor position to zero
// See also LCDClearScreen for manual clear

void HD44780LCD::PCF8574_LCDClearScreenCmd(void) {
	PCF8574_LCDSendCmd(LCD_CLRSCR);
	 bcm2835_delay(3); // Requires a delay
}

// Set Cursor position to zero

void HD44780LCD::PCF8574_LCDHome(void) {
	PCF8574_LCDSendCmd(LCD_HOME);
	bcm2835_delay(3); // Requires a delay
}

// Change entry mode 
// Param1 enum LCDEntryMode_e 1-4 , 4 choices.

void HD44780LCD::PCF8574_LCDChangeEntryMode(LCDEntryMode_e newEntryMode)
{
	PCF8574_LCDSendCmd(newEntryMode);
}

// **** EOF ****
