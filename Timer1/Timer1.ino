/* Esempio su come usare il Timer1 sulla WeAct RA4M1 ...
 * Sulla board è caricato il bootloader della R4 MINIMA
 * Il pin P012 è impostato come OUTPUT in modo da poterlo utilizzare anche con bootloader della NANO R4 (la NANO R4 non ha collegato niente sul P012)
 * Per modificare le temporizzazioni agire sul valore della PCLKB (in questo esempio vale 23999) e sulla variabile TIMER_SOURCE_DIV_X
 *   03/01/2026 - Luigi Conte (@Contel87)
 *     
 *  This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FspTimer.h"

#define P012    (BSP_IO_PORT_00_PIN_12) // Il led della WeAct Studio RA4M1 è montato sul pin 012
#define LED_ON  (R_PORT0->PODR_b.PODR12 = 1)   //Accende il led della WeAct
#define LED_OFF (R_PORT0->PODR_b.PODR12 = 0)   //Spegne il led
#define LED_X   (R_PORT0->PODR_b.PODR12 = !(R_PORT0->PODR_b.PODR12)) // Toggle led

FspTimer fsp_timer;
uint32_t cnt;

void AGTCallback(timer_callback_args_t __attribute((unused)) * p_args);

void setup() {
  //pinPeripheral(P012, IOPORT_CFG_PORT_DIRECTION_OUTPUT); // Configura il led sulla P012 copme output
   R_IOPORT_PinCfg(nullptr, P012, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

 //Setting interrupt handling for each AGT 1ms If PCLKB=24MHz TIMER_SOURCE_DIV_1 (1 minute), then 1/(24e6*1) * 24000 = 1ms
 fsp_timer.begin(TIMER_MODE_PERIODIC, AGT_TIMER, 1, 23999, 1, (timer_source_div_t)TIMER_SOURCE_DIV_1, AGTCallback);

IRQManager::getInstance().addPeripheral(IRQ_AGT, (void*)fsp_timer.get_cfg());

fsp_timer.open();
//fsp_timer.start(); //calling start() is not necessary: open() starts the counter already
}

void loop() {
  // Il timer1 incrementa la variabile cnt di 1 ogni millisecondo
  if(cnt >= 1000) {cnt = 0;} // Ogni secondo la variabile si azzera
  (cnt < 500) ? LED_ON : LED_OFF; // Ogni 500mSec cambia lo stato del led
}

void AGTCallback(timer_callback_args_t __attribute((unused)) * p_args) {
 cnt++;
}