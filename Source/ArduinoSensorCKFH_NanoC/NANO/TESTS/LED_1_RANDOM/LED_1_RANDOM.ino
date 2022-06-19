/*
 * LED_SEND_MANUAL.ino
 * Manually send message to LED
 */


#define LED_PIN_R   9    /* Pin connected to LED color (red  ) */
#define LED_PIN_G   10   /* Pin connected to LED color (green) */
#define LED_PIN_B   11   /* Pin connected to LED color (blue ) */
#define LED_TIMEOUT 100  /* Timeout value                      */
#define BAUDRATE    9600 /* Baudrate value                     */ 


/*
 * NAME: void setup()
 * DESC: The setup routine runs every time board starts or
 *       once when you press reset
 */
void setup() {
  init_colors();
	/* 
	 * Если порт 0 неподключен, то генератор псевдослучайных чисел будет инициализироваться функцией randomSeed() со случайного
   * значения при каждом запуске программы из-за "шума" на порту
	 */
  randomSeed(analogRead(0));
  Serial.begin(BAUDRATE);
}


void init_colors(){
	pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
	pinMode(LED_PIN_B, OUTPUT);
}


/*
 * NAME: void loop()
 * DESC: Loop will be run for infinite time
 */
void loop() {
  analogWrite( LED_PIN_R, random(0, 100) );
	analogWrite( LED_PIN_G, random(0, 100) );
	analogWrite( LED_PIN_B, random(0, 100) );
	delay(LED_TIMEOUT * 4);

	analogWrite( LED_PIN_R, 0 );
	analogWrite( LED_PIN_G, 0 );
	analogWrite( LED_PIN_B, 0 );
	delay(LED_TIMEOUT * 4);
}

