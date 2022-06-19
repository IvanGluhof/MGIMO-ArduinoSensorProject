/*
 * SERIAL.ino
 * Serial port availability test
 */


/*
 * NAME: void setup()
 * DESC: The setup routine runs every time board starts or
 *       once when you press reset
 */
void setup() {
  Serial.begin(9600); /* Initialize serial */
}


/*
 * NAME: void loop()
 * DESC: Loop will be run for infinite time
 */
void loop() {
  if( Serial.available() > 0 ) {
    int incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  } else {
    Serial.print("I received nothing\n");
  }
  delay(1000);
}
