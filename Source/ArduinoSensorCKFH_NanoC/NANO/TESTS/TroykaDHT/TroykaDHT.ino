#include <TroykaDHT.h> /* DHT library */

#define BAUDRATE     9600 
#define TIMEOUT_2SEC 2000
#define DHT_PIN      11
#define DHT_SENSOR   DHT21
/* 
 * Создаём объект класса DHT
 * Передаём номер пина к которому подключён датчик и тип датчика
 * Типы сенсоров: DHT11, DHT21, DHT22
 */
DHT Sensor_DHT(DHT_PIN, DHT_SENSOR);


/*
 * NAME: void setup()
 * DESC: The setup routine runs every time board starts or
 *       once when you press reset
 */
void setup() {
  Serial.begin(BAUDRATE); /* Initialise serial         */
  Sensor_DHT.begin();     /* DHT native begin function */
}


/*
 * NAME: void loop()
 * DESC: Loop will be run for infinite time
 */
void loop() {
  Sensor_DHT.read();                /* Read DHT sensor data */
  switch( Sensor_DHT.getState() ) { /* Check state          */
    case DHT_OK:
      /* Signal OK */
      // выводим показания влажности и температуры
      Serial.print("Temperature = ");
      Serial.print(Sensor_DHT.getTemperatureC());
      Serial.println(" C \t");
      Serial.print("Temperature = ");
      Serial.print(Sensor_DHT.getTemperatureK());
      Serial.println(" K \t");
      Serial.print("Temperature = ");
      Serial.print(Sensor_DHT.getTemperatureF());
      Serial.println(" F \t");
      Serial.print("Humidity = ");
      Serial.print(Sensor_DHT.getHumidity());
      Serial.println(" %");
      break;
    case DHT_ERROR_CHECKSUM:
      // CSUM error
      Serial.println("Checksum error");
      break;
    case DHT_ERROR_TIMEOUT:
      // Timeout
      Serial.println("Time out error");
      break;
    // данных нет, датчик не реагирует или отсутствует
    case DHT_ERROR_NO_REPLY:
      Serial.println("Sensor not connected");
      break;
  }

  delay(TIMEOUT_2SEC); /* 2 seconds timeout */
  print_emulate_clear();
}

void print_emulate_clear() {
  for( int i = 0; i < 7; i++ ) { Serial.println(""); }
}