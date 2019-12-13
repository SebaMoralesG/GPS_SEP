#include <avr/io.h>
#include <string.h>

#include "GPS_display.h"
#include "graphic_shapes.h"
#include "ST7735_commands.h"

int int_velocidad = 0;
int16_t speed = 0;
int16_t Tspeed = 0;
int16_t speedRef = 115;
uint8_t CredS = 0;
uint8_t CgreenS = 255;
uint8_t CredT = 0;
uint8_t CgreenT = 255;
int direction = 0;
char string_velocidad[5];
char speedchar[5];
int temp;
char temperature[5];
char tempchar[5];
int16_t tempRef = 115;
int tempint;
int tempTrns;
char rumbo[2];

void GPSdisplayinit(void){
	for(uint8_t pixel_y=0; pixel_y<TFT_HEIGHT; pixel_y++)
	{
		for(uint8_t pixel_x=0; pixel_x<TFT_WIDTH; pixel_x++)
		{
			int16_t blue = 255-(pixel_x+pixel_y)/2;
			if (blue < 0)
			blue = 0;
			PORTD &= ~(1 << PORTD2);
			drawPixelRGB(pixel_x, pixel_y, 0, 0, blue);
			PORTD |= (1 << PORTD2);
			
		}
	}
	PORTD &= ~(1 << PORTD2);
	//barras velocidad temperatura
	fillRect(5,30,30,88,ST7735_BLACK);
	drawRect(5,30,30,88,ST7735_CYAN);
	drawString(6,15,ST7735_WHITE,ST7735_WHITE,1,"SPEED");
	drawString(130,15,ST7735_WHITE,ST7735_WHITE,1,"TEMP");
	fillRect(125,30,30,88,ST7735_BLACK);
	drawRect(125,30,30,88,ST7735_CYAN);
	fillRect(38,30,84,50,ST7735_BLACK);
	drawRect(38,30,84,50,ST7735_CYAN);
	//texto comun pantalla
	drawString(80,33,ST7735_WHITE,ST7735_WHITE,2,"kmh");
	//drawString(65,50,ST7735_WHITE,ST7735_WHITE,2,"27");
	drawString(90,50,ST7735_WHITE,ST7735_WHITE,2,"C");
	//Reloj velocidades
	fillCircle(80,100,15,ST7735_BLACK);
	drawCircle(80,100,15,ST7735_RED);
	drawChar(78,87,'N',ST7735_RED,ST7735_RED,1);
	drawChar(78,108,'S',ST7735_RED,ST7735_RED,1);
	drawChar(67,98,'W',ST7735_RED,ST7735_RED,1);
	drawChar(88,98,'E',ST7735_RED,ST7735_RED,1);
	PORTD |= (1 << PORTD2);
}

void SpeedDisplay(void){
	if (speed != int_velocidad)
	{
		PORTD &= ~(1 << PORTD2);
		drawString(70,33,ST7735_BLACK,ST7735_BLACK,2,string_velocidad);
		PORTD |= (1 << PORTD2);
		speed = int_velocidad;
		Tspeed = -speed/3+115;
		PORTD &= ~(1 << PORTD2);
		drawString(70,33,ST7735_WHITE,ST7735_WHITE,2,string_velocidad);
		PORTD |= (1 << PORTD2);
		if (Tspeed > speedRef)
		{
			while (Tspeed != speedRef)
			{
				uint16_t colorS = ((CredS & 0xF8) << 8) | ((CgreenS & 0xFC) << 3) | (0 >> 3);
				PORTD &= ~(1 << PORTD2);
				drawFastHLine(7,speedRef,26,colorS);
				PORTD |= (1 << PORTD2);
				CgreenS -= 3;
				CredS += 3;
				speedRef--;
			}
		}
		else if (Tspeed < speedRef)
		{
			while (Tspeed != speedRef)
			{
				uint16_t colorT = ((CredT & 0xF8) << 8) | ((0 & 0xFC) << 3) | (CgreenT >> 3);
				PORTD &= ~(1 << PORTD2);
				drawFastHLine(7,speedRef,26,ST7735_BLACK);
				drawFastHLine(127,speedRef,26,ST7735_BLACK);
				PORTD |= (1 << PORTD2);
				CgreenS += 3;
				CredS -= 3;
				speedRef++;
			}
		}
	}
}

void TempDisplay(void){
	if (tempint != temp)
	{
		tempint = temp;
		tempTrns = -tempint/2+81;
		PORTD &= ~(1 << PORTD2);
		drawString(70,33,ST7735_WHITE,ST7735_WHITE,2,temperature);
		PORTD |= (1 << PORTD2);
		PORTD &= ~(1 << PORTD2);
		drawString(70,33,ST7735_BLACK,ST7735_BLACK,2,temperature);
		PORTD |= (1 << PORTD2);
		if (tempTrns > tempRef)
		{
			while (tempTrns != tempRef)
			{
				uint16_t colorT = ((CredT & 0xF8) << 8) | ((CgreenT & 0xFC) << 3) | (0 >> 3);
				PORTD &= ~(1 << PORTD2);
				drawFastHLine(127,tempRef,26,colorT);
				PORTD |= (1 << PORTD2);
				CgreenS -= 3;
				CredS += 3;
				tempRef--;
			}
		}
		else if (tempTrns < tempRef)
		{
			while (tempTrns != tempRef)
			{
				uint16_t colorT = ((CredT & 0xF8) << 8) | ((0 & 0xFC) << 3) | (CgreenT >> 3);
				PORTD &= ~(1 << PORTD2);
				drawFastHLine(127,tempRef,26,ST7735_BLACK);
				PORTD |= (1 << PORTD2);
				CgreenS += 3;
				CredS -= 3;
				tempRef++;
			}
		}
	}
}

void CompassDisplay(void){
	if (direction != rumbo)
	{
		if (rumbo == "NX")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(81,99,79,101,75,95,ST7735_BLACK);
			fillTriangle(80,95,82,100,78,100,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "NE")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(80,95,82,100,78,100,ST7735_BLACK);
			fillTriangle(79,99,81,101,85,95,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "XE")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(79,99,81,101,85,95,ST7735_BLACK);
			fillTriangle(80,102,80,98,85,100,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "SE")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(80,102,80,98,85,100,ST7735_BLACK);
			fillTriangle(81,99,79,101,85,105,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "SX")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(81,99,79,101,85,105,ST7735_BLACK);
			fillTriangle(80,105,82,100,78,100,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "SO")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(80,105,82,100,78,100,ST7735_BLACK);
			fillTriangle(79,99,81,101,75,105,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "XO")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(79,99,81,101,75,105,ST7735_BLACK);
			fillTriangle(80,102,80,98,75,100,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		else if (rumbo == "NO")
		{
			PORTD &= ~(1 << PORTD2);
			fillTriangle(80,102,80,98,75,100,ST7735_BLACK);
			fillTriangle(81,99,79,101,75,95,ST7735_WHITE);
			PORTD |= (1 << PORTD2);
		} 
		direction = rumbo;	
	}
}
