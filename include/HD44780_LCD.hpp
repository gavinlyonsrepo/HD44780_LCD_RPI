/*!
	@file     HD44780_LCD.hpp
	@author   Gavin Lyons
	@brief    HD44780-based character LCD I2C(PCF8574)library header file for RPI
	
	@note 
	-# Compiler: C++ g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
	-# Tested: Raspbian 10, armv7l Linux 5.10.63-v7+ , RPI M3B Rev 1.2
	-# URL: https://github.com/gavinlyonsrepo/HD44780_LCD_RPI
*/

#include <bcm2835.h>
#include <iostream> // for cout error messages
#include "HD44780_LCD_Print.hpp"

#pragma once


// Section: Class's
class HD44780PCF8574LCD : public Print{ 
  public:
	// public enums

	/*! Backlight Control ,  Command Byte Code */
	enum LCDBackLight_e : uint8_t{
		LCDBackLightOnMask = 0x0F, /**< XXXX-1111 , Turn on Back light */
		LCDBackLightOffMask = 0x07 /**< XXXX-0111, Turn off Back light */
	}; 

	/*! Entry mode control  set command,  Command Byte Code */
	enum LCDEntryMode_e : uint8_t{
		LCDEntryModeOne = 0x04, /**< Display Shift :OFF Decrement Address Counter */
		LCDEntryModeTwo = 0x05, /**< Display Shift :ON  Decrement Address Counter */
		LCDEntryModeThree = 0x06, /**< Display Shift :OFF Increment Address Counter, default */
		LCDEntryModeFour = 0x07 /**<  Display Shift :ON Increment Address Counter */
	}; 

	/*! Cursor mode, Command Byte Code */
	enum LCDCursorType_e : uint8_t {
		LCDCursorTypeOff= 0x0C, /**< Make cursor invisible */
		LCDCursorTypeBlink = 0x0D, /**< Turn on blinking-block cursor */
		LCDCursorTypeOn = 0x0E,  /**< Turn on visible  underline cursor */
		LCDCursorTypeOnBlink  = 0x0F /**<Turn on blinking-block cursor + visible underline cursor */
	}; 

	/*! Direction mode for scroll and move */
	enum LCDDirectionType_e : uint8_t {
		LCDMoveRight= 1, /**< move or scroll right */
		LCDMoveLeft = 2   /**< move or scroll left */
	}; 

	/*! Line Row Number */
	enum LCDLineNumber_e : uint8_t{
		LCDLineNumberOne = 1,   /**< row 1 */
		LCDLineNumberTwo = 2,   /**< row 2 */
		LCDLineNumberThree = 3, /**< row 3 */
		LCDLineNumberFour = 4  /**<  row 4 */
	}; 
	
	
	HD44780PCF8574LCD(uint8_t NumRow, uint8_t NumCol, uint8_t I2Caddress, uint16_t I2Cspeed);
	~HD44780PCF8574LCD(){};
	
	void LCDInit(LCDCursorType_e);
	void LCDDisplayON(bool);
	void LCDResetScreen(LCDCursorType_e);
	
	void LCDBackLightSet(bool);
	bool LCDBackLightGet(void);
	
	void LCDDebugSet(bool);
	bool LCDDebugGet(void);
	
	int16_t LCDVerNumGet(void);
	
	bool LCD_I2C_ON(void);
	void LCD_I2C_SetSpeed(void);
	void LCD_I2C_OFF(void);
	int16_t LCDCheckConnection(void);
	bool LCDI2CErrorGet(void);
	uint16_t LCDI2CErrorTimeoutGet(void);
	void LCDI2CErrorTimeoutSet(uint16_t);
	
	void LCDSendString (char *str);
	void LCDSendChar (char data);
	virtual size_t write(uint8_t);
	void LCDCreateCustomChar(uint8_t location, uint8_t* charmap);
	void LCDPrintCustomChar(uint8_t location);
	
	void LCDMoveCursor(LCDDirectionType_e, uint8_t moveSize);
	void LCDScroll(LCDDirectionType_e, uint8_t ScrollSize);
	void LCDGOTO(LCDLineNumber_e  lineNo, uint8_t  col);
	void LCDClearLine (LCDLineNumber_e lineNo);
	void LCDClearScreen(void);
	void LCDClearScreenCmd(void);
	void LCDHome(void);
	void LCDChangeEntryMode(LCDEntryMode_e mode);

  private:
	void LCDSendCmd (unsigned char cmd);
	void LCDSendData (unsigned char data);
	
	// Private Enums
	/*!  DDRAM address's used to set cursor position  Note Private */
	enum LCDAddress_e : uint8_t {
		LCDLineAddressOne =  0x80,  /**< Line 1 */
		LCDLineAddressTwo =  0xC0, /**< Line 2 */
		LCDLineAddress3Col20 = 0x94, /**< Line 3 20x04 line 3 */
		LCDLineAddress4Col20 = 0xD4, /**< Line 4 20x04 line 4 */
		LCDLineAddress3Col16  = 0x90, /**< Line 3 16x04  untested, no part */
		LCDLineAddress4Col16  = 0xD0 /**< Line 4 16x04 untested, no part */
	}; 

	/*!  Command Bytes General  Note Private */
	enum LCDCmdBytesGeneral_e : uint8_t {
		LCDCmdModeFourBit = 0x28, /**< Function set (4-bit interface, 2 lines, 5*7 Pixels) */
		LCDCmdHomePosition  = 0x02, /**< Home (move cursor to top/left character position) */
		LCDCmdDisplayOn = 0x0C,  /**< Restore the display (with cursor hidden) */
		LCDCmdDisplayOff = 0x08, /**< Blank the display (without clearing) */
		LCDCmdClearScreen = 0x01 /**< clear screen command byte*/
	};
	
	enum  LCDBackLight_e _LCDBackLight= LCDBackLightOnMask;  /**< Enum to store backlight status*/
	
	
	const int16_t _LibVersionNum = 132; /**< library version number */
	bool _DebugON = false;  /**< debug flag , if true error messages will be printed to console */
	
	const uint8_t LCD_I2C_ADDRESS = 0x27;  /**< Default I2C address for I2C module PCF8574 backpack on LCD */
	uint8_t _LCDSlaveAddresI2C = LCD_I2C_ADDRESS ; /**< I2C address for I2C module PCF8574 backpack on LCD*/
	uint16_t _LCDSpeedI2C = 0x00; /**< I2C speed default 0(100K) or BCM2835_I2C_CLOCK_DIVIDER enum values */ 
	uint16_t _I2C_ErrorDelay = 100; /**<I2C delay in event of error in mS*/
	uint16_t _I2C_Error = 0; /**< In event of I2C error holds bcm2835 I2C reason code */
	
	uint8_t _NumRowsLCD = 2; /**< number of rows on LCD*/
	uint8_t _NumColsLCD = 16; /**< number of columns on LCD*/

		
  }; // end of HD44780PCF8574LCD class

