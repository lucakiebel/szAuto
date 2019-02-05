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

  void lcd_init(uint8_t *text); // LCD-Objekt initialisieren
  void lcd_incErr(uint8_t pos); // Fehlerz�hler erh�hen
                                // Zeichen auf LCD ausgeben
  void lcd_disp_chr(uint8_t c, uint8_t pos);
                                // Text auf LCD ausgeben
  void lcd_disp_str(uint8_t *str, uint8_t pos, uint8_t len);
                                // Zahl auf LCD ausgeben
  void lcd_disp_val(uint32_t val, uint8_t pos, uint8_t len);

#endif /* COMIR_LCD_H_ */
