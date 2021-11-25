#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <FastLED.h>
#include <LinkedList.h>

/* DEFINES */
#define DATA_PIN    8
#define NUM_LEDS    288
#define NUM_ROWS    17
#define NUM_COLS    17
#define BRIGHTNESS  30
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define DOT_SPEED 100

/* STRUCTURES */
struct Dot {
  int x, y;
} food, snakeHead, tempDot;

/* GLOBAL VARIABLES */
CRGB leds[NUM_LEDS];
int mmap[NUM_ROWS][NUM_COLS] = {};
int userInput = 0;
int dir = 0; // 1 = up, 2 = left, 3 = down, 4 = right
int debug = 1;
LinkedList<Dot> snake = LinkedList<Dot>();


void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
  randomSeed(analogRead(0));

  // create 2d array of LED lights. j = x, i = y
  // each value holds location of LED on strip 
  int curLED = -1; // first light is a phantom light
  for(int i = 16; i >= 0; i--) {
    for(int j = 16; j >= 0; j--) {
      mmap[j][i] = curLED;
      curLED++;
    }

    // decrease curLED and increment to get next row
    // if total rows are odd, make sure the last row(decrementing)
    // down is checked
    if(i >= 1) {
      i--; // going to next row up
      for(int j = 0; j <= 16; j++) {
        mmap[j][i] = curLED;
        curLED++;
      }
    }
  }

  // initialize border
  for(int i = 0; i <= 16; i++) {
    leds[mmap[0][i]] = CRGB::Blue; // left
    leds[mmap[i][0]] = CRGB::Blue; // top
    leds[mmap[16][i]] = CRGB::Blue; // right
    leds[mmap[i][16]] = CRGB::Blue; // bottom
  }

  // initialize snake
  leds[mmap[8][8]] = CRGB::Red;
  snakeHead.x = 8;
  snakeHead.y = 8;
  snake.add(snakeHead);
  addRandomDot();
  FastLED.show();
}

/* resets program */
void(* resetFunc) (void) = 0; // declare reset function at address 0

void loop() {
  // check if user pressed w,a,s,d key
  changeDirection();

  // move user in direction it's going
  moveUser(0);

  // check if user touches food
  if(checkFoodCollision()) {
    moveUser(1); // add next food to linkedlist
  }

  // check if user touches itself
  checkSnakeCollision();
  

    
}

