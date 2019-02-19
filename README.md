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

1. Installiere [Visual Studio Code](https://code.visualstudio.com), [Atom](https://atom.io) oder einen anderen editor der von [PlatformIO](https://platformio.org) unterstützt wird.
2. Öffne nun den Erweiterungs Manager (VScode linke seitenleiste ganz unten), suche nach PlatformIO und installiere es (kann länger dauern). Mehr dazu auch unter [platformio.org](https://platformio.org)
2.  Lade das Projekt Herunter, entpacke es und wähle den ordner über File->Open Folder aus.
3. Nun kannst du links unter src die main.cpp öffnen und wie in der Arduino IDE bearbeiten.
4.  Du kannst jetzt noch verschiedene einstellungen vornehmen z.B. die Pins ändern. Außerdem muss dort die Min und Max Temperatur eingeben, da eine Steuerung über buttons noch nicht implementiert wurde.
3.  Nun schließt du deinen Arduino an und klickst unten auf den Pfeil nach Rechts
5.  Schließe die verschiedenen Komponenten wie folgt an:
<img src="https://gitlab.com/johuck/arduino-heat-controll-lcd/raw/master/bauplan_Steckplatine.png" alt="drawing" width="500"/>



To-Do
-----

