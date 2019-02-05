/* Datei:  comir_lcd.h
 * Autor: 
 * letzte �nderung: 02.11.2015
 *
 * Quellprogramm f�r den Ger�tetreiber comir_lcd
 *
 * PModCLS an JD (oben)
 */


#include "comir_lcd.h"
#include <avr/interrupt.h>

// globale private Daten
uint8_t LCD_dat[32];  // Ausgabetext
uint8_t lcd_idx;      // Indexvariable

void lcd_init(uint8_t *text){ // LCD-Treiber initialisieren
  UCSR1C = 0b110;             // �bertragungsformat 8N1
  UBRR1 = 51;                 // 9600 Baud
  UCSR1B = (1<<TXEN1);	      // Sender ein

  // 8 Zeichen LC-Display-Initialisierung
  uint8_t lcd_init_dat[] = "\x1B[0h\x1B[j";
  for (lcd_idx=0; lcd_idx<7; lcd_idx++){
    while (!(UCSR1A&(1<<UDRE1)));
    UDR1 = lcd_init_dat[lcd_idx];
  }
  // Initialisierung des Hintergrundtextes
  for (lcd_idx=0; lcd_idx<32; lcd_idx++) // �bernahme des
    LCD_dat[lcd_idx] = text[lcd_idx];    // Init.-Texts
  lcd_idx = 0;                   // Index auf Feldanfang

  UCSR1B |= (1<<UDRIE1);         // Puffer-frei-Interrupt ein
}

ISR(USART1_UDRE_vect){	         // Puffer-frei ISR
  UDR1 = LCD_dat[lcd_idx];       // schicke n�chstes
  lcd_idx++;                     // Zeichen
  // nach dem letzten folgt das erste Zeichen
  if (lcd_idx>=32) lcd_idx = 0;
}

// S�ttigungsz�hler auf LCD erh�hen
// Reihenfolge ->1->2->..->9->a->b->c->..->z
void lcd_incErr(uint8_t pos){
  pos &= 0x1F;                // WB auf 0:31 begrenzen
  uint8_t w = LCD_dat[pos];
  if (w>='0' && w<'9') w++;
  else if (w=='9') w = 'a';
  else if (w>='a' && w<'z') w++;
  else if (w=='z');
  else w = '1';
  LCD_dat[pos] = w;
}

// Zeichen auf LCD ausgeben
void lcd_disp_chr(uint8_t c, uint8_t pos){
  if (pos<0x20){                // bei zul�ssiger Position
    LCD_dat[pos & 0x1F] = c;    // Zeichen schreiben
  }
}

// Text auf LCD ausgeben
void lcd_disp_str(uint8_t *str, uint8_t pos, uint8_t len){
  while (len){                 // f�r alle Zeichen
    lcd_disp_chr(*str, pos);   // Zeichen schreiben
    str++; pos++; len--;       //
  }
}


// Zahl auf LCD ausgeben
void lcd_disp_val(uint32_t val, uint8_t pos, uint8_t len){
  while (len){
    len--;                    // Ziffernanzahl verringern
    lcd_disp_chr((val % 10) + '0', pos+len); // Rest mod 10
    val = val / 10;           // Ziffer und Wert durch 10
  }
  if (val)                    // wenn Stellenzahl zu klein,
  lcd_disp_chr('?', pos);     // Ersatz 1. Ziffer durch '?'
}

// ISR zur Behandlung ungewollter Interrupts
ISR(BADISR_vect){            // Fehlerz�hler (letztes
  lcd_incErr(31);            // Zeichen) hochz�hlen
}
