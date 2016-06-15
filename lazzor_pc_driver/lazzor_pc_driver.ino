#define MAX_PROSSES_NUMBER 5
#define LIGHT_INTENSETY 500

#define MIN_LASER_PULS_VALUE 150
#define MAX_LASER_PULS_VALUE 255
#define TICK_TIME 375
#define GLOBAL_DELAY 5


int current_tick_time = millis();
int last_tick_time = 0;



// photocells to messure the laser light

int photocellPinFan = A0;    // the cell and 10K pulldown are connected to a0
int photocellPinCPU = A1; 
int photocellPinRam = A2;   
int photocellPinHDD = A3;   
int photocellPinGra = A4; 
// tmp place holder for photocell messurement
int photocellReading;   
// tmp place holder for the photocell value
int photocellValue;

// relay switches
int switchReleayFan = 4;  // ON with LOW
int switchReleayHDD = 12; // ON with LOW
int switchReleayArduino = 13; // ON with LOW

// LEDs for signals
int ledSignalCPU_1 = 7; // yellow // OUT with LOW
int ledSignalCPU_2 = 8; // yellow // OUT with LOW
int ledSignalRam = 2;   // yellow // OUT with LOW

// all lasers
int laserPowerNetPin = 3;  // Netzteil ( PowerSupply ) is allways on by start up
int laserPowerFanPin = 5;  // Fan
int laserPowerCPUPin = 6;  // CPU
int laserPowerRamPin = 9;  // Ram
int laserPowerHDDPin = 10; // Grafic card 
int laserPowerGraPin = 11; // Hard Drive

// all steps set to false = inactive for start
boolean isFanReached = false;
boolean isCPUReached = false;
boolean isRamReached = false;
boolean isHDDReached = false;
boolean isGraReached = false;

///// // // // // //
boolean all_laser_on = false;
boolean debug = false;
boolean pulseLaser = false;

int current_laser_puls_value = MIN_LASER_PULS_VALUE;
int tick_time = TICK_TIME;

int random_number = 0;


void setup(void) {
  // We'll send debugging information via the Serial monitor
  if(debug){ 
    Serial.begin(1200);  
  }

  pinMode( switchReleayFan, OUTPUT );
  pinMode( switchReleayHDD , OUTPUT );
  pinMode(switchReleayArduino  , OUTPUT );
  
  pinMode(ledSignalCPU_1,OUTPUT ); 
  pinMode(ledSignalCPU_2,OUTPUT ); 
  pinMode(ledSignalRam,  OUTPUT ); 
   
  pinMode(laserPowerNetPin , OUTPUT );
  pinMode(laserPowerFanPin , OUTPUT );
  pinMode(laserPowerCPUPin , OUTPUT );
  pinMode(laserPowerRamPin , OUTPUT );
  pinMode(laserPowerHDDPin , OUTPUT );
  pinMode(laserPowerGraPin , OUTPUT );  

  digitalWrite( ledSignalCPU_1 , LOW ); 
  digitalWrite( ledSignalCPU_2 , LOW ); 
  digitalWrite( ledSignalRam , LOW ); 
  
  digitalWrite( switchReleayFan , LOW );   
  digitalWrite( switchReleayHDD , LOW ); 
  digitalWrite( switchReleayArduino , LOW ); 
  
  // STARTUP CHECK
   check_all_lasers();
   check_relay_connection();   
   
  // For maintanance 
   check_photocell_with_laser(); 
}

boolean randTrueFalse(){
  
  random_number = random();
  
  if(random_number <= 0.5){
   return true;     
  }else{    
   return false;   
  }   
}

void print_debug(){   
  
    Serial.print(" Analog  Photore. = ");
    Serial.println(photocellReading);   
    Serial.print(" isFanReached. = ");
    Serial.println(isFanReached);   
    Serial.print(" isCPUReached  = ");
    Serial.println(isCPUReached );  
    Serial.print(" isRamReached  = ");
    Serial.println(isRamReached ); 
    Serial.print(" isHDDReached  = ");
    Serial.println(isHDDReached ); 
    Serial.print(" isGraReached  = ");
    Serial.println(isGraReached ); 


}

