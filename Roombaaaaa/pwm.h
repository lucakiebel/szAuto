/* Datei:  pwm.h
 * Autor: 
 * letzte �nderung: 02.11.2015
 *
 * Header f�r den Treiber f�r die Geschwindig-
 * keitssteuerung der beiden Motoren
 *
 * beide PModHB5 mit Motoren �ber Y-Kabel an JL
 * JLX "gekreuzt (=)"
 */

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
  void pwm_init();              // Treiber initialisieren
  void pwm_start();             // Antrieb einschalten
  void pwm_stop();              // Antrieb ausschalten
  void pwm_set_R(int16_t pwm);  // Pulsbreiten Antrieb R einstellen
  void pwm_set_L(int16_t pwm);  // Pulsbreiten Antrieb L einstellen
  void pwm_set_reverse(uint8_t r);

#endif /* PWM_H_ */
