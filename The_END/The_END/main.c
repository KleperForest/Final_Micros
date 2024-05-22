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

#include "PWM1/PWM1.h"
#include "ADC/ADC.h"
#include "PWM2/PWM2.h"


int main(void)
{
  int caso = 0, activa = 0,  activa2 = 0, estado = 0, activa3 = 0, samtf=0, activa4 = 0, activa5 = 0, moverr = 0;      //Variables a utilizar para el procesamiento
  int garra = 0, garra1 = 0, brazo = 0, brazo1 = 0, codo = 0, codo1 = 0, rota = 0, rota1 = 0, sumar = 0;
  volatile char receivedChar = 0, mover = 0;
  int anterior1 = 0;
 }

void setup(void){
	cli();  //Apagar interrupciones
	
	DDRB |= (1 << PORTB2) | (1 << PORTB1) | (1 << PORTB3); //PB1, PB2, PB3 como salida de servos
	DDRC =0;  //Puerto C como entrada
	DDRD |= (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6);   //PD3 como salida de servo, PD4 - PD6 salida de LEDS
	DDRB &= 0b00001110;   //Activar entrada de pulsador 1 de estado, 2 de guardar/confirmar.
	PORTB = (1 << PORTB4) | (1 << PORTB0);  //Activar PULLUP
	DDRD = 0b01111000;
	
	initFastPWM1(8);  //Iniciar funcion de FASTPWM en timer1
	channel(channelB, modo);
	channel(channelA, modo);
	topValue(29999);   //Valor alto de desbordamiento para 1 a 2 ms que se necesita de servo
	initADC();   //Iniciar la funcion de ADC
	initFastPWM2();  //Iniciar funcion de FASTPWM en timer2
	initUART9600();
	PORTD |=  (1 << DDD4);
	
	sei();   //Activar interrupciones
}