/* Datei:  test_pwm.c
 * Autor: 
 * letzte �nderung:  02.11.2015
 *
 * Testprogramm f�r die pwm-Ansteuerung der Motoren
 *
 * Die Motoren sind �ber zwei H-Br�cken PModHB5 und
 * Splitter-Kabel an JL und PmodUSBUSART an JH oben
 * anzuschliessen. Motor A  ist oben.
 * PmodUSBUSART an JH oben anschliessen.
 * JHX und JLX "gekreuzt (=)"
 */

#include "comir_pc.h"
#include "comir_tmr.h"
#include "pwm.h"
#include "rotmess.h"
#include <avr/interrupt.h>

uint8_t msg[COM_PC_RMSG_LEN];
uint8_t smsg[COM_PC_SMSG_LEN];
uint8_t active;

int16_t replayR, replayL;
uint8_t replay = 0;

int main(void){
	DDRJ=0xFF;
	int16_t pwm;
	uint16_t time;
	int16_t speed_R, speed_L;
	rotmess_init();
	com_pc_init();          // Treiber comir_pc und
	pwm_init();             // pwm initialisieren
	tmr_init();
	sei();				  // Interrupts global ein
	//com_pc_send(0xDD);
	active = 0;
	PORTJ=0x00;

	while(1){
		if (com_pc_get(msg)){ // wenn neue Nachricht
			if(msg[5] == 0x0F) {
				pwm = msg[0]<<8 | msg[1];
				pwm_set_R(pwm);     // PWM-Wert f�r Motor R
				pwm = msg[2]<<8 | msg[3];
				pwm_set_L(pwm);     // PWM-Wert f�r Motor L

				time = msg[4]<<8 | msg[5];
				tmr_start(0xFF, 0); // Timer Kanal 0 starten
				pwm_start();        // PWM (Motoren) starten
				active = 1;
				replay = 0;
			} else if(msg[5] == 0x05) {
				replay = 1;
				replayR = msg[0]<<8 | msg[1];
				replayL = msg[2]<<8 | msg[3];
			} else if(msg[5] == 0x02) {
				pwm_set_reverse(msg[0]);
			} else if(msg[5] == 0x04) {
				PORTJ = 0xFF;
				pwm_set_R(8000);
				pwm_set_L(8000);
				tmr_start(0xFF, 0);
				replayL = 12800;
				replayR = 12800;
				replay = 1;
				active = 1;
				pwm_start();
			}

		}
		//PORTJ = tmr_restzeit(0);
		if (!tmr_restzeit(0) && active){ // wenn Timer abgelaufen,
			pwm_stop();           // PWM (Motoren) aus

			active = 0;
			//com_pc_send(msg);
		}


		if(!tmr_restzeit(2)) {
			smsg[0] = 0;//;speed_R>>8;
			smsg[1] = 0;//;speed_R & 0xff;

			smsg[2] = 0;//;speed_L>>8;
			smsg[3] = 0;//;speed_L & 0xff;

			smsg[4] = 0;
			smsg[5] = 0x05;
			if(rotmess_get(&speed_R, &speed_L)) {
				//PORTJ = speed_R;
				if(replay) {

					replayR -= speed_R;
					replayL -= speed_L;
					if(replayR <= 0) {
						pwm_set_R(0);
						pwm_start();
					}
					if(replayR <= 0) {
						pwm_set_L(0);
						pwm_start();
					}
				}
				smsg[0] = speed_R>>8;
				smsg[1] = speed_R & 0xff;

				smsg[2] = speed_L>>8;
				smsg[3] = speed_L & 0xff;

				smsg[4] = 0;
				smsg[5] = 0x05;
			}
			com_pc_send(smsg);
			tmr_start(0x01, 2);
		}

		/*if(rotmess_get(&speed_R, &speed_L)) {
			com_pc_send(smsg);

			smsg[0] = speed_R>>8;
			smsg[1] = speed_R & 0xff;

			smsg[2] = speed_L>>8;
			smsg[3] = speed_L & 0xff;

			smsg[4] = 0;
			smsg[5] = 0x05;
		}*/
	}
}
