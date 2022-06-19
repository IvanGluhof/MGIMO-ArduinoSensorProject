/*
 * LED_SEND_MANUAL.ino
 * Manually send message to LED
 */


#define LED_PIN     9   /* Pin connected to LED color      */
#define LED_TIMEOUT 100 /* Timeout value for blink         */
String message;         /* String object used for messages */


/*
 * NAME: void setup()
 * DESC: The setup routine runs every time board starts or
 *       once when you press reset
 */
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}


/*
 * NAME: void loop()
 * DESC: Loop will be run for infinite time
 */
void loop() {
  /* Check serial availability */
  if( Serial.available() > 0 ) {
    /* 
     * Передаваемые с компьютера данные поставляются байт за байтом, в виде отдельных символов (англ. character). 
     * Нам нужно последовательно их обрабатывать когда в порту доступны (англ. available) новые данные, то 
     * считываем (англ. read) пришедший символ в переменную.
     */
    char incomingChar = Serial.read();
    /* 
     * Не стоит путать целые числа и символы. Они соотносятся друг с другом по таблице, называемой кодировкой. 
     * Например '0' — это 48, '9' — 57, 'A' — 65, 'B' — 66 и т.п.
     * Символы в программе записываются в одинарных кавычках.
     */
    if( incomingChar >= '0' && incomingChar <= '9' ) {
      message += incomingChar; /* Если пришёл символ-цифра, добавляем его к сообщению */
    } else if( incomingChar == '\n' ) {
      /* 
       * Если пришёл символ новой строки, т.е. enter, переводим накопленное сообщение в целое число (англ. to integer).
       * Так последовательность символов '1', '2', '3' станет числом 123. Результат выводим на светодиод
       */
      analogWrite(LED_PIN, message.toInt());
      message = "";                    /* Обнуляем накопленное сообщение, чтобы начать всё заново */
      Serial.print("Message sent!\n"); /* For debug */
    }
  } else {
    Serial.print("Serial is empty, waiting...\n");
    delay(LED_TIMEOUT);
  }
  /* Sent message number throught Serial Monitor */
}

