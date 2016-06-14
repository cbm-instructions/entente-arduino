#define MAX_PROSSES_NUMBER 5
#define LIGHT_INTENSETY 500

#define MIN_LASER_PULS_VALUE 150
#define MAX_LASER_PULS_VALUE 255
#define TICK_TIME 375
#define GLOBAL_DELAY 50


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


void setup(void) {
  // We'll send debugging information via the Serial monitor
  if(debug){ 
    Serial.begin(500);  
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
 // check_all_lasers();
 // check_relay_connection();     
}

void print_debug(){   
  
    Serial.print(" Analog  Photore. = ");
    Serial.println(photocellReading);      
         
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
  delay(40); 
  analogWrite(laserPower, 120);
  delay(30); 
  analogWrite(laserPower, 100);
  delay(25); 
  analogWrite(laserPower, 80);
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

void loop(void) {

       current_tick_time = millis();   
        
       if( ( current_tick_time - last_tick_time ) >= tick_time ){  
           
          check_and_set_all_components();
      
         last_tick_time = current_tick_time;   
      }
    
      if(debug){print_debug();}   
      
      delay( GLOBAL_DELAY );      
}        

void  check_and_set_all_components(){

  // allways active the laser from power supplie 
  digitalWrite( laserPowerNetPin , HIGH );     
  
  // if FAN photocell is not reached check it and if the photocell is hit actiate next laser
  
  // Cheack Fan ///////////////////////////////////////////////////////// 
  if(isFanReached == false){
    
         digitalWrite( laserPowerFanPin , LOW); 
         digitalWrite( switchReleayFan , HIGH );    
        // check photocell
        isFanReached = check_input(photocellPinFan, laserPowerFanPin);           
      }
  else{  
      digitalWrite( laserPowerFanPin , HIGH);
      digitalWrite( switchReleayFan , LOW );
    }    
   // Cheack CPU /////////////////////////////////////////////////////////     
  if(isCPUReached == false){  
      digitalWrite( laserPowerCPUPin , LOW );
      digitalWrite( ledSignalCPU_1 , LOW ); 
      digitalWrite( ledSignalCPU_2 , LOW );      
    
    // check onlay if the previous is active
     if(isFanReached){
      isCPUReached = check_input( photocellPinCPU , laserPowerCPUPin); 
     }   
     
    }
  else {     
      digitalWrite( laserPowerCPUPin , HIGH ); 
      digitalWrite( ledSignalCPU_1 , HIGH ); 
      digitalWrite( ledSignalCPU_2 , HIGH); 
    }     
  // Check RAM /////////////////////////////////////////////////////////
  if(isRamReached == false){
      digitalWrite( laserPowerRamPin , LOW );  
      digitalWrite( ledSignalRam , LOW );     
      
     // check onlay if the previous is active
     if( isCPUReached ){
      isRamReached =  check_input(photocellPinRam , laserPowerRamPin);
     }      
  }
  else if( isCPUReached ){       
      digitalWrite( laserPowerRamPin , HIGH ); 
      digitalWrite( ledSignalRam , HIGH );        
  }           
  
  // Cheack HDD /////////////////////////////////////////////////////////
  if(isHDDReached == false){    
       digitalWrite( switchReleayHDD , HIGH );  
       digitalWrite( laserPowerHDDPin , LOW );
      // check onlay if the previous is active
      if( isRamReached ){
       isHDDReached =  check_input( photocellPinHDD  , laserPowerHDDPin);    
      }
    }
  else {
        digitalWrite( switchReleayHDD , HIGH ); 
        digitalWrite( laserPowerHDDPin ,HIGH  );       
      }     
 // Cheack grafic card //////////////////////////////////////////////////
  if(isGraReached == false){
       digitalWrite( laserPowerGraPin , LOW );
       digitalWrite(  switchReleayArduino  , HIGH ); 
       // check onlay if the previous is active
       if( isHDDReached ){
         isGraReached = check_input(  photocellPinGra  , laserPowerGraPin);
       }      
     } 
  else {    
       digitalWrite( laserPowerGraPin , HIGH );
       digitalWrite(  switchReleayArduino  , LOW );
     }   
       
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



