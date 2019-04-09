/*
 * main.c
 *
 *  Created on: 2010-09-13
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "LCD/lcd44780.h"
#include "IR_DECODE/ir_decode.h"

#define LED_PIN (1<<PC7)			// definicja pinu do którego pod³¹czona jest dioda
#define LED_TOG PORTC ^= LED_PIN	// makrodefinicja – zmiana stanu diody

// klawisz K1
#define KEY_PIN (1<<PC6)

int main(void) {


	DDRA |= (1<<PA7);	// ustawiamy kierunek linii podœwietlenia LCD jako WYJŒCIE
	PORTA |= (1<<PA7);	// za³¹czamy podœwietlenie LCD - stan wysoki

	lcd_init();	/* inicjalizacja LCD */
	ir_init();	/* inicjalizacja dekodowania IR */

	sei();	/* w³¹czamy globalne przerwania */

	lcd_locate(0,0);
	lcd_str_P(PSTR("Kody pilota RC5")); /* wyœwietl napis w zerowej linii LCD */
	/* pêtla nieskoñczona */
	while(1) {

		if(Ir_key_press_flag) {	/* jeœli odebrano prawid³owe kody z pilota */
			if( !address ) {		/* jeœli adres == 0 */
				//lcd_locate(1,0);
				//lcd_str("dd");
				lcd_locate(1,0);
				lcd_str("key: ");
				lcd_int(command);	// wyœwietl w drugiej linii LCD kod klawisza
				lcd_str("   ");

			}

			/* jeœli wciœniêty konkretny przycik vol_up */
			if( !address && vol_up==command ) {
				lcd_locate(1,10);
				lcd_str("vol_up");	/* wyœwietl jego nazwê */

			} else {
				lcd_locate(1,10);
				lcd_str("      ");	/* jeœli inny skasuj pole */
				DDRC |= LED_PIN;		// kierunek pinu PC7 – wyjœciowy
				DDRC &= ~KEY_PIN;		// kierunek piny PC6 - wejœciowy
				PORTC |= KEY_PIN;		// podci¹gamy pin klawisza do VCC
			}

			/* wyzerowanie flagi odbioru oraz wartoœci odebranych kodów */
			Ir_key_press_flag=0;
			command=0xff;
			address=0xff;

		}

	}

}
