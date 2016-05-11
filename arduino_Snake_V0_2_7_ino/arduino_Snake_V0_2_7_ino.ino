// NOT TESTED
// Arduino Snake 0.2.6
// CBM Project by the Universaty of applied Science Mannheim


#include "FastLED.h"
#include "Sides.h"
#include <NewPing.h>

#define NUM_LEDS_PER_STRIP 40
#define NUM_STRIPS 2
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

#define DISTANCE_TO_SENSOR 30 // cm

// data pin for the led-stripe
#define DATA_PIN_LED_1 3
#define DATA_PIN_LED_2 5

// should be 2dimensionsl 
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

// global settings
int start_time = millis(); // !  This number will overflow (go back to zero), after approximately 50 days.
int global_delay         = 10;  
int current_tick_time    = 0;
int last_tick_time       = 0;
int game_tick_intervall  = 50; // millisec. to wait in each tick

// debug mode
boolean debug_mode = false;

// play with borders ?
boolean play_snake_with_borders = false;

// postion x means positon on the led-stripe
int position_x = 0;
// pitison y means wich led-stripe
int position_y = 0;



// postion x means positon on the led-stripe
int tail_position_x[NUM_LEDS_PER_STRIP];
// pitison y means wich led-stripe
int tail_position_y[NUM_STRIPS];

// temp. only to save the last postione due the iteration
int lastPosition_x; 
int lastPosition_y; 
 

// speed in  number of filds per tick
int snake_speed = 1 ; //

// apple position
int position_apple_x = -1;
int position_apple_y = -1;

// points
int snake_length = 1;

// snake direction start time
Side current_direction = LEFT;


boolean go_top  = false;
boolean go_down = false;
boolean go_left = false;
boolean go_right= false;


//set pins for sensors
// LEFT
int pin_trigger_1=7;
int pin_echo_1 =6;
// RIGHT
int pin_trigger_2 = 8;
int pin_echo_2 = 9;
// TOP
int pin_trigger_3 = 0;
int pin_echo_3 = 0;
// DOWN
int pin_trigger_4 = 0;
int pin_echo_4 = 0;

// set time and distance for sensors
int run_time_1=-1;
int distance_1=-1;

int run_time_2=-1;
int distance_2=-1;

int run_time_3=-1;
int distance_3=-1;

int run_time_4=-1;
int distance_4=-1;



//digitalWrite(pin,value) HIGH
//analogWrite(pin,value)LOW

// set sonar sensors
NewPing sonar_left(pin_trigger_1, pin_echo_1, DISTANCE_TO_SENSOR);
NewPing sonar_right(pin_trigger_2, pin_echo_2, DISTANCE_TO_SENSOR);
NewPing sonar_top(pin_trigger_3, pin_echo_3, DISTANCE_TO_SENSOR);
NewPing sonar_down(pin_trigger_4, pin_echo_4, DISTANCE_TO_SENSOR);


void setup() {
  
  if( debug_mode ){
      Serial.begin (9600); // bps rates: 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200
  }
  
  // Load all strips and set number of LEDs
  for( int i = 0; i < NUM_STRIPS;i++){    
      FastLED.addLeds<NEOPIXEL, DATA_PIN_LED_1>(leds[i], NUM_LEDS_PER_STRIP);
  }
  
}

//  check if a "button" was used 
// button in this case is an sonic-sensor distance measurement
void set_move_to_side( float distance, Side side){
    
    switch ( side ) {
      
      case TOP: 
          if(distance <= DISTANCE_TO_SENSOR && distance > -1 ) {              
              go_top = true;
          }else{
              go_top = false;           
          }
          break;
      case DOWN: 
          if(distance <= DISTANCE_TO_SENSOR && distance > -1 ) {
              go_down = true; 
          }else{
              go_down = false;
          }
          break;
      case LEFT:
      
          if(distance <= DISTANCE_TO_SENSOR && distance > -1 ){            
              go_left = true;      
           }else{
              go_left = false;  
           }   
           break;        
      case RIGHT:
      
           if(distance <= DISTANCE_TO_SENSOR && distance > -1 ){              
              go_right = true;   
           }else{
              go_right = false;  
           }  
           break;        
      default:     
          
        break;      
    }    
 }
 
void set_LED_by_mode(String mode, int x, int y){          
    
      if(mode == "GREEN_GRASS"){
            leds[y][x].r = 0;
            leds[y][x].g = 10 +random(0,10);
            leds[y][x].b = random(0,5);
            leds[y][x].fadeLightBy(65);          
      }  
}
 
 void show_output_with_leds(){    
  // first clear "screen"
  // NEED TO TEST
  // leds[position_x] = CRGB::BLACK;
  for(int y = 0 ; y< NUM_STRIPS ; y++){
      for(int x = 0 ; x< NUM_LEDS_PER_STRIP ; x++){ 
          //leds[k][i] = CRGB::Black; 
         set_LED_by_mode("GREEN_GRASS", x, y);   
    }
   }
    delay(5);
   // go only to one side if no other sensor is triggerd 
// PRINT SANKE
   // shoud be 2dimensional, but first test it with one !
         
   leds[position_y][position_x].r = 250;
   leds[position_y][position_x].g = 5;
   leds[position_y][position_x].b = 15;     


// to get the tail, not tested.... 
   for(int i = 0; i < snake_length; i ++){       
     
     leds[tail_position_x[i]][tail_position_y[i]] = CRGB::Green; 
   }



// SHWOW APPLE
    leds[position_apple_y][position_apple_x] = CRGB::Red; 
   
   FastLED.show();  
}

