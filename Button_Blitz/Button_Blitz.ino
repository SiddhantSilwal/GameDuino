#include <LiquidCrystal.h>

// LCD Keypad Shield pin setup
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int buttonPin = A0;  // Button input pin
int score = 0;

void setup() {
    lcd.begin(16, 2);
    lcd.print("Press to Start");
    Serial.begin(9600);  // Initialize Serial Monitor
    Serial.print("Initiated");
}

void loop() {
    int button = readButton();
    
    if (button != 0) {  // Any button press starts the game
        playGame();
    }
}

// Function to play the game
void playGame() {

      score = 0;
    lcd.clear();
    lcd.print("Get Ready...");
    delay(2000);
    int speed = 2000;
    
    while (true) {
        char symbols[] = {'>', '^', 'V', '<'};
        int index = random(0, 4);
        char symbol = symbols[index];
        int order = index +1;
        
        
        lcd.clear();
        lcd.print("Press: ");
        lcd.print(symbol);

        Serial.print("New Symbol: ");
        Serial.println(symbol);  // Print the required symbol

        unsigned long startTime = millis();
        while (millis() - startTime < speed) {  // 2-second window
            int button = readButton();
            if (button != 0) {

                // Print button name
                
                if (button == order) {  // Correct button
                    Serial.println("Correct!");
                    score++;
                    Serial.println(order);
                    Serial.println(score);
                    
                    if (score % 5 == 0)
                      {
                        speed = speed -100;
                      }

                      if (speed < 200)
                        {
                          speed = 200;
                        }
                    break;
                } else if (button =! order) {  // Wrong button
                    Serial.println("Wrong! Game Over.");
                    gameOver();
                    return;
                }
            }
        }



        if (millis() - startTime >= speed) {  // Timeout
            Serial.println("Timeout! Game Over.");
            gameOver();
            return;
        }
    }

}

// Function to handle game over
void gameOver() {
    lcd.clear();
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    Serial.print("Final Score: ");
    Serial.println(score);
    delay(3000);
    lcd.clear();
    lcd.print("Press to Start");
}

// Function to read button input
int readButton() {
    int val = analogRead(buttonPin);
    
    if (val < 50)   return 1; // Right
    if (val < 250)  return 2; // Up
    if (val < 450)  return 3; // Down
    if (val < 650)  return 4; // Left
    if (val < 850)  return 5; // Select
    
    return 0; // No button pressed
}
