Arduino Heizungssteuerung
==========================


Über das Projekt
----------------
Dieses Projekt wird als steuerung für einen Honigauftauschrank der [Werkstatt Imkerei](https://werkstatt-imkerei.de/) an der [Gesamtschule Höhscheid](https://gesamtschule-höhscheid.de/) entwickelt. Es lässt sich aber problemlos auch für andere Dinge verwenden.

Material
-------
Folgende Dinge werden benötigt:
*   Kabel
*   Arduino (UNO, Nano usw.)
*   LCD 16x2 Display mit I2C Adapter
*   DHT22 Temperatur Sensor (falls nicht auf einer platine wird ein 10KOhm Wiederstand benötigt)
*   Heizung die durch 5V ein und ausgeschaltet werden kann (Relais)

Installation
------------
In den folgenden Schritten wird erklärt wie sie dieses Projekt verwenden können.

1.  Installiere die Arduino IDE von [Arduino.cc](https://www.arduino.cc/en/Main/Software) und lade [dieses Projekt](https://gitlab.com/johuck/arduino-heat-controll-lcd/-/archive/master/arduino-heat-controll-lcd-master.zip) herunter und entpacke es.
2.  Öffne nun die Arduino IDE und öffne die TemperaturDisplay.ino.
3.  Nun schließen Sie ihren Arduino an und wählen unter Werkzeuge das passende Board und den passenden Port aus.
4.  Sie können jetzt noch verschiedene einstellungen vornehmen z.B. die Pins ändern. Außerdem müssen sie dort die Min und Max Temperatur eingeben, da eine Steuerung über buttons noch nicht implementiert wurde.
5.  Schließen sie nun die verschiedenen Komponenten wie folgt an:
    *   Schließen sie das LCD Display über einen I2C Adapter an. Die Standard belegung sieht wie folgt aus:
        *   VCC -> +5V
        *   GND -> GND
        *   SDA -> A4
        *   SCL -> A5
    *   Schließen sie nun den DHT22 Sensor an. Dabei sieht die belegung wie folgt aus:
        *   DAT -> 2
        *   VCC -> +5V
        *   GND -> GND
        *   Verbinden sie falls der Sensor nicht auf einer Platine mit Wiederstand ist VCC und DAT durch einen 10 KOhm Wiederstand.
    *   Schließen sie nun ein Relais o.ä. mit der Heizung an Pin 4 und GND an.
6.  Klicken sie nun auf den Button mit einem Pfeil der nach rechts zeigt und der Sketch wird auf den Arduino Hochgeladen.

Hier ein möglicher aufbau:

![fritzing grafik](https://gitlab.com/johuck/arduino-heat-controll-lcd/raw/master/bauplan_Steckplatine.png)

To-Do
-----
*   Unterstützung für andere Temperatur sensoren.