/* checks if head of user touches another part of snake */
int checkSnakeCollision() {
  int snakeCollided = 0;
  for(int i = 1; i < snake.size()-1; i++) {
    tempDot = snake.get(i);

    if(snakeHead.x == tempDot.x && snakeHead.y == tempDot.y) {
      Serial.println("snake collided with itself!");
      leds[mmap[4][3]] = CRGB::Purple;
      leds[mmap[3][2]] = CRGB::Purple;
      leds[mmap[2][2]] = CRGB::Purple;
      leds[mmap[1][3]] = CRGB::Purple;
      leds[mmap[1][4]] = CRGB::Purple;
      leds[mmap[1][5]] = CRGB::Purple;
      leds[mmap[1][6]] = CRGB::Purple;
      leds[mmap[2][7]] = CRGB::Purple;
      leds[mmap[3][7]] = CRGB::Purple;
      leds[mmap[4][6]] = CRGB::Purple;
      leds[mmap[4][5]] = CRGB::Purple;
      leds[mmap[3][5]] = CRGB::Purple;

      leds[mmap[6][2]] = CRGB::Orange;
      leds[mmap[5][3]] = CRGB::Orange;
      leds[mmap[5][4]] = CRGB::Orange;
      leds[mmap[5][5]] = CRGB::Orange;
      leds[mmap[5][6]] = CRGB::Orange;
      leds[mmap[5][7]] = CRGB::Orange;
      leds[mmap[7][3]] = CRGB::Orange;
      leds[mmap[7][4]] = CRGB::Orange;
      leds[mmap[7][5]] = CRGB::Orange;
      leds[mmap[7][6]] = CRGB::Orange;
      leds[mmap[7][7]] = CRGB::Orange;
      leds[mmap[6][5]] = CRGB::Orange;

      leds[mmap[8][2]] = CRGB::Green;
      leds[mmap[8][3]] = CRGB::Green;
      leds[mmap[8][4]] = CRGB::Green;
      leds[mmap[8][5]] = CRGB::Green;
      leds[mmap[8][6]] = CRGB::Green;
      leds[mmap[8][7]] = CRGB::Green;
      leds[mmap[9][3]] = CRGB::Green;
      leds[mmap[10][4]] = CRGB::Green;
      leds[mmap[11][3]] = CRGB::Green;
      leds[mmap[12][2]] = CRGB::Green;
      leds[mmap[12][3]] = CRGB::Green;
      leds[mmap[12][4]] = CRGB::Green;
      leds[mmap[12][5]] = CRGB::Green;
      leds[mmap[12][6]] = CRGB::Green;
      leds[mmap[12][7]] = CRGB::Green;

      leds[mmap[13][2]] = CRGB::Red;
      leds[mmap[14][2]] = CRGB::Red;
      leds[mmap[15][2]] = CRGB::Red;
      leds[mmap[13][3]] = CRGB::Red;
      leds[mmap[13][4]] = CRGB::Red;
      leds[mmap[13][5]] = CRGB::Red;
      leds[mmap[13][6]] = CRGB::Red;
      leds[mmap[13][7]] = CRGB::Red;
      leds[mmap[14][5]] = CRGB::Red;
      leds[mmap[15][5]] = CRGB::Red;
      leds[mmap[14][7]] = CRGB::Red;
      leds[mmap[15][7]] = CRGB::Red;

      leds[mmap[1][10]] = CRGB::Red;
      leds[mmap[1][11]] = CRGB::Red;
      leds[mmap[1][12]] = CRGB::Red;
      leds[mmap[1][13]] = CRGB::Red;
      leds[mmap[4][10]] = CRGB::Red;
      leds[mmap[4][11]] = CRGB::Red;
      leds[mmap[4][12]] = CRGB::Red;
      leds[mmap[4][13]] = CRGB::Red;
      leds[mmap[2][9]] = CRGB::Red;
      leds[mmap[3][9]] = CRGB::Red;
      leds[mmap[2][14]] = CRGB::Red;
      leds[mmap[3][14]] = CRGB::Red;      

      leds[mmap[5][9]] = CRGB::Pink;
      leds[mmap[5][10]] = CRGB::Pink;
      leds[mmap[5][11]] = CRGB::Pink;
      leds[mmap[5][12]] = CRGB::Pink;
      leds[mmap[5][13]] = CRGB::Pink;
      leds[mmap[6][14]] = CRGB::Pink;
      leds[mmap[7][9]] = CRGB::Pink;
      leds[mmap[7][10]] = CRGB::Pink;
      leds[mmap[7][11]] = CRGB::Pink;
      leds[mmap[7][12]] = CRGB::Pink;
      leds[mmap[7][13]] = CRGB::Pink;

      leds[mmap[8][9]] = CRGB::Purple;
      leds[mmap[8][10]] = CRGB::Purple;
      leds[mmap[8][11]] = CRGB::Purple;
      leds[mmap[8][12]] = CRGB::Purple;
      leds[mmap[8][13]] = CRGB::Purple;
      leds[mmap[8][14]] = CRGB::Purple;
      leds[mmap[9][9]] = CRGB::Purple;
      leds[mmap[10][9]] = CRGB::Purple;
      leds[mmap[9][12]] = CRGB::Purple;
      leds[mmap[10][12]] = CRGB::Purple;
      leds[mmap[9][14]] = CRGB::Purple;
      leds[mmap[10][14]] = CRGB::Purple;

      leds[mmap[11][9]] = CRGB::Orange;
      leds[mmap[11][10]] = CRGB::Orange;
      leds[mmap[11][11]] = CRGB::Orange;
      leds[mmap[11][12]] = CRGB::Orange;
      leds[mmap[11][13]] = CRGB::Orange;
      leds[mmap[11][14]] = CRGB::Orange;
      leds[mmap[12][9]] = CRGB::Orange;
      leds[mmap[13][9]] = CRGB::Orange;
      leds[mmap[12][12]] = CRGB::Orange;
      leds[mmap[13][12]] = CRGB::Orange;
      leds[mmap[14][10]] = CRGB::Orange;
      leds[mmap[14][11]] = CRGB::Orange;
      leds[mmap[14][13]] = CRGB::Orange;
      leds[mmap[14][14]] = CRGB::Orange;

      FastLED.show();
      delay(4000);
      resetFunc();
    }
  }
}

