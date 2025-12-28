/* Generare un segnale PWM su un pin della WeActStudio RA4M1
 * Se viene premuto il pulsante P013 questo programma genera velocemente un segnale PWM di 10Khz sul pin P304 con la funzione tone(P304, 10000, 0);
 * Il led 012 segnala l'uscita pwm attiva
 *
 *   28/12/2025 - Luigi Conte (@Contel87)
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

#define P012 (pin_size_t)BSP_IO_PORT_00_PIN_12 // Il led della WeAct Studio RA4M1 è montato sul pin 012
#define P013 (pin_size_t)BSP_IO_PORT_00_PIN_13 // Il pulsante WeAct Studio RA4M1 è montato sul pin 013
#define P304 (pin_size_t)BSP_IO_PORT_03_PIN_04 // Un pin che supporta il PWM

#define LED_ON  (R_PORT0->PODR_b.PODR12 = 1)   //Accende il led della WeAct
#define LED_OFF (R_PORT0->PODR_b.PODR12 = 0)   //Spegne il led
#define LED_X   (R_PORT0->PODR_b.PODR12 = !(R_PORT0->PODR_b.PODR12)) // Toggle led

#define BUTTON_STATUS (R_PORT0->PIDR_b.PIDR13) // Controlla se il pulsante 013 sulla WeAct è premuto



void setup() {
  pinPeripheral(P013, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);// Il bootloader della MINIMA R4 non configura il P013, perciò lo facciamo noi
  pinPeripheral(P012, IOPORT_CFG_PORT_DIRECTION_OUTPUT);                          // Configura il led sulla P012 copme output
  }

void loop() {

  if(BUTTON_STATUS){
         noTone(P304); // Switch off PWM signal on P304
         LED_OFF;
  }else{
        tone(P304, 10000, 0); //10Kh PWM output sul pin 304 della WeAct
         LED_ON;
  }
  delay(100);
}
