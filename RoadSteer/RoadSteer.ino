#include <LiquidCrystal.h> // LCD display library

// the pins used by the LCD shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define button constants
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//game field size is 16x4 for 16x2 LCD
#define WIDTH 16
#define HEIGHT 4
#define MAX_ENEMIES 5

long int framerate=0; //delay between game steps
byte currentMap = 0; //current character map
byte enemyCount = 1;
byte level = 1;
int enemiesPassed = 0;
byte iteration = 0;

char screenBuffer[HEIGHT/2][WIDTH+1]; //characters to be displayed on the screen

//Define custom characters for game sprites as 6x8x8 array:
//6 layers of 8 custom characters, each character is coded by 8 bytes

byte spritemap[][8][8] = {
//layer 0
{ 
  //sprite 0
  {
    B00111,
    B00100,
    B00000,
    B00001,
    B00000,
    B00100,
    B00000,
    B00001
  },
  //sprite 1
  {
    B00111,
    B01010,
    B01111,
    B01010,
    B00000,
    B00100,
    B00000,
    B00001
  },
  //sprite 2
  {
    B00111,
    B00100,
    B00000,
    B00001,
    B01010,
    B01111,
    B01010,
    B00000
  },
  //sprite 3
  {
    B00111,
    B01010,
    B01111,
    B01010,
    B01010,
    B01111,
    B01010,
    B00000
  },
  //sprite 4
  {
    B00000,
    B01010,
    B01111,
    B01010,
    B01000,
    B00000,
    B00000,
    B00111
  },
  //sprite 5
  {
    B01000,
    B00000,
    B00000,
    B00000,
    B01010,
    B01111,
    B01010,
    B00111
  },
  //sprite 6
  {
    B00000,
    B01010,
    B01111,
    B01010,
    B01010,
    B01111,
    B01010,
    B00111
  },
  //sprite 7
  {
    B01000,
    B00000,
    B00000,
    B00000,
    B01000,
    B00000,
    B00000,
    B00111
  }
},
//layer 1
{ 
  //sprite 0
  {
    B01110,
    B01000,
    B00000,
    B00010,
    B00000,
    B01000,
    B00000,
    B00010
  },
  //sprite 1
  {
    B01110,
    B01010,
    B11111,
    B01010,
    B00000,
    B01000,
    B00000,
    B00010
  },
  //sprite 2
  {
    B01110,
    B01000,
    B00000,
    B00010,
    B01010,
    B11111,
    B01010,
    B00000
  },
  //sprite 3
  {
    B01110,
    B01010,
    B11111,
    B01010,
    B01010,
    B11111,
    B01010,
    B00000
  },
  //sprite 4
  {
    B00000,
    B01010,
    B11111,
    B01010,
    B10000,
    B00000,
    B00001,
    B01110
  },
  //sprite 5
  {
    B10000,
    B00000,
    B00001,
    B00000,
    B01010,
    B11111,
    B01010,
    B01110
  },
  //sprite 6
  {
    B00000,
    B01010,
    B11111,
    B01010,
    B01010,
    B11111,
    B01010,
    B01110
  },
  //sprite 7
  {
    B10000,
    B00000,
    B00001,
    B00000,
    B10000,
    B00000,
    B00001,
    B01110
  }
},
//layer 2
{ 
  //sprite 0
  {
    B11100,
    B10000,
    B00000,
    B00100,
    B00000,
    B10000,
    B00000,
    B00100
  },
  //sprite 1
  {
    B11100,
    B11010,
    B11111,
    B11010,
    B00000,
    B10000,
    B00000,
    B00100
  },
  //sprite 2
  {
    B11100,
    B10000,
    B00000,
    B00100,
    B11010,
    B11111,
    B11010,
    B00000
  },
  //sprite 3
  {
    B11100,
    B11010,
    B11111,
    B11010,
    B11010,
    B11111,
    B11010,
    B00000
  },
  //sprite 4
  {
    B00000,
    B11010,
    B11111,
    B11010,
    B00000,
    B00000,
    B00010,
    B11100
  },
  //sprite 5
  {
    B00000,
    B00000,
    B00010,
    B00000,
    B11010,
    B11111,
    B11010,
    B11100
  },
  //sprite 6
  {
    B00000,
    B11010,
    B11111,
    B11010,
    B11010,
    B11111,
    B11010,
    B11100
  },
  //sprite 7
  {
    B00000,
    B00000,
    B00010,
    B00000,
    B00000,
    B00000,
    B00010,
    B11100
  }
},
//layer 3
{ 
  //sprite 0
  {
    B11000,
    B00000,
    B00000,
    B01000,
    B00000,
    B00000,
    B00000,
    B01000
  },
  //sprite 1
  {
    B11000,
    B01010,
    B11111,
    B01010,
    B00000,
    B00000,
    B00000,
    B01000
  },
  //sprite 2
  {
    B11000,
    B00000,
    B00000,
    B01000,
    B01010,
    B11111,
    B01010,
    B00000
  },
  //sprite 3
  {
    B11000,
    B01010,
    B11111,
    B01010,
    B01010,
    B11111,
    B01010,
    B00000
  },
  //sprite 4
  {
    B00000,
    B01010,
    B11111,
    B01010,
    B00001,
    B00000,
    B00100,
    B11000
  },
  //sprite 5
  {
    B00001,
    B00000,
    B00100,
    B00000,
    B01010,
    B11111,
    B01010,
    B11000
  },
  //sprite 6
  {
    B00000,
    B01010,
    B11111,
    B01010,
    B01010,
    B11111,
    B01010,
    B11000
  },
  //sprite 7
  {
    B00001,
    B00000,
    B00100,
    B00000,
    B00001,
    B00000,
    B00100,
    B11000
  }
},
//layer 4
{ 
  //sprite 0
  {
    B10001,
    B00001,
    B00000,
    B10000,
    B00000,
    B00001,
    B00000,
    B10000
  },
  //sprite 1
  {
    B10001,
    B11010,
    B11111,
    B11010,
    B00000,
    B00001,
    B00000,
    B10000
  },
  //sprite 2
  {
    B10001,
    B00001,
    B00000,
    B10000,
    B11010,
    B11111,
    B11010,
    B00000
  },
  //sprite 3
  {
    B10001,
    B11010,
    B11111,
    B11010,
    B11010,
    B11111,
    B11010,
    B00000
  },
  //sprite 4
  {
    B00000,
    B11010,
    B11111,
    B11010,
    B00010,
    B00000,
    B01000,
    B10001
  },
  //sprite 5
  {
    B00010,
    B00000,
    B01000,
    B00000,
    B11010,
    B11111,
    B11010,
    B10001
  },
  //sprite 6
  {
    B00000,
    B11010,
    B11111,
    B11010,
    B11010,
    B11111,
    B11010,
    B10001
  },
  //sprite 7
  {
    B00010,
    B00000,
    B01000,
    B00000,
    B00010,
    B00000,
    B01000,
    B10001
  }
},
//layer 5
{ 
  //sprite 0
  {
    B00011,
    B00010,
    B00000,
    B00000,
    B00000,
    B00010,
    B00000,
    B00000
  },
  //sprite 1
  {
    B00011,
    B01010,
    B01111,
    B01010,
    B00000,
    B00010,
    B00000,
    B00000
  },
  //sprite 2
  {
    B00011,
    B00010,
    B00000,
    B00000,
    B01010,
    B01111,
    B01010,
    B00000
  },
  //sprite 3
  {
    B00011,
    B01010,
    B01111,
    B01010,
    B01010,
    B01111,
    B01010,
    B00000
  },
  //sprite 4
  {
    B00000,
    B01010,
    B01111,
    B01010,
    B00100,
    B00000,
    B10000,
    B00011
  },
  //sprite 5
  {
    B00100,
    B00000,
    B10000,
    B00000,
    B01010,
    B01111,
    B01010,
    B00011
  },
  //sprite 6
  {
    B00000,
    B01010,
    B01111,
    B01010,
    B01010,
    B01111,
    B01010,
    B00011
  },
  //sprite 7
  {
    B00100,
    B00000,
    B10000,
    B00000,
    B00100,
    B00000,
    B10000,
    B00011
  }
}
};


