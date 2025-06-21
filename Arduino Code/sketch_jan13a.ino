#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DFPlayer Mini bağlantıları
SoftwareSerial mySoftwareSerial(A2, A1);  // RX (A2), TX (A1)
DFRobotDFPlayerMini myDFPlayer;

// LCD tanımları
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Butonların bağlı olduğu pinlerin listesi
const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0}; // 12 buton
int lastButtonState[12]; // Butonların önceki durumları

// Notalar dizisi
const char* notes[] = {
  "Do",   // Button 1
  "Do Majör",  // Button 2
  "Re",   // Button 3
  "Re Minör",  // Button 4
  "Mi",   // Button 5
  "Fa",   // Button 6
  "Sol Majör",  // Button 7
  "Sol",   // Button 8
  "Sol Minör",  // Button 9
  "La",   // Button 10
  "La Majör",  // Button 11
  "Si"    // Button 12
};

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("DFPlayer Mini modülü başlatılamadı!"));
    while (true);
  }
  myDFPlayer.volume(30); // Ses seviyesi

  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); // Pull-up direnç aktif
    lastButtonState[i] = HIGH;           // Başlangıç durumu
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HAZIR!");
}

void loop() {
  for (int i = 0; i < 12; i++) {
    int currentButtonState = digitalRead(buttonPins[i]);
    if (lastButtonState[i] == HIGH && currentButtonState == LOW) {
      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.println(" pressed!");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nota: ");
      lcd.print(notes[i]);  // Notaları yazdır
      lcd.setCursor(0, 1);
      

      myDFPlayer.stop();        // Önceki dosyayı durdur
      myDFPlayer.play(i + 1);   // Oynat
    }
    lastButtonState[i] = currentButtonState;
  }
}
