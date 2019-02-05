/* Datei:  test_joistick.c
 * Autor: 
 * letzte �nderung:  03.11.2015
 *
 * Anzeige der Joystick-Werte x, y, Button und
 * des Z�hlers der Endlosschleife auf dem LCD
 *
 * Joystick (PModJSTK) an JB und LCD (PmodLCS) an JD oben
 */


#include <avr/io.h>
#include "comir_pc.h"
#include "comir_lcd.h"
#include "comir_joystick.h"
#include "comir_tmr.h"
#include <avr/interrupt.h>

// Initialisierung SPI-Bus f�r die Joystick-Kommunikation
// Stecker  Belegung    Bedeutung
// JM-1	    /SS  PB0    Slave-Auswahl       (Ausgang)
// JM-2     MOSI PB2    Master-Out-Slave-In (Ausgang)
// JM-3     MISO PB3    Master-In-Slave-Out (Eingang)
// JM-4     SCK  PB1    Schiebetakt         (Ausgang)
/*
void joystick_init(){
  DDRB = 0xf7;            // /SS, MOSI, SCK: Ausg�nge
  PORTB |= 0x1;           // /SS=1 (Slave deaktiviert)
     // SPI als Master mit f_SCK=f_CPU/128 einschalten
  SPCR = (1<<SPE)|(1<<MSTR)|(0b11<<SPR0);
}

// Datenaustausch mit dem Joystick
// ptr:    Zeiger auf einen 5Byte gro�en Puffer
// LedDat: einzuschaltende Joystick-Leds, Wert 0 bis 3
void joystick_get(uint8_t *buffer, uint8_t LedDat){
  PORTB &=~1;                     // Slave aktivieren
                                  // 1. Byte senden
	SPDR = 0b10000000|(LedDat & 0b11);
	uint8_t i;
	for (i=0;i<5;i++){
    while (!(SPSR & (1<<SPIF)));  // Warte auf SPI-�bertragung
    buffer[i] = SPDR;
    if (i<4) SPDR = 0;
  }
  PORTB |= 1;                     // Slave deaktivieren
}*/

// Konstantendefinitionen f�r die LCD-Darstellung
#define INITSTR "                                "
#define LCP_x     2 // x-Koordinate
#define LCP_y    18 // y-Koordinate
#define LCP_Btn1 14 // Button 1 gedr�ckt
#define LCP_Btn2 13 // Button 2 gedr�ckt
#define LCP_Btn3 12 // Button 3 gedr�ckt
#define LCP_Ct   26 // Z�hler �bertragungen

uint8_t msg[COM_PC_RMSG_LEN];
uint8_t smsg[COM_PC_SMSG_LEN];

void send(uint8_t dat) {
	msg[0] = dat;
	com_pc_send(msg);
}

int main(void){
  com_pc_init();
  tmr_init();
  js_init();
  //joystick_init();	// SPI initialisieren
  lcd_init((uint8_t*)INITSTR);
  uint32_t Ct;      // Datenz�hler

  //uint8_t dat[5];   // Daten
  sei();
  while(1) {

	  /*lcd_disp_val(js_get_x(),LCP_x, 4);
	  lcd_disp_val(js_get_y(),LCP_y, 4);

	  if (js_get_btn() & 0b100) lcd_disp_chr('E', LCP_Btn3);
	  else lcd_disp_chr('A', LCP_Btn3);
	  if (js_get_btn() & 0b010) lcd_disp_chr('E', LCP_Btn2);
	  else lcd_disp_chr('A', LCP_Btn2);
	  if (js_get_btn() & 0b001) lcd_disp_chr('E', LCP_Btn1);
      else lcd_disp_chr('A', LCP_Btn1);

	  js_set_led(0b11);*/

	  if (!tmr_restzeit(1)){ // wenn Timer abgelaufen,
		  msg[0] = js_get_btn();
		  msg[1] = 0;

		  msg[2] = 0;
		  msg[3] = 0;

		  msg[4] = 0x00;
		  msg[5] = 0x00;

		  msg[6] = 0x00;
		  msg[7] = 0x00;
		  msg[8] = 0x00;
		  msg[9] = 0x00;
		  msg[10] = 0x00;
		  msg[11] = 0x00;

		  com_pc_send(msg);
		  tmr_start(0x08, 1);
	  }


	  if (!tmr_restzeit(0)){ // wenn Timer abgelaufen,

		  int16_t left = ((js_get_y() - 500) * 20) + ((js_get_x() - 500) * 20);
		  int16_t right = ((js_get_y() - 500) * -20) + ((js_get_x() - 500) * 20);

		  msg[0] = left >> 8;
		  msg[1] = left & 0x00FF;

		  msg[2] = right >> 8;
		  msg[3] = right & 0x00FF;

		  msg[4] = 0x00;
		  msg[5] = 0x0F;
		  /*
		  msg[6] = 0x00;
		  msg[7] = 0x00;
		  msg[8] = 0x00;
		  msg[9] = 0x00;
		  msg[10] = 0x00;
		  msg[11] = 0x00;*/

		  com_pc_send(msg);


		  tmr_start(0x01, 0);
	  }

	  if(com_pc_get(smsg)) {
		  switch (smsg[5]) {
			  case 0x01:
			    lcd_disp_chr(smsg[0], smsg[1]);
				break;
			  case 0x02:
			    js_set_led(smsg[0]);
				break;

		  }
	  }



    /*joystick_get(dat, (Ct/200) & 0b11);
	  lcd_disp_val((dat[1]<<8)+dat[0],LCP_x, 4);
	  lcd_disp_val((dat[3]<<8)+dat[2],LCP_y, 4);
		if (dat[4] & 0b100) lcd_disp_chr('E', LCP_Btn3);
		else lcd_disp_chr('A', LCP_Btn3);
		if (dat[4] & 0b010) lcd_disp_chr('E', LCP_Btn2);
		else lcd_disp_chr('A', LCP_Btn2);
		if (dat[4] & 0b001) lcd_disp_chr('E', LCP_Btn1);
		else lcd_disp_chr('A', LCP_Btn1);
	  lcd_disp_val(Ct++,LCP_Ct,6);*/
  }



}
