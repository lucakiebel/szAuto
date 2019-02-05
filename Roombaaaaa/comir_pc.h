/* Datei:  comir_pc.h
 * Autor: 
 * letzte �nderung: 02.11.2015
 *
 * Header f�r den Ger�tetreiber comir_pc
 *
 * PmodUSBUSART an JH oben, JHX "gekreuzt (=)"
 */


#ifndef COMIR_PC_H_
#define COMIR_PC_H_

  #include <avr/io.h>

  // Gr��e des Empfangs- und Sendepuffers
  #define COM_PC_RMSG_LEN  6
  #define COM_PC_SMSG_LEN  6

  void com_pc_init();        // Initialisierung
                             // Nachricht �bernehmen
  uint8_t com_pc_get(uint8_t *msg);
                             // Nachricht �bergeben
  uint8_t com_pc_send(uint8_t *msg);

  uint8_t com_pc_last_byte();// R�ckgabe letztes Byte
  uint8_t com_pc_err();      // Fehlerabfrage

#endif /* COMIR_PC_H_ */
