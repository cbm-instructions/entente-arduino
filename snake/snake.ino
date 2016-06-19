// Arduino Snake 1.0.0
// CBM Project by the Universaty of applied Science Mannheim
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "JoystickLib.h"
#include "Side.h"
#include <NewPing.h>

#define NUM_MATRIX_X 2
#define NUM_MATRIX_Y 1

// if you have an 8 of 8 pixel board
#define NUM_LEDS_X (NUM_MATRIX_X * 8)
#define NUM_LEDS_Y (NUM_MATRIX_Y * 8)

// all leds you have
#define NUM_LEDS (NUM_LEDS_X * NUM_LEDS_Y)

#define MATRIX_DATA_PIN 6
#define JOYSTICK_SELECT_BUTTON_PIN 7

#define JOYSTICK_X_AXIS A0
#define JOYSTICK_Y_AXIS A1

#define NUMBER_OF_INPUTS_FOR_SAVE 3 

#define SNAKE_START_LENGTH 3
#define SNAKE_START_X 4
#define SNAKE_START_Y 4

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8,2,1,MATRIX_DATA_PIN,
NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
NEO_GRB            + NEO_KHZ800);


// text for output, we save the Strings on the "hard-drive" to make things easier 
const char introMessage[]      ={"<SNAKE D> PRESS START TO CONTINUE IF YOU LIKE TO PLAY"};
const char gameOverMessage[]  ={"GAME OVER YOUR SCORE"};
char gameOverScoreConcatenation[60];

