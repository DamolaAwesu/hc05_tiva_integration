/*------------------------------------LCD Module-------------------------------------------------------
Version: 1.0
Author(s): Damola Awesu; Vinay Garade
File Description: Header File containing function definitions for LCD control used in files: main.c, lcd.c

------------------------------------------------------------------------------------------------------*/

#include "TM4C123.h"                    // Device header
#include "bsp.h"
#include <string.h>

void LCD_Init(void) {
	/*Initialise PB5 - PB7 as LCD command pins RS, RW, E; Initialise PE0 - PE7 as LCD Data Pins D0 - D7*/
	SYSCTL->RCGCGPIO |= GPIO_CFG;
	GPIOA->DIR |= CMD_CFG;						
	GPIOA->DEN |= CMD_CFG;
	GPIOB->DIR = DATA_CFG;
	GPIOB->DEN = DATA_CFG;
	/*Configure LCD for Startup*/
	LCD_8BitCommand(MODE_8BIT);
	LCD_8BitCommand(CURS_HOME);
	LCD_8BitCommand(CURSOR_SHIFT);
	LCD_8BitCommand(LCD_ON);
	LCD_8BitCommand(CLR_SCR);
}

void LCD_4BitTransfer(unsigned char data, unsigned char reg_select) {
	
	data &= 0xF0;
	reg_select &= 0x0F;
	GPIOB->DATA |= (reg_select << 5);
	GPIOE->DATA |= data;
	GPIOB->DATA |= (reg_select << 5)|(1 << 7);
	delay_us(0);
	GPIOB->DATA &= ~(1 << 7);
	
}

void LCD_8BitCommand(unsigned char command) {
	/*
		This function takes an 8-bit variable(command) and uses it to configure the LCD's operating mode. RS is set to 0 to enable command mode
		and the command is written to the data pins.
		Refer to the LCD Control section in bsp.h to see possible command values
	*/
	GPIOA->DATA |= CMD_SELECT;
	GPIOB->DATA = command;
	GPIOA->DATA |= EN_PIN_HIGH;
	delay_us(0);
	if(command < 0x4) {
		delay_us(2000);
	}
	else {
		delay_us(37);
	}
}

void LCD_4BitCommand(unsigned char command) {
	/*
		This function takes an 8-bit variable(command) and uses it to configure the LCD's operating mode. RS is set to 0 to enable command mode
		and the command is written to the data pins.
		Refer to the LCD Control section in bsp.h to see possible command values
	*/
	GPIOE->DATA |= (command & 0xF0);
	GPIOE->DATA = command;
	GPIOB->DATA |= EN_PIN_HIGH;
	delay_us(0);
	if(command < 0x4) {
		delay_us(2000);
	}
	else {
		delay_us(37);
	}
}

void LCD_8BitData(unsigned char data) {
	/*
		This function takes an 8-bit variable(data) and prints it on the LCD.
		RS is set to 0 to enable data mode and the data is written to the data pins.
	*/
	GPIOA->DATA |= DATA_SELECT;
	GPIOB->DATA = data;
	GPIOA->DATA |= EN_PIN_HIGH;
	delay_us(0);
	GPIOA->DATA &= ~LCD_RESET;
	delay_us(0);
}

void LCD_4BitData(unsigned char data) {
	/*
		This function takes an 8-bit variable(data) and prints it on the LCD.
		RS is set to 0 to enable data mode and the data is written to the data pins.
	*/
	GPIOB->DATA |= DATA_SELECT;
	GPIOE->DATA = data;
	GPIOB->DATA |= EN_PIN_HIGH;
	delay_us(0);
	GPIOB->DATA &= ~LCD_RESET;
	delay_us(0);
}

void displayString(char *string) {
	/*
		This function takes a string of character and prints it on the LCD.
		Prints the string by iteratively passing individual characters to the LCD_Data() function
	*/
	while(*string) {
		if(strcmp(string, " ")==0) {
			LCD_8BitCommand(SPACE_BAR);
		}
		else {
			LCD_8BitData(*string++);
		}
	}
}

void setCursor(uint8_t line, uint8_t column) {
	/*
		This function accepts two parameters(line,column) to adjust the cursor position on the LCD.
		Achieves this by passing predefined command instructions to the LCD command register based on the value of the supplied parameters
	*/
	if(line == 1)  {
		LCD_8BitCommand(LINE_1);
	}
	else {
		LCD_8BitCommand(LINE_2);
	}
	for(int i = 0; i < column; i++) {
		LCD_8BitCommand(SPACE_BAR);
	}
}
