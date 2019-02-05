/* Datei:  comir_tmr.c
 * Autor: 
 * letzte �nderung: 27.09.2016
 *
 * Quellprogramm f�r den Ger�tetreiber comir_tmr
 */

#include "comir_tmr.h"
#include <avr/interrupt.h>

#define WGM_CTC	  0b0100    // Clear Timer on Compare
#define CS256     0b100     // Vorteiler 256

uint32_t tmr_ct;            // Aufw�rtsz�hler Programmzeit
uint16_t tmr_array[4];      // Abw�rtsz�hler Wartezeit

// Timer 1 f�r OCRA-Interrupts alle 10 ms initialisieren
void tmr_init(){
  TCCR1A = WGM_CTC & 0b11;  // Betriebsart & Z�hltakt
  TCCR1B = (WGM_CTC & 0b1100)<<1 | (CS256 & 0b111);
  OCR1A  = 3125;            // Vergleichswert f�r 100 ms
  tmr_ct = 0;               // Laufzeitz�hler l�schen
  TIMSK1 |= 1<<OCIE1A;	    // Vergleichs-Interrupt A ein
}


ISR(TIMER1_COMPA_vect){     // Vergleichs-Interrupt
  uint8_t idx;              // alle 100 ms
  tmr_ct++;                 // Programmzeit z�hlen
  for (idx=0; idx<4; idx++) // f�r alle 4 Wartez�hler
    if (tmr_array[idx])     // wenn ungleich null
      tmr_array[idx]--;     // abw�rts z�hlen
}

void tmr_start(uint16_t tw, uint8_t nr){
  uint8_t  tmp = TIMSK1;    // Int.-Zustand sichern
  TIMSK1 &= ~(1<<OCIE1A);	// Vergleichs-Interrupt A aus
  tmr_array[nr & 0b11] = tw;// Wartezeit schreiben
  TIMSK1 = tmp;             // Int.-Zustand wiederherst.
}

uint16_t tmr_restzeit(uint8_t nr){ // Restzeit lesen
  uint8_t  tmp = TIMSK1;    // Int.-Zustand sichern
  TIMSK1 &= ~(1<<OCIE1A);	// Vergleichs-Interrupt A aus
  uint16_t z = tmr_array[nr & 0b11]; // Restzeit lesen
  TIMSK1 = tmp;             // Int.-Zustand wiederherst.
	return z;
}

uint32_t tmr_get(){         // Zeitzaehler lesen
  uint8_t  tmp = TIMSK1;    // Int.-Zustand sichern
  TIMSK1 &= ~(1<<OCIE1A);	// Vergleichs-Interrupt A aus
  uint32_t z = tmr_ct;      // Uhrzeit lesen
  TIMSK1 = tmp;             // Int.-Zustand wiederherst.
	return z;
}