//Base class for game objects
class GameObject
{
//Object's coordinates and speed
protected:
  int8_t _x;
  int8_t _y;
  int8_t _speed;
public:
  GameObject():_x(0),_y(0),_speed(0){}
  GameObject(int8_t x, int8_t y): _x(x), _y(y), _speed(0){}
  GameObject(int8_t x, int8_t y, int8_t speed): _x(x), _y(y), _speed(speed){}
//Getters and setters
  int8_t x() const{
    return _x;
  }
  
  int8_t y() const{
    return _y;
  }
  int8_t speed() const{
    return _speed;
  }
  bool setX(int8_t x){
    if (x<0)
      return false;
    _x = x;
    return true;
  }
  bool setY(int8_t y){
    if ((y<0)||(y>=HEIGHT))
      return false;
    _y = y;
    return true;
  }
  bool setSpeed(int8_t speed){
    _speed = speed;
    return true;
  }
//Collision detection
  bool collides(const GameObject& o){
    return ((_x==o.x())&&(_y==o.y())) ? true : false;
  }
};

GameObject player, enemies[MAX_ENEMIES];

/* Processing buttons state*/
byte buttonPressed()
{
 int adc_key_in = analogRead(0);      // read analogue value
//Please adjust these values for your version of LCD shield
 if (adc_key_in <= 60)   return btnRIGHT;
 if (adc_key_in <= 200)  return btnUP;
 if (adc_key_in <= 400)  return btnDOWN;
 if (adc_key_in <= 600)  return btnLEFT;
 if (adc_key_in <= 800)  return btnSELECT;
 return btnNONE;
}