const uint16_t colors[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
 

int color_red  = 0;
int color_green= 0;
int color_blue = 0;

// snake direction start time
Side  current_direction = EAST;
Side  last_direction = EAST;
Side  saveLastInputs[NUMBER_OF_INPUTS_FOR_SAVE] ;
Side  joystick_input;
// x  and y axis
Joystick stick(JOYSTICK_X_AXIS ,JOYSTICK_Y_AXIS);
// just to set start value
int select_button_state = 0;

// global settings
// millis(); // !  This number will overflow (go back to zero), after approximately 50 days.
int global_delay         = 30;  
int current_tick_time    = 0;
int last_tick_time       = 0;
int game_tick_intervall  = 240; // millisec. to wait each tick
int last_matrix_UPdate_time = 0;

boolean isGameStart = false;
// debug mode
boolean debug_mode = false;
// input debug only
boolean debug_input = false;
// show intro
boolean isIntro     = true;

// postion x of the sanke on the board
int position_x =  SNAKE_START_X;
// pitison y of the sanke on the board
int position_y =  SNAKE_START_Y;
// temp. only to save the last postione due the iteration
int lastPosition_x; 
int lastPosition_y; 

int tail_position_x[NUM_LEDS];
int tail_position_y[NUM_LEDS];

// speed in  number of filds per tick
int snake_speed = 1 ; //
// length on screen and points for score
int snake_length = SNAKE_START_LENGTH;
// apple position
int position_apple_x = 4;
int position_apple_y = 4;

// temp varibales
int totalShifts = 0;
int count_matrix_text_iteration = 0;
int color_puls_range = 0;
boolean isColorPulsHigh = false;
boolean isColorPulsLow = false;
boolean isRainbowMode = false;
boolean isJoystickCenterPosition = false;

// for print text to matrix

int matrixCursureStartX = matrix.width();

boolean introPlayed = false;
boolean gameEndFlashSnake = false;

// stuff for method pirnt_all()
// only for rainbow_mode ;)
int color_stepper = 0;
int color_step_width = 4;
uint32_t  tmp_color ;

// you cant see more then 30 frames per sekonds 
// just reduce it a bit 
 int last_print_time = millis();
 int print_tick_intervall = 80;
 
 // just to try to set the apple the right way
 
  int max_free_place_searche = 20;
 
  boolean is_new_apple_position = true;
  

void setup() {

 if( debug_mode || debug_input ){
    Serial.begin (9600); // bps rates: 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200
  }

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(colors[0]);

  // Calibrate the joystick. For this to be effective, the joystick should
  // be resting at its center position.
  stick.calibrate();

  //activate pull-UP resistor on the push-button pin
  pinMode(JOYSTICK_SELECT_BUTTON_PIN,INPUT_PULLUP);
  
  reset_Tail();

} 

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


// the print function 
void print_all(){     
  
    // just wait a bit until next print ( change color and take the leds on )
    if( ( current_tick_time - last_print_time ) >= print_tick_intervall ) {    
  
        // first clear "screen" so all is black , then we print new pixel         
        matrix.fillScreen(0);   
      
        // SHWOW APPLE  -> feat the snake , is there still a vegetarien snake !? 
        matrix.drawPixel( position_apple_x , position_apple_y, 0xF800);    
      
        // to get the tail
        for(int i = 0; i < snake_length; i ++){ 
             
          //make the tail shine like a rainbow, its also called the nyan mode
          // but thats outdatet so we use termini rainbows instead
          if(isRainbowMode ){  
            // use the color circel, is we wont a hard color change -> high color_step_width
            // else a low one
            color_stepper+=color_step_width ;   
            color_stepper %=255;// we use RGB so we use only 255 numbers :)
                
            tmp_color = Wheel(color_stepper); 
        
          }else{       
            // just normal yellow
            tmp_color = matrix.Color(255,255,10);
          }    
          // set the new pixel 
          matrix.drawPixel(tail_position_x[i], tail_position_y[i], tmp_color );      
        }
        // PRINT SANKE 
        matrix.drawPixel( position_x, position_y, matrix.Color(50,205,50)); 
      
        matrix.show();
        
       last_print_time = millis();  
    }
}

// just to save the last number of inputs, for speacial functions ;)
void save_input(Side newState){  
  
   if( isJoystickCenterPosition == false ){
     
        for(int k = NUMBER_OF_INPUTS_FOR_SAVE -1; k > 0;k--){
      
          saveLastInputs[k] =   saveLastInputs[k-1];   
      
        }  
        saveLastInputs[0] = newState;
     isJoystickCenterPosition = true;
  }
  
  if(joystick_input != NONE_DIRECTION){
    isJoystickCenterPosition = false;    
  }
}

void check_saved_input(){
   

    if( saveLastInputs[0] == WEST && saveLastInputs[1] == SOUTH && saveLastInputs[2] == EAST){
      isRainbowMode = true;
    } 
    
    if( saveLastInputs[0] == NORTH &&  saveLastInputs[1] == NORTH && saveLastInputs[2] == SOUTH){    
        isRainbowMode = false;
    }
      
}
/*
set the postion and check if borders reached
 */
void set_position(){   

  if( current_direction == WEST  ){
    position_x-= snake_speed;
  }
  if( current_direction == EAST ){
    position_x+= snake_speed;
  }
  if( current_direction == NORTH  ){  
    position_y-= snake_speed;  
  }
  if( current_direction == SOUTH ){
    position_y+= snake_speed;  
  }   


  // just to prevent index out of bound
  // dont jump out the box  

  position_x %= (NUM_LEDS_X);
  position_y %= (NUM_LEDS_Y);  

  if(position_x < 0){
    position_x = NUM_LEDS_X-1;
  }
  if(position_y < 0){
    position_y = NUM_LEDS_Y-1;
  }

  shift_snake(position_x, position_y);

}


// to shift the tail of the snake and set the new postion of the head
void shift_snake(int head_position_x, int head_position_y){

  // snake_length is max in size becouse its not dynamic -> use of current fill-size
  for( int k =  NUM_LEDS-1; k > 0;k--){     //  
    tail_position_x[k] = tail_position_x[k-1];   
    tail_position_y[k] = tail_position_y[k-1];  
  }      

  head_position_x %= (NUM_LEDS_X);
  head_position_y %= (NUM_LEDS_Y);  

  tail_position_x[0] = head_position_x; 
  tail_position_y[0] = head_position_y;

}
// FOR TWO SENSOR INPUTS 
// from the point of view of the snake itself :)
void set_direction(){ 

  
  if( joystick_input == WEST && current_direction != EAST){
    last_direction = current_direction;
    current_direction = joystick_input;
  }
  else if( joystick_input == EAST && current_direction != WEST){
    last_direction = current_direction;
    current_direction = joystick_input;
  }
  else if( joystick_input == NORTH   && current_direction != SOUTH){
    last_direction = current_direction;
    current_direction = joystick_input;
  }
  else if( joystick_input == SOUTH && current_direction != NORTH ){
    last_direction = current_direction;
    current_direction = joystick_input;
  }

  else if( joystick_input == NORTH_WEST ){  
    if(  current_direction  == WEST ||  current_direction  == EAST){
      last_direction = current_direction;
      current_direction = NORTH;     
    }
    else  if( current_direction == NORTH || current_direction == SOUTH){
      last_direction = current_direction;
      current_direction = WEST;  
      joystick_input = NONE_DIRECTION;
    }
  }  
  else if( joystick_input == NORTH_EAST){    
    if(  current_direction  == WEST ||  current_direction  == EAST){
      last_direction = current_direction;
      current_direction = NORTH;             
    }
    else if( current_direction == NORTH || current_direction == SOUTH){
      last_direction = current_direction;
      current_direction = EAST;        
    }
  }      
  else  if( joystick_input == SOUTH_EAST){    
    if(  current_direction  == WEST ||  current_direction == EAST){
      last_direction = current_direction;
      current_direction = SOUTH;        
    }
    else  if( current_direction == NORTH || current_direction == SOUTH){
      last_direction = current_direction;
      current_direction = EAST;      
    }    
  }
  else if( joystick_input == SOUTH_WEST){    
    if(  current_direction  == WEST ||  current_direction  == EAST){
      last_direction = current_direction;
      current_direction = SOUTH;
    }
    else if( current_direction == NORTH || current_direction == SOUTH){
      last_direction = current_direction;
      current_direction = WEST;   
    }    
  }  

}
// the tail pixel need to set all to -1 -> else there are 1 and visible :)
void reset_Tail(){
    for( int k = 0; k <  NUM_LEDS-1;k++){     //  

    tail_position_x[k] = -1;   
    tail_position_y[k] = -1;  

  }     
  
}

// sets the apple for the snake to eat... poor snake only apples as source to grow
void set_new_random_apple_positon() {    
  
  // try to set random and fi  max_free_place_searche is reached, just set the apple 
  // behinde snake
  
  for(int i = 0;i < max_free_place_searche;i++){
    
      position_apple_x = random( 0 , NUM_LEDS_X );
      position_apple_y = random( 0 , NUM_LEDS_Y );
    
      is_new_apple_position = true;  
    
      for( int k =  1; k < snake_length;k++){     //   
        if( position_apple_x == tail_position_x[k] &&   tail_position_y[k] == position_apple_y){     
          is_new_apple_position = false;      
        }         
      } 
  
  
      if(is_new_apple_position){break;}
      
      if(i  ==  ( max_free_place_searche -1)){
       
          position_apple_x = tail_position_x[snake_length-1];
          position_apple_y = tail_position_y[snake_length-1];
       
      } 
    
  }

  
  
}

// just checks if the head of the snake (default green) has got an apple
// snake grows each time and the apple disapears and a new one appears :)
void check_if_apple_is_reached() { 

  if(position_x == position_apple_x && position_y == position_apple_y ){

    if(debug_mode){   
      Serial.println(F("GOT AN APPLE"));
    }        
    // Check if snake fill the whole matrix, if so snake dont grow larger :)
    if(snake_length < NUM_LEDS_X * NUM_LEDS_Y -1){ 
      snake_length++; 
    }

    set_new_random_apple_positon();
  }
}


// just listen to the input
// if is an other input just replace the stick with the other input
// or make another one similar to this. 
// the enum from Side.h are used so you could use anything

void get_input(){

  
  // select button
  select_button_state = digitalRead(JOYSTICK_SELECT_BUTTON_PIN);

  // Figure out what direction the joystick is in and print it.
  if (stick.isUp()) {
    if( debug_input ){
      Serial.println(F("JoyStick-Up"));
    }              
    joystick_input = NORTH;
  }
  if (stick.isRight() ) {
    if( debug_input  ){
      Serial.println(F("JoyStick-Right"));
    }            
    joystick_input = EAST;
  }

  if (stick.isDown()) {
    if( debug_input  ){  
      Serial.println(F("JoyStick-Down"));
    }          
    joystick_input = SOUTH;
  }

  if (stick.isLeft()) {
    if( debug_input ){
      Serial.println(F("JoyStick-Left"));
    }      
    joystick_input = WEST;
  }  

  if (stick.isRightUp()) {  
    if( debug_input  ){
      Serial.println(F("JoyStick-Right_UP"));
    }            
    joystick_input = NORTH_EAST;        
  }

  if (stick.isRightDown()) {
    if( debug_input  ){
      Serial.println(F("JoyStick-Right_DOWN"));
    }            
    joystick_input = SOUTH_EAST;
  }

  if (stick.isLeftDown()) {
    if( debug_input  ){
      Serial.println(F("JoyStick-Left_DOWN"));
    }            
    joystick_input = SOUTH_WEST;
  }

  if (stick.isLeftUp()) {
    if(debug_input  ){
      Serial.println(F("JoyStick-Left_UP"));
    }            
    joystick_input = NORTH_WEST;
  }  

}
// if poor snake is starting eating itself, cannibalism is forbidden, so the game ends ...
// ... press start to continue 
boolean isSnakeDead(){     
  // snake_length is max in size becouse its not dynamic -> use of current fill-size
  for( int k =  1; k < snake_length;k++){     //   
    if( position_x == tail_position_x[k] &&   tail_position_y[k] == position_y){     
      return true;        
    }         
  }   
  return false;    
}

// her ist the "game-logic" 
void snake_control(){
     
  // to garante that the game will run in the same speed we set time ticks ... last thing ist for the select button to pause the game ;)     
   
  
  if( ( current_tick_time - last_tick_time ) >= game_tick_intervall ) {    

    
    save_input(joystick_input);
    check_saved_input();
    
    check_if_apple_is_reached();       
    set_direction();
    set_position();      
    increase_snake_speed();
    last_tick_time = millis();     
  }
}

// to make the screen appears a bit fancy or so
void intro() {

  if(current_tick_time < 5000){   

    matrix.drawPixel( random( 0 , NUM_LEDS_X ), random( 0 , NUM_LEDS_Y ) ,  matrix.Color( random(170,255), random(0,55) ,random(0, 55)));
    matrix.show();
    
  }
  else if(current_tick_time < 6000){  
    matrix.fillScreen(matrix.Color( 0,0,255 ));
    matrix.show();   
    delay(10);
    flash_matrix(400);
  } else{
    isIntro = false;       
  } 
} 

// hear a text could be printed on the matrix
void print_message(String message ){
       
  if((current_tick_time - last_matrix_UPdate_time) >= 60){
      matrix.fillScreen(0);
      totalShifts = (message.length() *6);

     
      matrix.setCursor(matrixCursureStartX , 0);

      matrix.print((message));
    
      if(--matrixCursureStartX < -totalShifts) {
        matrixCursureStartX = matrix.width();          
        
      }
     //count_matrix_text_iteration++;
      
     // puls_matrix_text();
      
      matrix.show();        
     
      last_matrix_UPdate_time = current_tick_time;
  }
  
}

void puls_matrix_text(){  

        if(count_matrix_text_iteration % 2 == 0){
          
           if(color_puls_range  < 150 && isColorPulsHigh == false){
                 color_puls_range++; }
           else{
                 isColorPulsHigh = true;
                 isColorPulsLow = false;
           }
           
           if(color_puls_range > -150 && isColorPulsLow == false){
                 color_puls_range--;
           }else{
                isColorPulsHigh = false;
                 isColorPulsLow = true;           
             
           }       
           

           matrix.setTextColor(matrix.Color(color_puls_range,color_puls_range,100));
   
        }
  
}

void flash_matrix(int time_to_wait_until_finished){

  matrix.setBrightness(90);
  matrix.show();
  delay(100);
  matrix.setBrightness(75);
  matrix.show();
  matrix.setBrightness(30);
  matrix.show();
  delay(100);
  matrix.setBrightness(5);  
  matrix.show();
  delay(time_to_wait_until_finished); 
}

void gameOver(){
  
  if(gameEndFlashSnake == false){

    flash_matrix(1500);

    gameEndFlashSnake = true;

  }
  else { 
              
      sprintf(gameOverScoreConcatenation,"%s:%i",gameOverMessage,snake_length);
       matrix.fillScreen(matrix.Color( 0,0,255 )); 
      print_message(gameOverScoreConcatenation);   
  }
}

void print_debug_data(){
  
    Serial.print("Last Direction"),
    Serial.println(last_direction);
    Serial.print("Current Direction");
    Serial.println(current_direction);
    Serial.print("JoyStick Input:");
    Serial.println(joystick_input);

    Serial.print(" Position X: ");
    Serial.println(position_x);   
    Serial.print(" Position Y: ");
    Serial.println(position_y);   
}

void restart(){
  
 snake_length = SNAKE_START_LENGTH; 
 position_x =  SNAKE_START_X;
 position_y =  SNAKE_START_Y;
 reset_Tail();

 current_direction = EAST;
 last_direction    = EAST;
 joystick_input    = NONE_DIRECTION;
 
 last_tick_time    = current_tick_time;
 gameEndFlashSnake = false;
 
 game_tick_intervall = 250;
 
}

void increase_snake_speed(){
  
  if(snake_length == 10){
     game_tick_intervall = 180;  
  }
  
 if(snake_length == 15){
  game_tick_intervall = 150;  
  }
 if(snake_length == 20){
  game_tick_intervall = 100;  
  }
 if(snake_length >= 30){
  game_tick_intervall = 70;  
  }
}


void loop() {

  current_tick_time = millis();
  //INPUT         
  stick.loop();
  // Get all Joystick inUPut
  get_input();  

        if(isIntro == true){          
          intro();    
        }
        else{                                
             if(select_button_state == 0){
                 isGameStart = true; 
                 matrixCursureStartX = matrix.width();
             }
                
             
            if(isGameStart){          
                // GAME OVER CHECK
                if(isSnakeDead()){
                  gameOver();
                  if(select_button_state == 0){restart();}
                }  
                else{             
                    //CONTROL
                    snake_control();
                    //OUTPUT
                    print_all();         
                }                
            }else{
               matrix.fillScreen(matrix.Color( 0,0,255 ));
               print_message(introMessage);    
            }
        }
        
  if( debug_mode ){

     print_debug_data();
  }

  delay(global_delay); 

}
