/*
 * i2c-lcd.c
 *
 *  Created on: May 23, 2024
 *      Author: 21635
 */


/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4e // change this according to ur setup

//void lcd_send_cmd (char cmd)
//{
//	uint16_t command_word = 0;
//	command_word = cmd;  // RS=0
//	uint8_t command[2];
//	command[0] = (command_word>>8)&0xFF;  // control byte
//	command[1] = command_word&0xFF;  // data byte
//	HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_LCD, command, 2, 100);
//}
//
//void lcd_send_data (char data)
//{
//	uint16_t data_word = 0;
//	data_word = 0x4000|data;  // RS=1
//	uint8_t data_t[2];
//	data_t[0] = (data_word>>8)&0xFF;
//	data_t[1] = data_word&0xFF;
//	HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_LCD, data_t, 2, 100);
//}
//
//void lcd_clear (void)
//{
//	lcd_send_cmd(0x01);
//	HAL_Delay(5);
//}
//
//void lcd_put_cur(int row, int col)
//{
//    switch (row)
//    {
//        case 0:
//            col |= 0x80;
//            break;
//        case 1:
//            col |= 0xC0;
//            break;
//    }
//
//    lcd_send_cmd (col);
//}
//
//
void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(5);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}
//
//void lcd_send_string (char *str)
//{
//	while (*str) lcd_send_data (*str++);
//}


void lcd_send_cmd (char cmd){
	char data_u, data_l;
	uint8_t i2c_frame_data[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	i2c_frame_data[0] = data_u|0x0C;  //en=1, rs=0
	i2c_frame_data[1] = data_u|0x08;  //en=0, rs=0
	i2c_frame_data[2] = data_l|0x0C;  //en=1, rs=0
	i2c_frame_data[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *) i2c_frame_data, 4, 100);
	HAL_Delay(1);
}

void lcd_send_data (char data){
	char data_u, data_l;
	uint8_t i2c_frame_data[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	i2c_frame_data[0] = data_u|0x0D;  //en=1, rs=0
	i2c_frame_data[1] = data_u|0x09;  //en=0, rs=0
	i2c_frame_data[2] = data_l|0x0D;  //en=1, rs=0
	i2c_frame_data[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *) i2c_frame_data, 4, 100);
	HAL_Delay(1);
}

void lcd_clear (void){
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
}

void lcd_put_cur(int row, int col){
	switch(row){
	case 0:
		col |= 0x80;
		break;
	case 1:
		col |= 0xC0;
		break;
	}
	lcd_send_cmd(col);
}

//void lcd_init (void){
//    // 4-bit mode initialization
//	HAL_Delay(50); // wait for >40ms
//	lcd_send_cmd(0x30);
//	HAL_Delay(5); // wait for > 4.1ms
//	lcd_send_cmd(0x30);
//	HAL_Delay(1); // wait for >100us
//	lcd_send_cmd(0x30);
//	HAL_Delay(10);
//	lcd_send_cmd(0x20); // 4-bit mode
//	HAL_Delay(10);
//    // display initialization
//	lcd_send_cmd(0x28); // Function set --> DL = 0 (4-bit mode), N = 1 (2 line display), F = 0 (5x8 characters)
//	HAL_Delay(1);
//	lcd_send_cmd(0x08); // Display on/off control --> D = 0, C = 0, B = 0 --> display off
//	HAL_Delay(1);
//	lcd_send_cmd(0x01); // clear display
//	HAL_Delay(1);
//	lcd_send_cmd(0x06); // Enter mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
//	HAL_Delay(1);
//	lcd_send_cmd(0x0E); // Display on /off control --> D = 1, C = 1, B = 0
//
//	HAL_Delay(1);
//}

void lcd_send_string (char *str){
    while(*str){lcd_send_data(*str++);}
	HAL_Delay(1);
}
