#ifndef __DHT21_H__
#define __DHT21_H__

#include <TroykaDHT.h> /* TroykaDHT library      */

#define DHT21_USE
/* Sensor types: DHT11, DHT21, DHT22             */
#define DHT21_PIN 11       /* Connected data pin */
#define DHT21_SENSOR DHT21 /* Sensor type used   */
/* DHT21 data structure                          */
struct s_DHT21_data {
  int   state;          /* Device status         */
  float temperature_c;  /* C units temperature   */
  float temperature_k;  /* K units temperature   */
  float temperature_f;  /* F units temperature   */
  float humidity;       /* Humidity (%)          */
};

/* DHT obect class with pin (DHT_PIN) and sensor (DHT_SENSOR) */
DHT Sensor_DHT21(DHT21_PIN, DHT21_SENSOR);

#endif  /* #ifndef __DHT21_H__*/