int checkFoodCollision() {
  int collisionFound = 0;
  if(food.x == snakeHead.x && food.y == snakeHead.y) {
    if(debug) Serial.println("Collision detected!");
    addRandomDot();
    collisionFound = 1;
  }
  return collisionFound;
}

// adds random dot to screen, except for where user is at
void addRandomDot() {
  do {
    food.x = random(1,15);
    food.y = random(1,15);
  } while (food.x <= 0 || food.y <= 0 || leds[mmap[food.x][food.y]]); // CHECK ENTIRE LL INSTEAD OF LIGHT VALUE ONLY?
  leds[mmap[food.x][food.y]] = CRGB::Yellow;
  FastLED.show();
  if(debug) {
    Serial.print("Dot: (");
    Serial.print(food.x);
    Serial.print(", ");
    Serial.print(food.y);
    Serial.println(")");
  }
}

/* moves food in current direction its going returns 1 if 
 *  successully moved. returns 0 if failed */
int moveUser(int addToBack) {
  // move in direction it currently going
  int wasMoved = 0;
  if(dir == 1) { // move up
    // add next dot to linkedlist
    if(snakeHead.y <= 1) snakeHead.y = 15;
    else snakeHead.y--;
    snake.add(snakeHead);
    leds[mmap[snakeHead.x][snakeHead.y]] = CRGB::Red;
    
    // delete last dot in linkedlist
    if(!addToBack) {
      tempDot = snake.shift();
      leds[mmap[tempDot.x][tempDot.y]] = CRGB::Black;
      wasMoved = 1;
    }
    
  } else if(dir == 2) { // move left
    if(snakeHead.x <= 1) snakeHead.x = 15;
    else snakeHead.x--;
    snake.add(snakeHead);
    leds[mmap[snakeHead.x][snakeHead.y]] = CRGB::Red;

    if(!addToBack) {
      tempDot = snake.shift();
      leds[mmap[tempDot.x][tempDot.y]] = CRGB::Black;
      wasMoved = 1;
    }
    
  } else if(dir == 3) { // move down
    if(snakeHead.y >= 15) snakeHead.y = 1;
    else snakeHead.y++;
    snake.add(snakeHead);
    leds[mmap[snakeHead.x][snakeHead.y]] = CRGB::Red;

    if(!addToBack) {
      tempDot = snake.shift();
      leds[mmap[tempDot.x][tempDot.y]] = CRGB::Black;    
      wasMoved = 1;
    }
    
  } else if(dir == 4) { // move right
    if(snakeHead.x >= 15) snakeHead.x = 1;
    else snakeHead.x++;
    snake.add(snakeHead);
    leds[mmap[snakeHead.x][snakeHead.y]] = CRGB::Red;

    if(!addToBack) {
      tempDot = snake.shift();
      leds[mmap[tempDot.x][tempDot.y]] = CRGB::Black;
      wasMoved = 1;
    }
    
  }
  FastLED.show();
  delay(DOT_SPEED);
  return wasMoved;
}

// checks if user inputted w, a, s, or d
// returns 1 if true, 0 if false
int changeDirection() {
  int wasPressed = 0;
  if(Serial.available() > 0) {
    userInput = Serial.read();

    // check if user pressed w,a,s,d
    if(userInput == 'w' && dir != 3) { // user wants to go up(w)
      dir = 1; 
      wasPressed = 1;
    }
    else if(userInput == 'a' && dir != 4) { // user wants to go left(a)
      dir = 2; 
      wasPressed = 1;
    }
    else if(userInput == 's' && dir != 1) { // user wants to go down(s)
      dir = 3; 
      wasPressed = 1;
    }
    else if(userInput == 'd' && dir != 2) { // user wants to go right(d)
      dir = 4; 
      wasPressed = 1;
    } else if(userInput == 'q') { // user wants to stop. FOR DEBUGGING
      dir = 0; 
      if(debug) {
        Serial.print("User stopped at: (");
        Serial.print(snakeHead.x);
        Serial.print(", ");
        Serial.print(snakeHead.y);
        Serial.println(")");
      }
    } else if(userInput == 'r') {
      Serial.println("Game over!");
      delay(2000);
      resetFunc();
    }
  }
  return wasPressed;
}
