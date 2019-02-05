/* Datei:  pwm.c
 * Autor: 
 * letzte �nderung: 28.11.2016
 *
 * Quellprogramm f�r den Treiber f�r die Geschwindig-
 * keitssteuerung der beiden Motoren
 *
 * beide PModHB5 mit Motoren �ber Y-Kabel an JL
 * JLX "gekreuzt (=)"
 */

#include "pwm.h"

uint8_t reverse = 0;

void pwm_init(){       // Treiber initialisieren
  DDRL  =0b00110011;   // EN und DIR als Ausg�nge
  pwm_stop();          // Z�hltakt und PWM aus ...
  TCCR5C = 0b00000000;

  OCR5A = 0x2000;      // Z�hlperiode  (ca. 1 ms)
  OCR5B = 0;           // PWM_R = 0
  OCR5C = 0;           // PWM_L = 0
}

void pwm_set_reverse(uint8_t r) {
	reverse = r;
}

void pwm_start(){      // Antrieb einschalten
  // COM5B, COM5C = 0b10 (\eta = OCR-Wert/Periode)
  // IO-Takt ohne Vorteiler (CS=0b001)
  // Fast-PWM mit OCRA als Periodenreg. (WGM=0b1111)
  TCCR5A = 0b00101011; //

  TCCR5B = 0b00011001; // z�hle IO-Takt ohne Vorteiler
}

void pwm_stop(){      // Antrieb ausschalten
  TCCR5A = 0;         // PWM ausschalten

  TCCR5B = 0;         // Z�hltakt aus

  PORTL  = 0;         // Enable ausschalten

}
                      // Pulsbreiten Antrieb R einstellen
void pwm_set_R(int16_t pwm){
  if(reverse)
	pwm *= -1;
  if (pwm>=0){
    OCR5C = pwm;
    PORTL |=(1<<1);   //DIR-Bit (PL1) setzen
  }
  else{
    OCR5C = -pwm;
    PORTL &=~(1<<1);  //DIR (PL1) l�schen
  }
}

                     // Pulsbreiten Antrieb L einstellen
void pwm_set_L(int16_t pwm){
  if(reverse)
    pwm *= -1;
  if (pwm>=0){
    OCR5B =pwm;
    PORTL |=1;       //DIR-Bit (PL0) setzen
  }
  else{
    OCR5B = -pwm;
    PORTL &= ~1;     //DIR-Bit (PL0) l�schen;
  }
}
