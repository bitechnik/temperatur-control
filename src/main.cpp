#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int serialMode = 2;

const int dht22Pin = 2; // DHT22 daten Pin
const int heatPin = 4; //Pin zur Heizungssteuerung (An = Heizen, Aus = nicht Heizen)

float tempMin = 23.9; // Minimale Temperatur (Heizung geht an wenn kleiner)
float tempMax = 24.5; // Maximale Temperatur (Heizung geht aus wenn größer)

float t; // Variable für die Temperatur
float h; // Variable für die Luftfeuchtigkeit

char b = 0; // Biene Teil1 write Variable
char i = 1; // Biene Teil2 write Variable

int cmd;
int count = 0;

// function prototypes
void logo(int zeile);
void temp();
void serialOut();

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
  delay(1000); // Pause
  lcd.setCursor(0,0);
  sensors.begin();
  pinMode(5, INPUT);
}

void loop() {
  // Code in loop wird dauernd wiederholt
  lcd.setBacklight(HIGH);
  cmd = Serial.parseInt(); // Serielle schnittstelle auslesen
  if (digitalRead(5) == HIGH) {
    t = dht.readTemperature(); // Temperatur auslesen
    h = dht.readHumidity(); // Luftfeuchtigkeit auslesen
  }else if (digitalRead(5) == LOW){
    sensors.requestTemperatures();
    t = sensors.getTempCByIndex(0);
    h = 0.0;
  }
  
  if(isnan(t) ||isnan(h) ||t == -127.00){ // überprüfen ob fehlerhafte werte vom Sensor eingelesen wurden
    // Fehlermeldung falls werte fehlerhaft
    Serial.println("Fehler! Die Werte konnten nicht ausgelesen werden!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("!!Fehler!!");
    lcd.setCursor(0,1);
    lcd.print("Sensor Problem!");
    digitalWrite(heatPin, LOW);
  }else{
    // Falls Werte korrrekt, ausgabe der Werte
    // Ausgabe über die Serielle schnittstelle:
    serialOut;
    count++;
    if (count <= 3) {
      logo(0);
    }else{ // if (count > 3)
      temp();
      if (count >= 8){
        count = 0;
      }
    }
    // Heizungssteuerung
    if (t > tempMax){               //Wenn Temperatur größer als die maximal Temperatur ist
      digitalWrite(heatPin, LOW);     //wird die Heizung abgeschaltet
    }else if (t < tempMin) {        //Sonst wenn die temperatur kleiner als die mindest Temperatur ist
      digitalWrite(heatPin, HIGH);    //wird die Heizung angeschaltet
    }
  }
}

void serialOut() {
  if (cmd == 3) {
    serialMode = 1;
  }else if (cmd == 4){
    serialMode = 2;
  }else if(cmd == 5){
    serialMode = 3;
  }else if(cmd == 6){
    serialMode = 4;
  }
  if (serialMode == 1){
    Serial.print("Die Temperatur beträgt: ");
    Serial.print(t);
    Serial.println("°C");
    Serial.print("Die Luftfeuchtigkeit beträgt: ");
    Serial.print(h);
    Serial.println("%");
  }else if(serialMode == 4){
    Serial.println(h);
  }else if(serialMode == 3){
    Serial.println(t);
  }else if(serialMode == 2){
    Serial.print(t);//*2.5
    Serial.print(" ");
    Serial.println(h);
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