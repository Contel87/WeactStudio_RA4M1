/*
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

char serialRead(char *buffer) {
  char buf[PRINTF_BUFFER_SIZE] = { NULL_CHAR };
  static unsigned char index = 0;
  while (Serial.available() > 0) {
    char inch = Serial.read();
    if (inch == '\r') {
      for (int i = 0; i < strlen(buffer); i++) {
        buf[i] = buffer[i];
      }
      buffer[index] = NULL_CHAR;  // Aggiunge il terminatore di stringa
      index = RESET_VALUE;        // Reimposta l'indice per il prossimo comando
      return *buf;
    } else {
      if (index < PRINTF_BUFFER_SIZE - 1) {
        buffer[index++] = inch;     // Mette in coda il carattere ricevuto
        buffer[index] = NULL_CHAR;  // Aggiunge il terminatore di stringa
      }
    }
  }
}