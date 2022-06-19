/* REGION: Device configuration */
/* 
 * This region must contain #includes,
 * depending on the devices use, like this:
 * #include <device_lib.h>
 */
#include <DHT21.h>  /* DHT21 Temperature and humidity sensor */

#include <config.h> /* Setting and configuration header */

/* Serial prints should be replaced by log function with device number */
#define INIT_DEVICE(DEVICE_NAME)                \            
  InitResult &= DEVICE_NAME##_Init();           \ 
  InitResult == T_TRUE ? Serial.println("OK") :   \
                         Serial.println("Error"); \
  Sensors |= InitResult;                        \
/* ENDREGION: Device configuration */


/* Scan values data structure */
struct s_ScanValues {
  char  init;
#ifdef DHT21_USE
  s_DHT21_data DHT21_data;
#endif /* #ifdef DHT21_USE */
};


/***********************************************************
 * NAME: void setup()                                      *
 * DESC: The setup routine runs every time board starts or *
 *       once when you press reset                         *
 * TODO: 1) Send error signal                              *
 *       2) Add logging about start up                     *
 ***********************************************************/
void setup() {
  Serial.begin(BAUDRATE); /* Initialise serial     */
  /* Initialise onboard devices and initialise LED */
  ( init_devices() && init_led_rgb() ) ? Serial.println("All onboard devices are started") :
                                         Serial.println("Some onboard device have failed to start");
  delay(100);
} /* END void setup() */


/***********************************************************
 * NAME: void loop()
 * DESC: Loop will be run for infinite time
 ***********************************************************/
void loop() {
  /* Init scan data structure */
  s_ScanValues 
    ScanValues = { .init = 0 };

#ifdef DHT21_USE
  /* Get vaules from sensor */
  s_DHT21_data 
    DHT21_data = DHT21_ScanValues(DHT21_data);
  /* Copy data */
  memcpy(&ScanValues.DHT21_data, &DHT21_data, sizeof(struct s_DHT21_data));
#endif /* #ifdef DHT21_USE */

  TransmitData(ScanValues);

} /* END void loop() */


/*****************************************************************
 * NAME: s_ScanValues DHT21_TransmitData(s_ScanValues data_out)  *
 * DESC: Copy data to main structure and transmit                *
 * TODO: Add feedback signal                                     *
 *****************************************************************/
void TransmitData(s_ScanValues ScanValues) {
  /* This sends data over serial port e.g. bluetooth */
#ifdef DHT21_USE
  if( ScanValues.DHT21_data.state == DHT_OK ) {
    Serial.print("C") && Serial.print(ScanValues.DHT21_data.temperature_c) && Serial.print(":") ;
    Serial.print("K") && Serial.print(ScanValues.DHT21_data.temperature_k) && Serial.print(":") ;
    Serial.print("F") && Serial.print(ScanValues.DHT21_data.temperature_f) && Serial.print(":") ;
    Serial.print("H") && Serial.print(ScanValues.DHT21_data.humidity     ) && Serial.print(";;");
#if( DEBUG == ON )
    CLEARSCREEN();
#endif
  } else {
    Serial.print("DHT21 ERR ");
    Serial.println(ScanValues.DHT21_data.state);
  }
#endif /* #ifdef DHT21_USE */
}

/***********************************************************
 * NAME: T_BOOL init_devices()                             *
 * DESC: Initialise onboard devieces/sensors using defines *
 ***********************************************************/
T_BOOL init_devices() {
  T_BOOL 
    InitResult = T_TRUE,  /* Boolean result to check onboard devices */
    Sensors    = T_FALSE; /* Boolean result to check sensors         */

#ifdef DHT21_USE
  INIT_DEVICE(DHT21); /* DHT21 Initialise */ 
#endif /* #ifdef DHT21_USE */

  return( InitResult & Sensors );
} /* END T_BOOL init_devices() */


/***********************************************************
 * NAME: T_BOOL init_led_rgb()                             *
 * DESC: Custom onboard LED with pin sets                  *
 ***********************************************************/
T_BOOL init_led_rgb() {
	pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
	pinMode(LED_PIN_B, OUTPUT);

  return T_TRUE;
} /* T_BOOL init_led_rgb() */


/***********************************************************
 * NAME: void set_led_color()                              *
 * DESC: Set color using three diode LED                   *
 * TODO: Add custom color                                  *
 ***********************************************************/
void set_led_color(int color_value) {
  switch( color_value ) {
    case LED_COLOR_R: { 
      analogWrite( LED_PIN_R, HIGH );
      analogWrite( LED_PIN_G, 0    );  
      analogWrite( LED_PIN_B, 0    ); 
      break; 
    }
    case LED_COLOR_G: { 
      analogWrite( LED_PIN_R, 0    );
      analogWrite( LED_PIN_G, HIGH );  
      analogWrite( LED_PIN_B, 0    ); 
      break; 
      }
    case LED_COLOR_B: { 
      analogWrite( LED_PIN_R, 0    );
      analogWrite( LED_PIN_G, 0    );  
      analogWrite( LED_PIN_B, HIGH ); 
      break; 
    }
  }
} /* void set_led_color(int color_value) */


#ifdef DHT21_USE
/***********************************************************
 * NAME: T_BOOL DHT21_Init()                               *
 * DESC: Inits DH21 sensor using it's state                *
 ***********************************************************/
T_BOOL DHT21_Init() {
  T_BOOL 
    InitResult = T_FALSE;

  Sensor_DHT21.begin(); /* DHT native begin function */
  Sensor_DHT21.read();  /* Read DHT sensor data      */
  Sensor_DHT21.getState() == DHT_OK ? InitResult = T_TRUE : 
                                      InitResult = DHT21_ErrHandler(Sensor_DHT21.getState());

  return InitResult;
} /* END T_BOOL DHT21_Init() */


/***********************************************************
 * NAME: T_BOOL DHT21_ErrHandler()                         *
 * DESC: Handles error using DH21 state                    *
 * TODO: Use serial.print to send errors                   *
 ***********************************************************/
T_BOOL DHT21_ErrHandler(int state) {
  switch( state ) {
    case DHT_ERROR_CHECKSUM: { Serial.println("Checksum error"); break; } /* CSUM error                                */
    case DHT_ERROR_TIMEOUT:  { Serial.println("Time out error"); break; } /* Timeout error                             */
    case DHT_ERROR_NO_REPLY: { Serial.println("No connection" ); break; } /* No data, no reaction or sensor is missing */
  }

  return T_FALSE;
} /* END T_BOOL DHT21_ErrHandler(int state) */


/***********************************************************
 * NAME: s_DHT21_data DHT21_ScanValues(s_DHT21_data data)  *
 * DESC: Scan and fetch data from DHT21 sensor             *
 ***********************************************************/
s_DHT21_data DHT21_ScanValues(s_DHT21_data data) {
  /* Read DHT sensor data */
  Sensor_DHT21.read();                
  data.state = Sensor_DHT21.getState();

  if( data.state == DHT_OK ) {
    set_led_color(LED_COLOR_G);
    data.temperature_c = Sensor_DHT21.getTemperatureC();
    data.temperature_k = Sensor_DHT21.getTemperatureK();
    data.temperature_f = Sensor_DHT21.getTemperatureF();
    data.humidity      = Sensor_DHT21.getHumidity();
  } else {
    set_led_color(LED_COLOR_R);
    DHT21_ErrHandler(data.state);
  }

  delay(TIMEOUT_2S); /* 2 seconds timeout */

  return data;
} /* s_DHT21_data DHT21_ScanValues(s_DHT21_data data) */
#endif /* #ifdef DHT21_USE */