#define MAX_PROSSES_NUMBER 5
#define LIGHT_INTENSETY 300

int globalDelay = 20;
int current_tick_time = millis();
int last_tick_time = 0;

// photocells to messure the laser light

int photocellPinFan = A0;    // the cell and 10K pulldown are connected to a0
int photocellReadingFan;     // the analog reading from the analog resistor divider

int photocellPinCPU = A1;     
int photocellReadingCPU;     

int photocellPinRam = A2;   
int photocellReadingRam;   

int photocellPinHDD = A3;     
int photocellReadingHDD;   

int photocellPinGra = A4;    
int photocellReadingGra;   

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
int laserPowerHDDPin = 10; // Hard Drive
int laserPowerGraPin = 11; // Grafic card

/* for programm progress status
  is Start up    = 0
  is Fan Reached = 1
  is CPU Reached = 2
  is Ram Reached = 3
  is HDD Reached = 4
  is Gra Reached = 5
*/
int progress_counter = 0;
///// // // // // //

boolean pulseLaser = true;

int laserPulseDelay = 375;

boolean debug = false;

void setup(void) {
  // We'll send debugging information via the Serial monitor
  if(debug){ 
    Serial.begin(9600);  
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
  
  digitalWrite( switchReleayFan , HIGH );   
  digitalWrite( switchReleayHDD , HIGH ); 
  digitalWrite( switchReleayArduino , HIGH ); 
  
  // STARTUP CHECK
  check_all_lasers();
  check_relay_connection();     
}

void print_debug(){   
  
    Serial.print(" Analog photocell reading FAN = ");
    Serial.println(photocellReadingFan);   
   
    Serial.print(" Analog photocell reading CPU = ");
    Serial.println(photocellReadingCPU);    
    
    Serial.print(" Analog photocell reading Ram = ");
    Serial.println(photocellReadingRam);
   
    Serial.print(" Analog photocell reading HDD = ");
    Serial.println(photocellReadingHDD);  
    
    Serial.print(" Analog photocell reading Gra = ");
    Serial.println(photocellReadingGra);   

    Serial.print(" Programm-progress_counter = ");
    Serial.println(progress_counter);           
}

// first
void check_input(int photocellRead,  int laserPower){     
  
  
  //  CURRENT TEST SIDE
     analogWrite(laserPower, 175);      
     delay(laserPulseDelay);    
     analogWrite(laserPower, 80);      
     delay(laserPulseDelay);    
 /// 
 
      if (photocellRead < LIGHT_INTENSETY ){    
   
          if(progress_counter > MAX_PROSSES_NUMBER){
        
            progress_counter = MAX_PROSSES_NUMBER;
        
          }else{   
            progress_counter++;   
          }             
      }    
}

void loop(void) {
        
  if(debug){print_debug();}      
    
  photocellReadingFan = analogRead( photocellPinFan );  
  photocellReadingCPU = analogRead( photocellPinCPU ); 
  photocellReadingRam = analogRead( photocellPinRam ); 
  photocellReadingHDD = analogRead( photocellPinHDD ); 
  photocellReadingGra = analogRead( photocellPinGra );
   
   if( ( current_tick_time - last_tick_time ) >= laserPulseDelay ){  
       
     check_state_increase_prosses_count();
     set_components_to_progress_counter_level();
  
     last_tick_time = current_tick_time;   
  }

  current_tick_time = millis();
  
  delay( globalDelay );
  
   // ALLWAYS ON AFTER START UP
    digitalWrite(  switchReleayArduino  , LOW ); 
    digitalWrite( laserPowerNetPin , LOW );        
}

void check_state_increase_prosses_count(){
  
           if(progress_counter == 4){
              check_input(photocellReadingGra, laserPowerGraPin);}
            
           if(progress_counter == 3){
              check_input(photocellReadingHDD, laserPowerHDDPin);}
             
            if(progress_counter == 2){
              check_input(photocellReadingRam, laserPowerRamPin);}
             
            if(progress_counter == 1){
                check_input(photocellReadingCPU, laserPowerCPUPin);}
             
            if(progress_counter == 0){
              check_input(photocellReadingFan, laserPowerFanPin);}    
}

void  set_components_to_progress_counter_level(){
  
   if(progress_counter == 0){
       digitalWrite( laserPowerFanPin , HIGH ); 
       digitalWrite( switchReleayFan , HIGH );     
      }
  else{ 
      digitalWrite( laserPowerFanPin , LOW );
      digitalWrite( switchReleayFan , LOW );
    }
    
  if(progress_counter <= 1){  
      digitalWrite( laserPowerCPUPin , HIGH );
      digitalWrite( ledSignalCPU_1 , LOW ); 
      digitalWrite( ledSignalCPU_2 , LOW );  
    }
  else { 
      digitalWrite( laserPowerCPUPin , LOW ); 
      digitalWrite( ledSignalCPU_1 , HIGH ); 
      digitalWrite( ledSignalCPU_2 , HIGH); 
    }     
    
  if(progress_counter <= 2){
      digitalWrite( ledSignalRam , LOW ); 
      digitalWrite( laserPowerRamPin , HIGH );      
  }
  else { 
      digitalWrite( ledSignalRam , HIGH ); 
      digitalWrite( laserPowerRamPin , LOW );    
  }           
  
  if(progress_counter <= 3){    
       digitalWrite( switchReleayHDD , HIGH );  
       digitalWrite( laserPowerHDDPin , HIGH );}
  else {
        digitalWrite( switchReleayHDD , LOW ); 
       digitalWrite( laserPowerHDDPin , LOW );       
      }     
  
  if(progress_counter <= 4){
      digitalWrite( laserPowerGraPin , HIGH );
       digitalWrite(  switchReleayArduino  , HIGH ); } 
  else {
       digitalWrite( laserPowerGraPin , LOW );
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



