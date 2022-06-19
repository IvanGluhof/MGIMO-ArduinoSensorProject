#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef enum { T_FALSE, T_TRUE } T_BOOL;   /* Boolean typedef */
typedef enum { OFF    , ON     } T_STATUS; /* On/off typedef */ 
/* REGION: DEFINES                                                   */
#define DEBUG OFF
#if( DEBUG == ON )
  #define VAL_CLEARSCREEN 6
  #define CLEARSCREEN() for( int i = 0; i < VAL_CLEARSCREEN; i++ ) { Serial.println(""); }
#endif
/* Baud rates configuration (use standart 9600 or 115200)            */
#define BAUDRATE_9600   9600          /* 9600   bps                  */
#define BAUDRATE_115200 115200        /* 115200 bps                  */
#define BAUDRATE        BAUDRATE_9600 /* Defines current baudrate    */
/* Timeout configuration                                             */
#define TIMEOUT_1S    1000            /* 1 second                    */
#define TIMEOUT_2S    TIMEOUT_1S*2    /* 2 seconds                   */ 
#define TIMEOUT_3S    TIMEOUT_1S*3    /* 3 seconds                   */ 
#define TIMEOUT_4S    TIMEOUT_1S*4    /* 4 seconds                   */ 
#define TIMEOUT_NS(n) TIMEOUT_1S*n    /* n seconds (custom)          */
/* LED configuration                                                 */
#define LED_PIN_R   9          /* Pin connected to LED color (red  ) */
#define LED_PIN_G   10         /* Pin connected to LED color (green) */
#define LED_PIN_B   12         /* Pin connected to LED color (blue ) */
#define LED_TIMEOUT TIMEOUT_1S /* Timeout value                      */
/* LED defines to control LED color                                  */
#define LED_COLOR_R 0          /* RED                                */
#define LED_COLOR_G 1          /* GREEN                              */
#define LED_COLOR_B 2          /* BLUE                               */
/* Standart C Functions redefines  */
/* Standart C strstr (word parser) */
#define MEMCPY(dst, src) memcpy(dst, src, strlen(src) )
/* Standert memcpy with strlen - 1 (used for eliminating EOL) */
#define MEMCPY_STRLEN_1(dst, src) memcpy(dst, src, (strlen(src) - 1) )
/* ENDREGION: DEFINES                                                */

#endif  /* #ifndef __CONFIG_H__ */