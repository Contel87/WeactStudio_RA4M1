/* This program whait a serial command (IMPORTANT! select Carriage Return into serial monitor screen) ...
 * if received "LED_ON" command, switch ON P012 output, if received "LED_OFF" command, switch OFF P012 output.
 *   
 *   02/01/2025 - Luigi Conte (@Contel87)
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

#include "common_utils.h"

void setup() {
    /*WeActStudio RA4M1 board V1.0 has a button between P012 and ground*/
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_00_PIN_12, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
  /*WeActStudio RA4M1 board V1.0 has a Blue led connected to P013*/
  R_IOPORT_PinCfg(NULL, BSP_IO_PORT_00_PIN_13, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);

  Serial.begin(115200);
  delay(2000);
  /*print FSP version info*/
  fsp_pack_version_t version = {RESET_VALUE};
  R_FSP_VersionGet(&version);
  APP_PRINT(BANNER_INFO,EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);  
}

void loop() {
  while(BYTES_RECEIVED_ZERO == APP_CHECK_DATA); // Aspetta un comando dalla seriale (IMPORTANTE! deve essere inviato con il Carriage Return!)
  APP_READ(rByte);
  if(!strcmp(rByte, "LED_ON")) led_level_set(BSP_IO_LEVEL_HIGH); // Se sul serial monitor si scrive "LED_ON" si accende il led sul P012
  if(!strcmp(rByte, "LED_OFF")) led_level_set(BSP_IO_LEVEL_LOW); // Se sul serial monitor si scrive "LED_OFF" si spegne il led sul P012
  APP_PRINT("\rRicevuto il comando: %s", rByte);
}



/*******************************************************************************************************************//**
 * @brief This function sets the level of LED pins
 *
 * @param[IN]   led_level     level to be set
 * @retval      None
 **********************************************************************************************************************/
static void led_level_set(bsp_io_level_t led_level)
{
    fsp_err_t err = FSP_SUCCESS;
    /* Set LED status */
    //err = R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_bsp_leds.p_leds[RESET_VALUE], led_level);
    err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_12, led_level);
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n ** R_IOPORT_PinWrite FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
}
