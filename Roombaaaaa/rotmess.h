/* Datei:  wegmess.h
 * Autor: 
 * letzte �nderung: 28.11.2016
 *
 * Header f�r die Drehschrittmessung an beiden Motoren
 *
 * beide PModHB5 mit Motoren �ber Y-Kabel an JL
 * JLX "gekreuzt (=)"
 */

#ifndef ROTMESS_H_
#define ROTMESS_H_

  #include <avr/io.h>
  #define ABTASTSCHRITTE 2000 // 2000*0,5 ms = Messzeit 1s

  void rotmess_init();   // Initialisierung des Wegmesstreibers
  // Lesen der Anzahl der Incrementschritte in der als Konstante
  // definierten Anzahl von Abtastschritten
  uint8_t rotmess_get(int16_t *spR, int16_t *spL);
  // R�ckgabe "1" bei Fehler und Fehlerz�hler l�schen
  uint8_t rotmess_err();

#endif /* ROTMESS_H_ */
