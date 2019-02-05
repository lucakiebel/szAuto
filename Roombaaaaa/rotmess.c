/* Datei: rotmess.c
 * Autor: 
 * letzte �nderung: 28.09.2016
 *
 * Quellprogramm f�r den Treiber f�r die
 * Drehschrittmessung an beiden Motoren
 *
 * beide PModHB5 mit Motoren �ber Y-Kabel an JL
 * JLX "gekreuzt (=)"
 */

#include <avr/interrupt.h>
#include "rotmess.h"

int16_t Ct_T;				// Zeitz�hler
int16_t Ct_R, Ct_L;			// Geschwindigkeitsz�hler
int16_t speed_R, speed_L;	// Geschwindigkeitswerte
int8_t sens_R, sens_L;		// Bit(3:2) neue und B(1:0)
							// alte Sensorwerte
uint8_t rotmess_err_ct;		// Fehlerz�hler, nur Debug
uint8_t new_dat;		// 0 keine neuen Daten, 1 neue Daten


void clear_counter(){
	Ct_R = 0; Ct_L  = 0; // Winkelschritt- und
	Ct_T = 0;            // Zeitz�hler l�schen
}


void rotmess_init(){
  // Timer 0 f�r OCR0A-Interrupts alle 0.5 ms einrichten
  TCCR2A  = 0b10;		// WGM = 0b010 (CTC Mode mit OCR0A)
  TCCR2B  = 0b011;	    // CS  = 0b011 (Vorteiler 64)
  OCR2A   = 62;         // OCR = (0,5 ms*8MHz)/64-1
  TIMSK2 |= 1<<OCIE2A;	// Vergleichs-Interrupt freigeben
  sens_R = (PINL>>4) & 0b1100;	// Startwerte der Hall-
  sens_L =  PINL     & 0b1100;	// sensoren lesen
  clear_counter();				// Z�hler l�schen
}

// Drehschrittbestimmung
int8_t QuadEnc(uint8_t sensdat){
  switch (sensdat){
    case 0b0010:
    case 0b0100:
    case 0b1011:
    case 0b1101:
    return -1;
    case 0b0001:
    case 0b0111:
    case 0b1000:
    case 0b1110:
    return 1;
    case 0b0011:
    case 0b0110:
    case 0b1100:
    case 0b1001:
    if (rotmess_err_ct<0xFF)
      rotmess_err_ct++;
  }
  return 0;
}

ISR(TIMER2_COMPA_vect){
	speed_R += ((PINL>>4) & 0b1100);
	speed_L += (sens_L>>2) |  (PINL & 0b1100);
	/*sens_R = (sens_R>>2) | ((PINL>>4) & 0b1100);
  sens_L = (sens_L>>2) |  (PINL & 0b1100);
  Ct_R += QuadEnc(sens_R);
  Ct_L += QuadEnc(sens_L);
  Ct_T++;
  if (Ct_T>=ABTASTSCHRITTE){
    speed_R = Ct_R;
    speed_L = Ct_L;
    if (new_dat) rotmess_err_ct++;
    new_dat=1;
    clear_counter();
  }*/
}

uint8_t rotmess_get(int16_t *spR, int16_t *spL){
  //if (new_dat){             // wenn neue Daten
    uint8_t tmp = TIMSK0;   // ISR, die dieselben Daten
    TIMSK0 &= ~(1<<OCIE0A); // bearbeitet sperren
    *spR = speed_R;         // Ergebnisse kopieren
    *spL = speed_L;
	speed_R = 0;
	speed_L = 0;
    TIMSK0	= tmp;      // Interrupts wieder freigeben
    new_dat = 0;            // neue-Daten-Flag l�schen
    return 1;               // R�ckkehr mit "neue Daten"
  //}                         // sonst
  //return 0;                 // R�ckkehr ohne neue Daten
}

uint8_t rotmess_err(){      // Fehlerabfrage
	if (rotmess_err_ct){    // wenn Fehler aufgetreten sind
		rotmess_err_ct=0;   // Fehlerz�hler l�schen
		return 1;           // R�ckkehr 1 (wahr)
	}                       // sonst
	return 0;               // R�ckkehr mit 0 (falsch)
}
