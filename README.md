# MGIMO-ArduinoSensorProject
MGIMO Study - ArduinoSensorProject

# Project Arduino(C++), Android(Java) and Python
Уведомление человека об изменении температуры в помещении по сравнению с нормой (в архивах, хранилищах овощей и т.п.)
## Структура
- code_zip - архивы с кодом (распаковать любым архиватором)
- docs - документация
## Инструкция 
### Arduino(C++)
#### Распиновка Arduino Nano
![Alt text](http://wiki.amperka.ru/_media/%D0%BF%D1%80%D0%BE%D0%B4%D1%83%D0%BA%D1%82%D1%8B:arduino-nano:arduino-nano_pinout.png)

1. Подключать датчики необходимо к пинам 2-7, 11. В конфигурации датчика (отдельный .h модуль, например DHT21.h из проекта) необходимо указать число соответствующее номеру пина. Конфигурируется define'ами.
2. В проекте для пинов 9, 10 и 12 используется LED индикация. При необходимости убрать define'ы в config.h.
3. Подключать SERIAL трансмитеры (Bluetooth/WiFi/прямая передача) необходимо к пинам RX и TX.
4. Аналогичные действия необходимо сделать, если плата отличается - выставить корректные пины в define'ах и подключить датчики/трансмитеры в нужные пины.
#### Загрузка ПО в Arduino Nano
1. Установить [Arduino IDE](https://www.arduino.cc/en/Main/Software).
2. Распаковать /code_zip/ArduinoSensorCKFH_Stock.zip.
3. Запустить Arduino IDE и окрыть /NANO/MAIN/MainProject.ino (Файл -> Открыть -> выбрать в директории /NANO/MAIN).
4. Выбрать в Arduino IDE тип платы Nano и подключить плату Arduino Nano с помощью USB порта.
   Примечание: От плата Arduino Nano должны быть отключены все утройства кроме USB.
5. Настроить порт и выбрать его в Arduino IDE.
6. В меню Arduino IDE выбрать Скетч -> Загрузка.
### Android(Java)
1. Установить [Android Studio](https://developer.android.com/studio?hl=ru).
2. Установить JDK Suite. 
3. Распаковать /code_zip/ArduinoSensorCKFH_Android.zip.
4. Запустить Android Studio.
5. Открыть ранее распакованную директорию (возможно, понадобится донастройка конфигурации под Application).
6. Подключить Android смартфон по USB, включить на смартфоне режим разработчика.
7. Запустить проект (Run).
8. При необходимости собрать проект в .pak.
### Python
1. Установить [PyCharm Community](https://www.jetbrains.com/pycharm/) и интерпретатор Python 3.0 и выше.
2. Распаковать /code_zip/ArduinoSensorCKFH_Python.zip.
3. Запустить PyCharm Community.
4. В меню PyCharm Community выбрать Файл -> Открыть и открыть ранее распакованную директорию
5. Запустить проект Run -> Run 'AppMain'
