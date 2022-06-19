/*
 * BLINK.ino
 * Turns on an LED on for one second, then off for one second, repeatedly.
 * This example code is in the public domain.
 */


#define LED_SIGNAL  13  /* Pin 13 has a LED connected on most Arduino boards */
#define LED_TIMEOUT 200 /* Timeout value for blink                           */


/*
 * NAME: void setup()
 * DESC: The setup routine runs every time board starts or
 *       once when you press reset
 */
void setup() {
  pinMode(LED_SIGNAL, OUTPUT); /* Initialize the digital pin as an output */
}


/*
 * NAME: void loop()
 * DESC: Loop will be run for infinite time
 */
void loop() {
  led_timeout(LED_SIGNAL, HIGH); /* Turn the LED on (HIGH is the voltage level) */
  led_timeout(LED_SIGNAL, LOW);  /* Turn the LED off by making the voltage LOW  */
}


/*
 * NAME: led_timeout loop()
 * DESC: Send signal with a specified values and wait a second
 */
void led_timeout(int signal, int value) {
  digitalWrite(signal, value); 
  delay(LED_TIMEOUT);    
}