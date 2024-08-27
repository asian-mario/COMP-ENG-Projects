#include <Wire.h>
#include <Keyboard.h>
#include <hd44780.h> 
#include <hd44780ioClass/hd44780_I2Cexp.h> 

hd44780_I2Cexp lcd;

const int buzzerPin = 16;

// Pin assignments for buttons
const int buttonPinC = 8;
const int buttonPinW = 15;
const int buttonPinA = 4;
const int buttonPinS = 6;
const int buttonPinD = 9;

String inputSequence = "";
unsigned long debounceDelay = 50;

struct Stratagem {
  String sequence;
  String name;
  String tag; // New tag category
};

Stratagem stratagems[] = {
  {"WASD", "Eagle Strafing Run", "attack"},
  {"WSDS", "Eagle Airstrike", "attack"},
  {"WASS", "Eagle Cluster Bomb", "attack"},
  {"WAWS", "Eagle Napalm Airstrike", "attack"},
  {"SSSD", "Eagle Smoke Strike", "support"},
  {"WWAD", "Eagle 110MM Rocket Pods", "attack"},
  {"SSAD", "Eagle 500kg Bomb", "attack"},
  // Add more stratagems with their respective tags
};

byte smallExplosion[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte mediumExplosion[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte largeExplosion[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100
};

byte fullExplosion[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100
};

byte plane[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00110,
  0b01110,
  0b00110,
  0b00100,
  0b00100
};

void setup() {
  lcd.begin(16, 2);
  lcd.print("Ready");

  Keyboard.begin();

  pinMode(buttonPinC, INPUT_PULLUP);
  pinMode(buttonPinW, INPUT_PULLUP);
  pinMode(buttonPinA, INPUT_PULLUP);
  pinMode(buttonPinS, INPUT_PULLUP);
  pinMode(buttonPinD, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (digitalRead(buttonPinC) == LOW) {
    inputSequence = "";
    Serial.println("beep");
    lcd.clear();
    lcd.print("COMMAND STARTED");
    inputSequence = ""; 
    delay(500); 
  }

  checkAndAddKey(buttonPinW, 'W');
  checkAndAddKey(buttonPinA, 'A');
  checkAndAddKey(buttonPinS, 'S');
  checkAndAddKey(buttonPinD, 'D');

  if (checkStratagem()) {
    lcd.clear();
    lcd.print("Stratagem:");
    lcd.setCursor(0, 1);
    int stratagemIndex = findStratagem();
    lcd.print(stratagems[stratagemIndex].name);
    delay(1000);
    displayTagMessage(stratagems[stratagemIndex].tag); // Display message or animation based on the tag
    for (int i = 0; i < 3; i++) {
      tone(buzzerPin, 1000, 100);
      delay(200);
    }
    Serial.println("stratagem_confirmed");
    inputSequence = "";  // Reset the sequence after a match
  }
}

void checkAndAddKey(int buttonPin, char keyChar) {
  static unsigned long lastDebounceTime = 0;

  if (digitalRead(buttonPin) == LOW) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      inputSequence += keyChar;
      Serial.println("beep");
      lcd.clear();
      lcd.print(inputSequence);
      Keyboard.press(keyChar);
      tone(buzzerPin, 1000, 100);
      delay(100);
      Keyboard.releaseAll();
      lastDebounceTime = millis();
    }
  }
}

bool checkStratagem() {
  for (int i = 0; i < sizeof(stratagems) / sizeof(Stratagem); i++) {
    if (inputSequence == stratagems[i].sequence) {
      return true;
    }
  }
  return false;
}

int findStratagem() {
  for (int i = 0; i < sizeof(stratagems) / sizeof(Stratagem); i++) {
    if (inputSequence == stratagems[i].sequence) {
      return i;
    }
  }
  return -1;  // Should not happen if checkStratagem is true
}

void scrollText(String message, int row) {
  int len = message.length();

  for (int i = 0; i < len - 15; i++) {
    lcd.clear();
    lcd.setCursor(0, row);
    lcd.print(message.substring(i, i + 16));
    delay(300); // Adjust delay for scroll speed
  }
}

void animateNukeExplosion() {
  for (int i = 0; i < 14; i++) { // Move dot from left to right
    lcd.clear();
    lcd.setCursor(i, 0);
    lcd.write(byte(4)); // Display dot
    delay(100); // Adjust delay for speed
  }

  // Stage 1: Small explosion at the bottom
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.write(byte(0)); // Display small explosion
  delay(300); // Adjust delay for speed

  // Stage 2: Medium explosion, expanding upward
  lcd.setCursor(6, 1);
  lcd.write(byte(1)); // Display medium explosion
  lcd.setCursor(7, 0);
  lcd.write(byte(3)); // Display full explosion on top row
  lcd.setCursor(8, 1);
  lcd.write(byte(1)); // Display medium explosion
  delay(300);

  // Stage 3: Large explosion, further expanding upward
  lcd.setCursor(5, 1);
  lcd.write(byte(2)); // Display large explosion
  lcd.setCursor(9, 1);
  lcd.write(byte(2)); // Display large explosion
  lcd.setCursor(8, 0);
  lcd.write(byte(3)); // Display full explosion on top row
  delay(300);

  // Stage 4: Full explosion covering both rows

  lcd.setCursor(4, 1);
  lcd.write(byte(3)); // Display full explosion
  lcd.setCursor(10, 1);
  lcd.write(byte(3)); // Display full explosion
  lcd.setCursor(11, 1);
  lcd.write(byte(3)); // Display full explosion


  lcd.setCursor(6, 0);
  lcd.write(byte(3)); // Display full explosion on top row
  lcd.setCursor(9, 0);
  lcd.write(byte(3)); // Display full explosion on top row
  delay(300);

  // Final stage: Wipeout
  delay(1000); // Hold the final display
  lcd.clear();
  lcd.setCursor(0, 0);
  
  scrollText("GODSPEED HELLDIVER!", 0); // Display message after explosion
  delay(2000); // Hold the final display

  lcd.clear(); // Clear the screen
}

void displayTagMessage(String tag) {
  if (tag == "attack") {
    lcd.clear();
    animateNukeExplosion();
  } else if (tag == "support") {
    lcd.clear();
    lcd.print("Support Mode");
    // Add more animations or sounds here for support
  } else if (tag == "defense") {
    lcd.clear();
    lcd.print("Defense Mode");
    // Add more animations or sounds here for defense
  }
}
