#include <LiquidCrystal.h>

// LCD setup
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Button definitions
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Custom characters
byte planeTop0[8] = { B00100, B01110, B11111, B00000, B00000, B00000, B00000, B00000 };
byte planeBottom0[8] = { B00000, B00000, B00000, B00000, B00000, B01110, B11111, B00100 };
byte planeTop1[8] = { B00000, B00000, B00000, B00000, B00100, B01110, B11111, B00000 };
byte planeBottom1[8] = { B00000, B11111, B01110, B00100, B00000, B00000, B00000, B00000 };
byte asteroidTop[8] = { B00100, B01110, B11111, B11111, B00000, B00000, B00000, B00000 };
byte asteroidBottom[8] = { B00000, B00000, B00000, B00000, B11111, B11111, B01110, B00100 };
byte bulletTop[8] = { B00000, B00000, B00000, B00000, B00000, B00100, B00000, B00000 };
byte bulletBottom[8] = { B00000, B00000, B00100, B00000, B00000, B00000, B00000, B00000 };

// Game variables
int planeRow = 0;
int planeHalf = 0;
int score = 0;
unsigned long lastMove = 0;
unsigned long lastFire = 0;
int baseSpeed = 800;
int gameState = 0;

struct Bullet { int row, half, col; };
struct Asteroid { int row, half, col; };

Bullet bullets[5];
Asteroid asteroids[5];
byte bulletCount = 0;
byte asteroidCount = 0;

int readButton() {
  int key = analogRead(0);
  if (key < 50)    return btnRIGHT;
  if (key < 250)   return btnUP;
  if (key < 450)   return btnDOWN;
  if (key < 650)   return btnLEFT;
  if (key < 850)   return btnSELECT;
  return btnNONE;
}

void spawnAsteroid() {
  if (asteroidCount < 5) {
    asteroids[asteroidCount] = { random(0, 2), random(0, 2), 15 };
    asteroidCount++;
  }
}

void fireBullet() {
  if (bulletCount < 5) {
    bullets[bulletCount] = { planeRow, planeHalf, 1 };
    bulletCount++;
  }
}

void drawChar(int col, int row, byte top, byte bottom) {
  lcd.setCursor(col, row);
  for (int i = 0; i < asteroidCount; i++) {
    if (asteroids[i].row == row && asteroids[i].col == col) {
      lcd.write(asteroids[i].half ? 5 : 4);
      return;
    }
  }
  for (int i = 0; i < bulletCount; i++) {
    if (bullets[i].row == row && bullets[i].col == col) {
      lcd.write(bullets[i].half ? 7 : 6);
      return;
    }
  }
  lcd.write(' ');
}

void gameOver() {
  lcd.clear();
  lcd.print("Game Over!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  while(1) delay(1000);
}

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, planeTop0);
  lcd.createChar(1, planeBottom0);
  lcd.createChar(2, planeTop1);
  lcd.createChar(3, planeBottom1);
  lcd.createChar(4, asteroidTop);
  lcd.createChar(5, asteroidBottom);
  lcd.createChar(6, bulletTop);
  lcd.createChar(7, bulletBottom);
  randomSeed(analogRead(1));
}

void loop() {
  if (gameState) return;
  
  // Handle input
  int btn = readButton();
  if (btn == btnUP && planeHalf == 1) planeHalf = 0;
  if (btn == btnDOWN && planeHalf == 0) planeHalf = 1;
  if (btn == btnLEFT && planeRow == 1) planeRow = 0;
  if (btn == btnRIGHT && planeRow == 0) planeRow = 1;
  if (btn == btnSELECT) fireBullet();

  // Update game state
  if (millis() - lastMove > baseSpeed - (score * 10)) {
    // Move asteroids
    for (int i = 0; i < asteroidCount; i++) {
      if (--asteroids[i].col < 0) gameOver();
    }
    
    // Move bullets
    for (int i = 0; i < bulletCount; i++) {
      if (++bullets[i].col > 15) {
        bullets[i--] = bullets[--bulletCount];
      }
    }
    
    // Spawn asteroids
    if (random(100) < 20 + score) spawnAsteroid();
    lastMove = millis();
  }

  // Check collisions
  for (int i = 0; i < bulletCount; i++) {
    for (int j = 0; j < asteroidCount; j++) {
      if (bullets[i].row == asteroids[j].row &&
          bullets[i].half == asteroids[j].half &&
          bullets[i].col == asteroids[j].col) {
        score += 10;
        bullets[i--] = bullets[--bulletCount];
        asteroids[j--] = asteroids[--asteroidCount];
      }
    }
  }

  // Update display
  lcd.clear();
  lcd.setCursor(0, planeRow);
  lcd.write(planeRow * 2 + planeHalf);
  
  for (int r = 0; r < 2; r++) {
    for (int c = 1; c < 16; c++) {
      drawChar(c, r, 4, 5);
    }
  }
  
}
