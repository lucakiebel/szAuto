/* Datei:  comir_lcd.h
 * Autor: 
 * letzte �nderung: 02.11.2015
 *
 * Quellprogramm f�r den Ger�tetreiber comir_lcd
 *
 * PModCLS an JD (oben)
 */

#define WGM_CTC	  0b0100    // Clear Timer on Compare
#define CS256     0b100     // Vorteiler 256

#include "comir_joystick.h"
#include <avr/interrupt.h>

uint8_t dat[5];
uint32_t ledD;      // Datenz�hler
uint32_t tmr_ct;

uint16_t js_get_x() {
	return (dat[1]<<8)+dat[0];
}

uint16_t js_get_y() {
	return (dat[3]<<8)+dat[2];
}


uint8_t  js_get_btn() {
	return dat[4];
}



void js_set_led(uint8_t led){
	ledD = led;
}

void js_init() {
	DDRB = 0xf7;            // /SS, MOSI, SCK: Ausg�nge
	PORTB |= 0x1;           // /SS=1 (Slave deaktiviert)
     // SPI als Master mit f_SCK=f_CPU/128 einschalten
	SPCR = (1<<SPE)|(1<<MSTR)|(0b11<<SPR0);
	TCCR1A = WGM_CTC & 0b11;  // Betriebsart & Z�hltakt
	TCCR1B = (WGM_CTC & 0b1100)<<1 | (CS256 & 0b111);
	OCR1A  = 3125;            // Vergleichswert f�r 100 ms
	tmr_ct = 0;               // Laufzeitz�hler l�schen
	TIMSK1 |= 1<<OCIE1A;	    // Vergleichs-Interrupt A ein
}

void joystick_get(){
  PORTB &=~1;                     // Slave aktivieren
                                  // 1. Byte senden
  SPDR = 0b10000000|(ledD & 0b11);
  uint8_t i;
  for (i=0;i<5;i++){
    while (!(SPSR & (1<<SPIF)));  // Warte auf SPI-�bertragung
    dat[i] = SPDR;
    if (i<4) SPDR = 0;
  }
  PORTB |= 1;                     // Slave deaktivieren
}
/*
ISR(TIMER1_COMPA_vect){
	joystick_get(dat, ledD);
}*/

/*
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
*/
