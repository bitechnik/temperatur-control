#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

boolean startup = true;
boolean send = false;
unsigned long previousMillis = 0;

#define ONE_WIRE_BUS 6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int serialMode = 2;

const int dht22Pin = 2; // DHT22 daten Pin
const int heatPin = 4; //Pin zur Heizungssteuerung (An = Heizen, Aus = nicht Heizen)

float tempMin = 5; // Minimale Temperatur (Heizung geht an wenn kleiner)
float tempMax = 10; // Maximale Temperatur (Heizung geht aus wenn größer)

float t; // Variable für die Temperatur
float h; // Variable für die Luftfeuchtigkeit

char b = 0; // Biene Teil1 write Variable
char i = 1; // Biene Teil2 write Variable

int cmd;
int count = 3;

// function prototypes
void logo(int zeile);
void temp();
void serialPrint();
void sendData();
void getData(String input);
void getData();
void eepromWrite(int start, String string);
void debugPrint(String input);
String eepromRead(int start);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7 , 3, POSITIVE);
DHT dht(dht22Pin,DHT22);



byte bieneA[8] = { // Biene Teil 1
  B10000,
  B01100,
  B10011,
  B01101,
  B00101,
  B00010,
  B00001
};

byte bieneB[8] = { // Biene Teil 2
  B00100,
  B01001,
  B10010,
  B11100,
  B01010,
  B01001,
  B10010,
  B11100
};

void setup() {
  // Setup wird einmal ausgeführt
  Serial.begin(9600); // Serielle verbindung starten (baudrate 9600)
  lcd.begin(16, 2); // LCD initzialisieren
  lcd.clear();
  lcd.createChar(b, bieneA); // Biene Teil 1 erstellen
  lcd.createChar(i, bieneB); // Biene Teil 2 erstellen
  lcd.setBacklight(HIGH); // Hintergrund beleuchtung einschalten
  logo(0); // Imkerei Logo schreiben
  lcd.setCursor(0,1); // in zweite Zeile wechseln
  lcd.print("Hochfahren..."); // Hochfahren... schreiben
  dht.begin(); // Temp. und Luftf. Sensor initialisieren
  //delay(1000); // Pause
  lcd.setCursor(0,0);
  sensors.begin();
  pinMode(5, INPUT);
  getData(eepromRead(0));
  Serial.println("eeprom(0): " + eepromRead(0));
  delay(2500);
  Serial.println(String("#") + String("T") + 0.00 + String("H") + 0.00 + String("S") + tempMin + String("X") + tempMax + String(";"));
}

void loop() {
  getData();
  sendData();
  if (startup == true) {
    startup = false;
  }
  // Code in loop wird dauernd wiederholt
  lcd.setBacklight(HIGH);
  cmd = Serial.parseInt(); // Serielle schnittstelle auslesen
  if (digitalRead(5) == LOW) {
    t = dht.readTemperature(); // Temperatur auslesen
    h = dht.readHumidity(); // Luftfeuchtigkeit auslesen
  }else{
    sensors.requestTemperatures();    // delay(1000)
    t = sensors.getTempCByIndex(0);
    h = 0.0;
  }

  if(isnan(t) ||isnan(h) ||t == -127.00){ // überprüfen ob fehlerhafte werte vom Sensor eingelesen wurden
    // Fehlermeldung falls werte fehlerhaft
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("!!Fehler!!");
    lcd.setCursor(0,1);
    lcd.print("Sensor Problem!");
    digitalWrite(heatPin, LOW);
    t=0;
    h=0;
  }else{
    // Falls Werte korrrekt, ausgabe der Werte
    // Ausgabe über die Serielle schnittstelle:
    count++;
    if (count <= 3) {
      logo(0);
    }else{
      temp();
      if (count >= 10){
        count = 0;
      }
    }
    // Heizungssteuerung
    if (t > tempMax){               //Wenn Temperatur größer als die maximal Temperatur ist
      digitalWrite(heatPin, LOW);     //wird die Heizung abgeschaltet
    }else if(t < tempMin){        //Sonst wenn die temperatur kleiner als die mindest Temperatur ist
      digitalWrite(heatPin, HIGH);    //wird die Heizung angeschaltet
    }
  }
}

void eepromWrite(int start, String string) {
  char charToSave[100];
  string.toCharArray(charToSave, 100);
  EEPROM.put(start,charToSave);
  Serial.println("EEPROM written: " + eepromRead(start));
}

String eepromRead(int start) {
  char Buffer[100] = {0};
  EEPROM.get(start, Buffer);
  return Buffer;
}

void debugPrint(String input) {
  lcd.clear();
  lcd.print(input);
  Serial.print(input);
}
void sendData() {
  long currentMillis = millis();
  int interval = 1000;
  if (send == true && currentMillis - previousMillis >= interval){
    String data = String("#") + String("T") + t + String("H") + h + String("S") + tempMin + String("X") + tempMax + String(";");
    Serial.println(data);
    send = false;
  }
}
void getData(String input) {
  float newMin = 0;
  float newMax = 0;
  if (input.charAt(0) == 'S' && input.charAt(input.length()-1) == ';'){
    newMin = input.substring(input.indexOf("S")+1,input.indexOf("X")-1).toFloat();
    newMax = input.substring(input.indexOf("X")+1,input.indexOf(";")-1).toFloat();
    tempMin = newMin;
    tempMax = newMax;
  }
  send = true;
  previousMillis = millis();
}
void getData() {
  while (Serial.available()) {
    String input = Serial.readString();
    float newMin = 0;
    float newMax = 0;
    if (input.charAt(0) == 'S' && input.charAt(input.length()-1) == ';'){
      if (millis() > 11000) {
        newMin = input.substring(input.indexOf("S")+1,input.indexOf("X")-1).toFloat();
        newMax = input.substring(input.indexOf("X")+1,input.indexOf(";")-1).toFloat();
      }
      if (newMin != tempMin || newMax != tempMax) {
        eepromWrite(0, String("S") + newMin + String("X") + String(newMax) + String(";"));
      }
      tempMin = newMin;
      tempMax = newMax;
    }
    send = true;
    previousMillis = millis();
  }
}
void temp() { // Sensor Werte auf Display anzeigen
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp.: ");
  lcd.print(t);
  lcd.setCursor(0,1);
  lcd.print("Luftf.: ");
  lcd.print(h);
}

void logo(int zeile) { // Logo auf Display zeigen. Die zeile wird als parameter angegeben.
  lcd.clear();
  lcd.setCursor(0,zeile);
  lcd.print("WS Imkerei ");
  lcd.write(b);
  lcd.write(i);
  lcd.print(" ");
  lcd.write(b);
  lcd.write(i);
}
