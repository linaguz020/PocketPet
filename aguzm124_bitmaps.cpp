/*         Angelina Guzman aguzm124@ucr.edu
 *         Discussion Section: 21
 *         Assignment: Custom Lab Project

 *         Bitmaps for Pocket Pet. Uses arrays to write to pixels on the Nokia 5110 LCD screen.

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 */

void cat1(int cursX, int cursY) {
  static const byte cat1[] =  {B11100000, B00100000, B01000000, B10000000, B10000000};
  static const byte cat2[] = {B01000000, B00100000, B00100000, B11100000, B00000000};
  static const byte cat3[] = {B00000111, B00001000, B11110011, B10000100, B11100000};
  static const byte cat4[] = {B10000011, B11100000, B01000000, B11000111, B11100100};
  static const byte cat5[] = {B10001000, B11110110, B00010001, B00001101, B00000110};
  lcd.createChar(0, cat1);
  lcd.createChar(1, cat2);
  lcd.createChar(2, cat3);
  lcd.createChar(3, cat4);
  lcd.createChar(4, cat5);
  
  lcd.setCursor(cursX, cursY);
  lcd.write(0);
  lcd.setCursor(cursX + 5, cursY);
  lcd.write(1);
  lcd.setCursor(cursX, cursY+1);
  lcd.write(2);
  lcd.setCursor(cursX+5, cursY+1);
  lcd.write(3);
  lcd.setCursor(cursX+10, cursY+1);
  lcd.write(4);
}

void dog1(int cursX, int cursY) {
  static const byte dog1[] = {B00010000, B01101000, B10010000, B10001100, B10000010};
  static const byte dog2[] = {B10011001, B10000001, B01100001, B10000001, B10000001};
  static const byte dog3[] = {B10001100, B11110000, B00000000, B00000000, B10000000};
  static const byte dog4[] = {B01000000, B10000000, B00000000, B00000000, B00000000};
  static const byte dog5[] = {B00000000, B00000000, B00000000, B01000000, B10100000};
  static const byte dog6[] = {B10011111, B10000010, B00100001, B00100000, B00100000};
  static const byte dog7[] = {B00100001, B00100010, B01000010, B10011001, B10100100};
  static const byte dog8[] = {B01000010, B00000001, B00000000, B00000000, B00000000};
  
  lcd.createChar(0, dog1);
  lcd.createChar(1, dog2);
  lcd.createChar(2, dog3);
  lcd.createChar(3, dog4);
  lcd.createChar(4, dog5);
  lcd.createChar(5, dog6);
  lcd.createChar(6, dog7);
  lcd.createChar(7, dog8);

  lcd.setCursor(cursX, cursY);
  lcd.write(0);
  lcd.setCursor(cursX+5, cursY);
  lcd.write(1);
  lcd.setCursor(cursX+10, cursY);
  lcd.write(2);
  lcd.setCursor(cursX+15, cursY);
  lcd.write(3);
  lcd.setCursor(cursX, cursY+1);
  lcd.write(4);
  lcd.setCursor(cursX+5, cursY+1);
  lcd.write(5);
  lcd.setCursor(cursX+10, cursY+1);
  lcd.write(6);
  lcd.setCursor(cursX+15, cursY+1);
  lcd.write(7);
  }

void foodBowl(int cursX, int cursY) {
  static const byte bowl1[] = {B10000000, B11000000, B11100000, B11110000, B11100000};
  static const byte bowl2[] = {B11000000, B10000000, B00000000, B00000000, B00000000};
  
  lcd.createChar(0, bowl1);
  lcd.createChar(1, bowl2);
  
  lcd.setCursor(cursX, cursY);
  lcd.write(0);
  lcd.setCursor(cursX+5, cursY);
  lcd.write(1);
}

void healthBar(int numFull, int cursX, int cursY) {
  int diff = 10 - numFull;
  static const byte emptyBox[] = {B11111111, B10000001, B10000001, B10000001, B11111111};
  static const byte fullBox[] = {B11111111, B11111111, B11111111, B11111111, B11111111};

  lcd.createChar(0, emptyBox);
  lcd.createChar(1, fullBox);

  lcd.setCursor(cursX, cursY);
  for(int i = 0; i < numFull; i++) {
    lcd.write(1);
    }
  for(int k = 0; k < diff; k++) {
    lcd.write(0);
    }
  }