/*Update LCD screen
First drawing in a character buffer, then print it to the screen to avoid flickering.*/
void updateScreen(){
//Clearing the buffer
for(byte i = 0; i<WIDTH; i++){
  screenBuffer[0][i]=0;
  screenBuffer[1][i]=7;
}

//drawing the player
switch(player.y()){
case 0:
  screenBuffer[0][0] = 1;
  break;
case 1:
  screenBuffer[0][0] = 2;
  break;
case 2:
  screenBuffer[1][0] = 4;
  break;
case 3:
  screenBuffer[1][0] = 5;
  break;
}

//drawing enemies
for (byte enemyNum=0; enemyNum<enemyCount; enemyNum++)
{
  if(enemies[enemyNum].x()>WIDTH-1)
    continue;
  switch(enemies[enemyNum].y()){
  case 0:
    if (screenBuffer[0][enemies[enemyNum].x()]==0)
      screenBuffer[0][enemies[enemyNum].x()] = 1;
    else 
      screenBuffer[0][enemies[enemyNum].x()] = 3;
    break;
  case 1:
    if (screenBuffer[0][enemies[enemyNum].x()]==0)
      screenBuffer[0][enemies[enemyNum].x()] = 2;
    else
      screenBuffer[0][enemies[enemyNum].x()] = 3;
    break;
  case 2:
    if (screenBuffer[1][enemies[enemyNum].x()]==7)
      screenBuffer[1][enemies[enemyNum].x()] = 4;
    else
      screenBuffer[1][enemies[enemyNum].x()] = 6;
    break;
  case 3:
    if (screenBuffer[1][enemies[enemyNum].x()]==7)
      screenBuffer[1][enemies[enemyNum].x()] = 5;
    else
      screenBuffer[1][enemies[enemyNum].x()] = 6;
    break;
  }
}
//Sending the buffer to the screen
  for (byte i = 0; i < HEIGHT/2; i++)
  {
    lcd.setCursor(0,i);
    for (byte j=0;j<WIDTH;j++)
      lcd.print(screenBuffer[i][j]);
  }
}

/*Game over screen*/
void gameOver()
{
  lcd.setCursor(0,0);
  lcd.print("Game over");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.setCursor(7,1);
  lcd.print(enemiesPassed);
  while(1){
    for (byte i = 0; i<8; i++)
      lcd.createChar(i, spritemap[currentMap][i]);
    delay(20);
    currentMap+=1;
    if (currentMap>5)
      currentMap = 0;
  }
}

/*Init: starting LCD and initializing variables*/
void setup()
{
 lcd.begin(16, 2);
 randomSeed(analogRead(1));
 player.setX(0);
 player.setY(0);
 enemyCount = 1;
 framerate=20; //delay between game steps
 currentMap = 0;
 level = 1;
 enemiesPassed = 0;
 enemies[0].setY(random(4));
 enemies[0].setX(WIDTH-1);
 iteration = 0;
}

/*Game loop*/
void loop()
{
//Processing the buttons
  if (currentMap%2){//feel free to remove this condition to increase manoeuvrability
  switch(buttonPressed()){
  case btnUP:{
    player.setY(player.y()-1);
    break;
  }
  case btnDOWN:{
    player.setY(player.y()+1);
    break;
  }
  default:
    break;
  }
  }
  //swithing current character map
  for (byte i = 0; i<8; i++)
    lcd.createChar(i, spritemap[currentMap][i]);
  currentMap++;
  //moving objects
  if (currentMap>5){
    currentMap = 0;
    for (byte enemyNum=0; enemyNum<enemyCount; enemyNum++)
    {
      if(!enemies[enemyNum].x()){
        if (enemies[enemyNum].y()==player.y())
          gameOver();
        enemies[enemyNum].setY(random(4));
        enemies[enemyNum].setX(WIDTH+random(20));
        enemiesPassed++;
        if (!(enemiesPassed%2)){//switch to the next level
          level++;
          if (level<10)
            framerate = (10-level)*2;
          else framerate = 0;
          enemyCount++;
          if (enemyCount>MAX_ENEMIES)
            enemyCount = MAX_ENEMIES;
          enemies[enemyCount-1].setY(random(4));
          enemies[enemyCount-1].setX(WIDTH+random(20));
        }
      }
      //move enemies once per 3 animation iterations
      else if (!iteration){
        enemies[enemyNum].setX(enemies[enemyNum].x()-1);
      if ((enemies[enemyNum].x()<10)&&(enemies[enemyNum].y()>player.y()))
        enemies[enemyNum].setY(enemies[enemyNum].y()-1);
      if ((enemies[enemyNum].x()<10)&&(enemies[enemyNum].y()<player.y()))
        enemies[enemyNum].setY(enemies[enemyNum].y()+1);
      }
    }
    if (++iteration == 3)
      iteration = 0;
    updateScreen();
  }
  delay (framerate);
}