// to shift the tail of the snake and set the new postion of the head
void shift_snake(int head_position_x, int head_position_y){
  
   int max_x = sizeof(tail_position_x);
   int max_y = sizeof(tail_position_y);
   
   // snake_length is max in size becouse its not dynamic -> use of current fill-size
   for( int k = 0; k < snake_length ;k++){
     // 
     if( k+1 <  max_x ){
      tail_position_x[k+1] = tail_position_x[k];
     }
     
     if( k+1 <  max_y ){
      tail_position_y[k+1] = tail_position_y[k];
     }
   }
   
   tail_position_x[0] = head_position_x;
   tail_position_y[0] = head_position_y;
  
}

/*
set the postion and check if borders reached
*/

void set_position(){    
 
 
  
  if( current_direction == LEFT  ){ position_x-= snake_speed; }
  if( current_direction == RIGHT ){ position_x+= snake_speed; }
  if( current_direction == TOP   ){ position_y-= snake_speed; }
  if( current_direction == DOWN  ){ position_y+= snake_speed; }
   
   
   shift_snake(position_x, position_y);
   
   
   /*
  if( go_left  && !go_right && !go_top && !go_down ) {    
    position_x--;       
  }  
  if( go_right && !go_left && !go_top && !go_down ) {    
     position_x++;      
  }  
 
  if( go_top && !go_down &&  !go_right && !go_left ) {
     position_y++;    
  }  
  if(go_down && !go_top  &&  !go_right && ! go_left ) {
     position_y--;
  } 
  */
  // just to prevent index out of bound
  // dont jump out the box  
  
  if( !play_snake_with_borders ) {
    
      position_x %= NUM_LEDS_PER_STRIP;
      position_y %= NUM_STRIPS; 
      
      
  }
}

// FOR TWO SENSOR INPUTS 
// from the point of view of the snake itself :)
void set_direction(){

  // if you "push" right then it will go right from the current "face" direction
  if( go_right ) {
    if( current_direction == LEFT ) { // check the current direction
      
        current_direction = TOP; // set the new direction 
    }
    if( current_direction == RIGHT ) {
      
        current_direction = DOWN;      
    }
    if( current_direction == TOP ) {
      
        current_direction = RIGHT;      
    }
    if( current_direction == DOWN ) {
     
        current_direction = LEFT; 
    }
  }
  
  if( go_left ){
     if( current_direction == LEFT ) {
       
        current_direction = DOWN; 
    }
    if( current_direction == RIGHT ) {
      
        current_direction = TOP;      
    }
    if( current_direction == TOP ) {
      
        current_direction = RIGHT;  
    }  
    if( current_direction == DOWN ) {
    
        current_direction = LEFT;
    }  
  }
}
void set_new_random_apple_positon() {
  position_apple_x = random( 0 , NUM_LEDS_PER_STRIP );
  position_apple_y = random( 0 , NUM_STRIPS );
  
 }
 
void check_if_apple_is_reached() {
      if(position_x == position_apple_x && position_y == position_apple_y ){
        snake_length++; 
        
        set_new_random_apple_positon();
      }
}

void snake_control(){
   current_tick_time = millis();
   
   if( ( current_tick_time - last_tick_time ) >= game_tick_intervall ) {
          
     check_if_apple_is_reached();
     set_direction();
     set_position();      
     
     last_tick_time = millis();     
   }
}

void loop() {
   //INPUT     
    // First Sonic-Module left side    
    distance_1 = sonar_left.ping();
      
    // Second Sonic-Module right side    
    distance_2 = sonar_right.ping();    
          
    // check if one of the sensores are useed directen <= MAX_DISTANCE
    set_move_to_side(distance_1, LEFT);
    set_move_to_side(distance_2, RIGHT);
    set_move_to_side(distance_3, TOP);
    set_move_to_side(distance_4, DOWN);
        
  //CONTROL
    snake_control();
  //OUTPUT
    show_output_with_leds();  
 
    delay(global_delay);    
  
  if( debug_mode ){
   Serial.print("Distance 2: ");
   Serial.println(distance_2 );
   Serial.print("Distance 1: ");
   Serial.println(distance_1 );
  
   Serial.print(" Go Left: ");
   Serial.println(go_left);
   Serial.print(" Go Right: ");
   Serial.println(go_right); 
   
   Serial.print(" Position : ");
   Serial.println(position_x);   
  }
}
