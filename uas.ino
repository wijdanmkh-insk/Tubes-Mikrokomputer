#include <Keypad.h>

//Define keypad buttons
const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {10, 11, 12, 13}; 
byte colPins[COLS] = {A0, A1, A2, A3}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

const int interruptPin = 18; 
volatile bool interruptFlag = false;

const int potPin = A5; 
int potValue = 0; 

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  Serial.begin(9600);

void loop() {
  // Nyala dari tengah ke kiri dan kanan
  for (int i = 3; i >= 0; i--) {
    digitalWrite(ledPins[4 + i], HIGH);
    digitalWrite(ledPins[3 - i], HIGH);
    delay(500);
    digitalWrite(ledPins[4 + i], LOW);
    digitalWrite(ledPins[3 - i], LOW);
  }

  // Nyala dari kiri dan kanan kembali ke tengah
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[4 + i], HIGH);
    digitalWrite(ledPins[3 - i], HIGH);
    delay(500);
    digitalWrite(ledPins[4 + i], LOW);
    digitalWrite(ledPins[3 - i], LOW);
  }

  while (true) {
    digitalWrite(ledPins[4], HIGH);
    digitalWrite(ledPins[3], HIGH);
    delay(500);
    digitalWrite(ledPins[4], LOW);
    digitalWrite(ledPins[3], LOW);
    delay(500);

    char key = keypad.getKey();
    if (key) {
      Serial.println(key); 

      if (key == '2') {
        while (key != '#') {
          for (int i = 0; i < 8; i++) {
            digitalWrite(ledPins[i], HIGH);
          }
          delay(500);
          for (int i = 0; i < 8; i++) {
            digitalWrite(ledPins[i], LOW);
          }
          delay(500);
          key = keypad.getKey();
        }
      } else if (key == '5') {
        for (int i = 0; i < 8; i++) {
          digitalWrite(ledPins[i], LOW);
        }

       
        while (key != '8') {
          potValue = analogRead(potPin); 
          float voltage = potValue * (5.0 / 1023.0); 
          Serial.print("Tegangan: ");
          Serial.print(voltage);
          Serial.println(" V");

          Serial.print("Nilai ADC: ");
          Serial.println(potValue);

          // Matikan semua LED terlebih dahulu
          for (int i = 0; i < 8; i++) {
            digitalWrite(ledPins[i], LOW);
          }

          // Nyalakan LED sesuai dengan level tegangan
          if (voltage > 0 && voltage <= 1) {
            digitalWrite(ledPins[0], HIGH);
            digitalWrite(ledPins[1], HIGH); 
          } else if (voltage > 1 && voltage <= 2) {
            for (int i = 0; i < 4; i++) {
              digitalWrite(ledPins[i], HIGH);
            }
          } else if (voltage > 2 && voltage <= 3) {
            for (int i = 0; i < 5; i++) {
              digitalWrite(ledPins[i], HIGH); 
            }
          } else if (voltage > 3 && voltage <= 4) {
            for (int i = 0; i < 6; i++) {
              digitalWrite(ledPins[i], HIGH); 
            }
          } else if (voltage > 4 && voltage <= 5) {
            for (int i = 0; i < 8; i++) {
              digitalWrite(ledPins[i], HIGH); 
            }
          }

          delay(100); 
          key = keypad.getKey();
        }
      } else if (key == '8') {
        break; 
      } else if (key == '*') {
        handleInterrupt(); 
      }
    }

    if (interruptFlag) {
      interruptFlag = false; 
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 4; i++) {
          digitalWrite(ledPins[i], HIGH); 
        }
        delay(500);
        for (int i = 0; i < 4; i++) {
          digitalWrite(ledPins[i], LOW); 
        }
        delay(500);
        for (int i = 4; i < 8; i++) {
          digitalWrite(ledPins[i], HIGH); 
        }
        delay(500);
        for (int i = 4; i < 8; i++) {
          digitalWrite(ledPins[i], LOW); 
        }
        delay(500);
      }
    }
  }
}

void handleInterrupt() {
  interruptFlag = true; 
}
