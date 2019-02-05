/* Datei:  comir_tmr.h
 * Autor: 
 * letzte �nderung: 27.09.2016
 *
 * Header f�r den Ger�tetreiber comir_tmr
 */

#ifndef COMIR_TMR_H_
#define COMIR_TMR_H_

  #include <avr/io.h>
  void tmr_init();        // Treiber initialisieren
	                      // Wartezeit einstellen
  void tmr_start(uint16_t tw, uint8_t nr);
	                      // Restzeit lesen
  uint16_t tmr_restzeit(uint8_t nr);
  uint32_t tmr_get();     // Zeitz�hler lesen

#endif /* COMIR_TMR_H_ */
