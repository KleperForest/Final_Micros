//*********************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Alan Gomez
// Proyecto: Final_Project.c
// Descripción: Proyecto Final de Programación de Microcontroladores con lenguaje C.
// Hardware: ATmega328p
// Created: 4/29/2024 7:37:15 PM
//*********************************************************************

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>

#include "EEPROM/EEPROM.h"
#include "PWM1/PWM1.h"
#include "ADC/ADC.h"
#include "PWM2/PWM2.h"
#include "UART/UART.h"

//Pulsador en PC4 cambio de modo
//Pulsador en D8, almacena en EEPROM cuando esta en el modo EEPROM, D8 ES PULLUP
//Pulsador en PC5, devuelve las posiciones almacenadas en EEPROM, cuando esta en el modo EEPROM
//Servos conectados a D3, D9, D10, D11

void setup(void);

uint8_t servo1 = 0, servo2 = 0, servo3 = 0, servo4 = 0, ADCS = 0, modos = 0, CEEPROM = 0, DESADC = 0, mostrarEEPROM;  
volatile char receivedChar = 0;


void setup(void){
	
	
	DDRB |= (1 << PORTB2) | (1 << PORTB1) | (1 << PORTB3); // salida de servos
	DDRC =0;  //Puerto C como entrada
	DDRD |= (1 << DDD3 | (1 << DDD4) | (1 << DDD5) | (1 << DDD6));   //LEDS conectados a D4, D5, D6
	DDRB &= 0b00001110;   //Activar entrada de pulsador en D8
	PORTB = (1 << PORTB4) | (1 << PORTB0);  //Activar PULLUP 
	
	initFastPWM1(8);  // timer1
	channel(channelB, modo);
	channel(channelA, modo);
	topValue(29999);   // 1 a 2 ms servo
	initADC();   // ADC
	initFastPWM2();  // timer2
	PORTD |=  (1 << DDD5);
	
    sei();   //Activar interrupciones
}


int main(void)
{
	setup();
	while (1)
	{ 
		
		if(PINC & (1 << PINC4))
		{
			_delay_ms(10);
			modos ++;
			while (PINC & (1 << PINC4))   
			{
				_delay_ms(30);    //antirrebote
			}
		}
			switch(modos){
				
				case 1:    //Modo manual
					
					DESADC = 0;
					break;
					
				case 2:    //Modo Adafruit
					PORTD |=  (1 << DDD4);
					
					break;
					
				case 3:    //Modo EEPROM
					PORTD |=  (1 << DDD6);
					
					if ((PINB & (1 << PINB0)) == 0)    //Si se presiona el pulsador D8
					{
						_delay_ms(20);
						CEEPROM ++;
						while ((PINB & (1 << PINB0)) == 0)
						{
							_delay_ms(30);    //antirrebote
						}
					}
					
					if(PINC & (1 << PINC5)){
						mostrarEEPROM++;
						_delay_ms(10);
						DESADC = 1;  //desactivar ADC
						
						while (PINC & (1 << PINC5))
						{
							_delay_ms(30);    //antirrebote
						}
					}
						
						//Verificar que valor tiene la posicion actual y almacenar los valores
						if (CEEPROM == 1)
						{
							EEPROM_write(0,servo1);          //Almacenar cada valor de cada servomotor
							EEPROM_write(1,servo2);
							EEPROM_write(2,servo3);
							EEPROM_write(3,servo4);
						}
						
						else if (CEEPROM == 2)
						{
							EEPROM_write(4,servo1);
							EEPROM_write(5,servo2);
							EEPROM_write(6,servo3);
							EEPROM_write(7,servo4);
							
						}
						
						else if (CEEPROM == 3)
						{
							EEPROM_write(8,servo1);
							EEPROM_write(9,servo2);
							EEPROM_write(10,servo3);
							EEPROM_write(11,servo4);
							
						}
						
					else if (CEEPROM == 4)
					{
						CEEPROM = 0;
						EEPROM_write(12,servo1);
						EEPROM_write(13,servo2);
						EEPROM_write(14,servo3);
						EEPROM_write(15,servo4);
						
					}
					
					
					
					if (mostrarEEPROM == 1)
					{
					
						int  dato = EEPROM_read(0);
						convert2(dato, channel2A);  
						
						dato = EEPROM_read(1);  
						convert1(dato, channelA);
						
						dato = EEPROM_read(2);  
						convert1(dato, channelB);
						
						dato = EEPROM_read(3);  
						convert2(dato, channel2B);
					
					}
					
					if (mostrarEEPROM == 2)
					{
						
						int  dato = EEPROM_read(4);
						convert2(dato, channel2A);
						
						dato = EEPROM_read(5);
						convert1(dato, channelA);
						
						dato = EEPROM_read(6);
						convert1(dato, channelB);
						
						dato = EEPROM_read(7);
						convert2(dato, channel2B);
						
					}
					
					if (mostrarEEPROM == 3)
					{
						
						int  dato = EEPROM_read(8);
						convert2(dato, channel2A);
						
						dato = EEPROM_read(9);
						convert1(dato, channelA);
						
						dato = EEPROM_read(10);
						convert1(dato, channelB);
						
						dato = EEPROM_read(11);
						convert2(dato, channel2B);
						
					}
					
					if (mostrarEEPROM == 4)
					{
						
						int  dato = EEPROM_read(12);
						convert2(dato, channel2A);
						
						dato = EEPROM_read(13);
						convert1(dato, channelA);
						
						dato = EEPROM_read(14);
						convert1(dato, channelB);
						
						dato = EEPROM_read(15);
						convert2(dato, channel2B);
					}
					
					
					if (mostrarEEPROM == 5)
					{
						mostrarEEPROM = 0;
					}
					
					

					break;
			}
			
			if (modos == 4)
			{
				PORTD &=(1 << DDD5) ;
				modos = 0;
			}
			
		
		
	if (DESADC == 0)
	{
	
	ADCSRA |= (1 << ADSC);   //Leer ADC
	}
	_delay_ms(20);
		
	}
}



ISR (ADC_vect){
	if (DESADC == 0)
	{
		switch (ADCS){
			case 0:
				ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));   //Borrar configuracion actual y poner ADC0
				ADCS = 1;
				convert1(ADCH, channelB);   //enviar se?al a canal B de timer1
				servo1 = ADCH;
				break;
				
			case 1:
				ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));   //Borrar configuracion actual y poner ADC1
				ADMUX |= (1<<MUX0);
				ADCS = 2; 
				convert1(ADCH, channelA);       //enviar se?al a canal A de timer1
				servo2 = ADCH;
				break;
			
			case 2:
				ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
				ADMUX|=(1<<MUX1);
				ADCS = 3; 
				convert2(ADCH, channel2A);
				servo3 = ADCH;
				break;
			
			case 3:
				ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
				ADMUX|=(1<<MUX0)|(1<<MUX1);
				ADCS = 0;
				convert2(ADCH, channel2B);
				servo4 = ADCH;
				break;
		
		}
		
	ADCSRA |= (1 << ADIF);	
}
}


ISR(USART_RX_vect)
{
	receivedChar = UDR0;

	while(!(UCSR0A & (1<<UDRE0)));    //Mientras haya caracteres
	
}
