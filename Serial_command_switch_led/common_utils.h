#ifndef COMMON_UTILS_H_
#define COMMON_UTILS_H_

/* generic headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

#define MODULE_NAME             "r_ioport"
#define EP_VERSION              ("1.0")
#define RESET_VALUE             (0x00)
#define NULL_CHAR               '\0'

/* MACRO for checking if no byte is received */
#define BYTES_RECEIVED_ZERO  (0U)


#define APP_PRINT(fn_, ...)      do{\                                
                                 char buffer[500]; \
                                 sprintf(buffer,(fn_), ##__VA_ARGS__);\
                                 Serial.println(buffer);\
                                } while(false);                              

#define APP_ERR_PRINT(fn_, ...)  do{\  
                                char buffer[100]; \
        sprintf (buffer,"[ERR] In Function: %s(), %s",__FUNCTION__,(fn_),##__VA_ARGS__);\
        Serial.println(buffer);\
        } while(false);                                  

#define APP_ERR_TRAP(err)     do{\  
         char buffer[10]; \
        sprintf(buffer, "\r\nReturned Error Code: 0x%x  \r\n", err);\
        Serial.println(buffer);\
        }while(false); /* trap upon the error  */

#define PRINTF_BUFFER_SIZE     (32u)    // Size of buffer for Serial input to bulk-send chars via Serial
#define APP_READ(read_data)    serialRead(read_data)
#define APP_CHECK_DATA         (Serial.available() > 0)                    

#define BANNER_INFO             "\r\n******************************************************************"\
                                "\r\n*   Renesas FSP Example Project for "MODULE_NAME" Module                 *"\
                                "\r\n*   Example Project Version %s                                  *"\
                                "\r\n*   Flex Software Pack Version  %d.%d.%d                            *"\
                                "\r\n******************************************************************"\
                                "\r\nRefer to readme.txt file for more details on Example Project and" \
                                "\r\nFSP User's Manual for more information about "MODULE_NAME" driver\r\n"


static char rByte[PRINTF_BUFFER_SIZE] = {RESET_VALUE};
char serialRead(char *buffer);

#endif /* COMMON_UTILS_H_ */