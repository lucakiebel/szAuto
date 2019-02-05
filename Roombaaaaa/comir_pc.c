/* Datei:  comir_pc.c
 * Autor: 
 * letzte �nderung: 02.11.2015
 *
 * Quellprogramm f�r den Ger�tetreiber comir_pc
 *
 * PmodUSBUSART an JH oben, JHX "gekreuzt (=)"
 */


#include "comir_pc.h"
#include <avr/interrupt.h>

// globale private Daten
uint8_t rmsg[COM_PC_RMSG_LEN];  // Empfangs- und Sendepuffer
uint8_t smsg[COM_PC_SMSG_LEN];

uint8_t sidx, ridx, last_byte;  // Pufferzeiger, letztes Empf-Byte
uint8_t com_pc_err_ct;          // Fehlerz�hler

void com_pc_init(){             // Initialisierung f�r USART2
  UCSR2C=0b110;	                // �bertragungsformat 8N1
  UBRR2=51;                     // 9600 Baud
  UCSR2B=(1<<RXEN2)|(1<<TXEN2); // Sender und Empf�nger ein
  ridx = 0;                     // Empfangspuffer leer
  sidx = COM_PC_SMSG_LEN;       // Sendepuffer leer
  UCSR2B |= (1<<RXCIE2);        // Empfangsinterrupt ein
  last_byte = (uint8_t)' ';

  TCNT3  = 0;                   // Z�hler 3 r�cksetzen
  TCCR3B = 0;                   // Z�hltakt aus
  OCR3A  = 12500;               // Empfangs-Timeout 100 ms
  TIMSK3|= 1<<OCIE3A;           // Vergleichinterrupt A ein
}

// Aufteilung der Schrittfunktion in 2 ISR + 1 f�r Empfangs-Timeout
ISR(USART2_RX_vect){            // ISR Empfang
  last_byte = UDR2;             // Byte lesen
  if (ridx < COM_PC_RMSG_LEN){  // wenn Platz
    rmsg[ridx] = last_byte;     // in Puffer �bernehmen
    ridx++;

    TCNT3  = 0;                 // Z�hler 3 r�cksetzen
    TCCR3B = 0b11;              // Z�hltakt clk/64 ein
  }
}

ISR(TIMER3_COMPA_vect){         // ISR Empfangs-Timeout
  if ((ridx>0) && (ridx <COM_PC_RMSG_LEN)){
    ridx = 0;                   // Puffer l�schen
    com_pc_err_ct++;            // Fehlerz�hler erh�hen
  }
  TCCR3B = 0;                   // Z�hltakt aus
}

ISR(USART2_UDRE_vect){          // Sendepuffer-frei ISR
  if (sidx < COM_PC_SMSG_LEN){  // wenn Sendepuffer frei
    UDR2 = smsg[sidx];          // und Senddaten, versenden
    sidx++;
  }
  else
    UCSR2B &= ~(1<<UDRIE2);     // Puffer-frei-Interrupt aus
}

uint8_t com_pc_get(uint8_t *msg){ // Empfangsnachricht abholen
  uint8_t tmp = UCSR2B;        // Interrupt-Freigabe speichern
  UCSR2B &= ~(1<<RXCIE2);	   // Empfangs-Interrupt aus
  if (ridx < COM_PC_RMSG_LEN){ // wenn Empfangspuffer nicht leer,
    UCSR2B = tmp;              // Int.-Freigabe wiederherstellen
    return 0;                  // R�cksprung mit "falsch"
}
  for (ridx=0; ridx<COM_PC_RMSG_LEN;ridx++)
    msg[ridx] = rmsg[ridx];    // sonst Empfangsnachricht kopieren
  ridx = 0;                    // Empfangspuffer leeren
  UCSR2B = tmp;                // Int.-Freigabe wiederherstellen
  return 1;                    // R�cksprung mit "wahr"
}

uint8_t com_pc_send(uint8_t *msg){ // Sendenachricht �bergeben
  uint8_t tmp = UCSR2B;        // Interrupt-Freigabe speichern
  UCSR2B &= ~(1<<UDRIE2);	     // Puffer-frei-Interrupt aus
  if (sidx < COM_PC_SMSG_LEN){ // wenn der Sendepuffer nicht voll,
    UCSR2B = tmp;              // Interrupt-Freigabe wiederherstellen
    return 0;                  // R�ckgabe "falsch"
	}
  for (sidx=0; sidx<COM_PC_SMSG_LEN;sidx++)
    smsg[sidx] = msg[sidx];    // sonst Sendenachricht �bergeben
                               // und Zeiger auf Nachrichtenanfang
  sidx = 0;                    // Sendepuffer voll
  UCSR2B |= 1<<UDRIE2;         // Interrupt-Freigabe einschalten
  return 1;                    // R�cksprung mit "wahr"
}

uint8_t com_pc_last_byte(){    // R�ckgabe letztes Byte
  return last_byte;
}

uint8_t com_pc_err(){          // Fehlerabfrage
  if (com_pc_err_ct){          // wenn Fehler aufgetreten waren,
		com_pc_err_ct = 0;     //   Fehlerz�hler l�schen
    return 1;                  //   R�ckkehr 1 (wahr)
	}                          // sonst
  return 0;                    //   R�ckkehr mit 0 (falsch)
}
