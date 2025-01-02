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