// first
boolean check_input(int photocellReadPin,  int laserPower){     
   
  analogWrite(laserPower, 255);
  delay(15); // time to messure
  photocellValue = analogRead( photocellReadPin ); 
   //
  if (photocellValue < LIGHT_INTENSETY ){ 
    return true;              
  } 
   // to make after glow     
  analogWrite(laserPower, 150);
  delay(20); 
  analogWrite(laserPower, 120);
  delay(20); 
  analogWrite(laserPower, 100);
  delay(10); 
  analogWrite(laserPower, 70);
  delay(5); 
         
  return false;    
}

void set_all_laser_on(){
   
  digitalWrite( laserPowerNetPin , HIGH ); 
  digitalWrite( laserPowerFanPin , HIGH );   
  digitalWrite( laserPowerCPUPin , HIGH );
  digitalWrite( laserPowerRamPin , HIGH ); 
  digitalWrite( laserPowerHDDPin , HIGH );   
  digitalWrite( laserPowerGraPin , HIGH );   
  
}
// the main loop
void loop(void) {

       current_tick_time = millis();   
        
       if( ( current_tick_time - last_tick_time ) >= tick_time ){  
           
          check_and_set_all_components();
      
          last_tick_time = current_tick_time;   
      }
    
      if(debug){print_debug();}   
      
      delay( GLOBAL_DELAY );      
}        

// check if the LIGHT_INTENSETY of each diode 
// if the current light intensetie is <= LIGHT_INTENSETY
// active component and go to the next step
void  check_and_set_all_components(){

  // allways active the laser from power supplie 
  digitalWrite( laserPowerNetPin , HIGH );     
  
  // if FAN photocell is not reached check it and if the photocell is hit actiate next laser
  
  // Cheack Fan ///////////////////////////////////////////////////////// 
  if(isFanReached == false){
    
         digitalWrite( laserPowerFanPin , LOW); 
         digitalWrite( switchReleayFan , HIGH );    
        // check photocell        
        
       isFanReached = check_input(photocellPinFan, laserPowerNetPin);           
      }
  else{  
      digitalWrite( laserPowerFanPin , HIGH);
      digitalWrite( switchReleayFan , LOW );
    }    
   // Cheack CPU /////////////////////////////////////////////////////////     
  if(isCPUReached == false){  
      digitalWrite( laserPowerCPUPin , LOW );
      digitalWrite( ledSignalCPU_1 , HIGH ); 
      digitalWrite( ledSignalCPU_2 , HIGH );      
    
    // check onlay if the previous is active
     if(isFanReached){
      isCPUReached = check_input( photocellPinCPU , laserPowerFanPin); 
     }        
    }
  else {     
      digitalWrite( laserPowerCPUPin , HIGH ); 
      randomCPUGlow();
    }     
  // Check RAM /////////////////////////////////////////////////////////
  if(isRamReached == false){
      digitalWrite( laserPowerRamPin , LOW );  
      digitalWrite( ledSignalRam , HIGH );     
      
     // check onlay if the previous is active
     if( isCPUReached ){
      isRamReached =  check_input(photocellPinRam , laserPowerCPUPin);
     }      
  }
  else if( isCPUReached ){       
      digitalWrite( laserPowerRamPin , HIGH ); 
      randomRamGlow();       
  }         
  
  // Cheack HDD /////////////////////////////////////////////////////////
  if(isHDDReached == false){    
       digitalWrite( switchReleayHDD , HIGH );  
       digitalWrite( laserPowerHDDPin , LOW );
      // check onlay if the previous is active
      if( isRamReached ){
       isHDDReached =  check_input( photocellPinHDD  , laserPowerRamPin);    
      }
    }
  else {
        digitalWrite( switchReleayHDD ,LOW ); 
        digitalWrite( laserPowerHDDPin ,HIGH  );       
      }     
 // Cheack grafic card //////////////////////////////////////////////////
  if(isGraReached == false){
       digitalWrite( laserPowerGraPin , LOW );
       digitalWrite(  switchReleayArduino  , HIGH ); 
       
       // check onlay if the previous is active
       if( isHDDReached && false ==  isGraReached ){
         isGraReached = check_input(  photocellPinGra  , laserPowerHDDPin);
       }      
     } 
  else {    
       digitalWrite( laserPowerGraPin , HIGH );
       digitalWrite(  switchReleayArduino  , LOW );
     }   
     
     
    //final state if thee grafic card is reached active it as the final state
        if( isGraReached ){
           analogWrite(laserPowerHDDPin, 150);
          delay(40); 
          analogWrite(laserPowerHDDPin, 120);
          delay(30); 
          analogWrite(laserPowerHDDPin, 100);
          delay(25); 
          analogWrite(laserPowerHDDPin, 80);
          delay(5); 
       } 
       
}

