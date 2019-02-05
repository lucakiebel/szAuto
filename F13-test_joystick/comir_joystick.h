/* Datei:  comir_lcd.c
 * Autor: 
 * letzte �nderung: 02.11.2015
 *
 * Header f�r den Ger�tetreiber comir_lcd
 *
 * PModLS an JD oben
 */


#ifndef COMIR_LCD_H_
#define COMIR_LCD_H_

#include <avr/io.h>


  uint16_t js_get_x();
  uint16_t js_get_y();
  uint8_t  js_get_btn();
  void js_set_led(uint8_t);
  void js_init();

#endif /* COMIR_LCD_H_ */