// make the CPU-LEDs lights blink random
void randomCPUGlow(){
    
      for(int i = 0; i < 20;i++){
      if(randTrueFalse())
        {
          digitalWrite( ledSignalCPU_1 , LOW); 
          digitalWrite( ledSignalCPU_2 , HIGH);
        }else{
          digitalWrite( ledSignalCPU_1 , HIGH); 
          digitalWrite( ledSignalCPU_2 , LOW); 
        }
       delay(8); 
      }
      
      digitalWrite( ledSignalCPU_2 , HIGH);     
      digitalWrite( ledSignalCPU_1 , HIGH);   
      
}

// make the Ram-LED glow randomly
void randomRamGlow(){
        for(int i = 0; i < 10;i++){
      if(randTrueFalse())
        {
          digitalWrite( ledSignalRam , HIGH ); 
        }else{    
          digitalWrite( ledSignalRam , LOW); 
        }
       delay(8); 
      }
  
   digitalWrite( ledSignalRam , LOW); 
}

// the following function are to check all components 
// for the first start

void check_all_lasers(){
   delay( 500);
  digitalWrite( laserPowerNetPin , HIGH );
   delay( 500);
  digitalWrite(  laserPowerFanPin, HIGH );
   delay( 500);
  digitalWrite( laserPowerCPUPin , HIGH );
   delay( 500);
  digitalWrite( laserPowerRamPin , HIGH );
   delay( 500);
  digitalWrite( laserPowerHDDPin , HIGH );
   delay( 500);
  digitalWrite( laserPowerGraPin , HIGH );   
}

// just check the lights on the releay 
// or hear the sound
void check_relay_connection(){
   // turn second arduino ON and after 1 second OFF again
   digitalWrite(  switchReleayArduino  , LOW );
   delay( 1000);
   digitalWrite(  switchReleayArduino  , HIGH );
   
   digitalWrite(  switchReleayFan  , LOW );
   delay( 1000);
   digitalWrite(  switchReleayFan  , HIGH );
   
   digitalWrite(  switchReleayHDD  , LOW );
   delay( 1000);
   digitalWrite(  switchReleayHDD  , HIGH );
}


// just to check if all things work properbly
// check each photodiode 
// The user has to direct the laser to each diode by hand


void check_photocell_with_laser(){
 
  Serial.print("Starup check");   
  delay( 1000);
  
  if( check_input(photocellPinFan ,laserPowerNetPin ) == false){
     Serial.println(" ERROR photocell Fan"); 
  }
  
   delay( 1000);   
  if( check_input(photocellPinCPU ,  laserPowerCPUPin ) == false){
     Serial.println(" ERROR photocell CPU"); 
  }

   delay( 1000);   
  if( check_input(photocellPinRam , laserPowerRamPin) == false){
     Serial.println(" ERROR photocell Ram"); 
  }  

   delay( 1000);        
  if( check_input(photocellPinHDD , laserPowerHDDPin ) == false){
     Serial.println(" ERROR photocell HDD"); 
  }  
  
  delay( 1000);  
   if( check_input(photocellPinHDD , laserPowerGraPin) == false){
     Serial.println(" ERROR photocell HDD"); 
  }  
  
  
  
}



