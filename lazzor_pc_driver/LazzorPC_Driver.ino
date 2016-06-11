
int globalDelay = 20;
int current_tick_time = millis();
int last_tick_time = 0;

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

// switch
int switchReleayFan = 4;
int switchReleayHDD = 12;
int switchReleayArduino = 13;

// LEDs for signals
int ledSignalCPU_1 = 7; // yellow
int ledSignalCPU_2 = 8; // yellow
int ledSignalRam = 2;   // yellow

// all lasers
int laserPowerNet = 3;
int laserPowerFan = 5;
int laserPowerCPU = 6;
int laserPowerRam = 9;
int laserPowerHDD = 10;
int laserPowerGra = 11;

/* for switch case status
  is Fan Reached = 1
  is CPU Reached = 2
  is Ram Reached = 3
  is HDD Reached = 4
  is Gra Reached = 5
*/
int progress = 0;
///// // // // // //

boolean pulseLaser = true;

int laserPulseDelay = 375;

boolean debug = false;

void setup(void) {
  // We'll send debugging information via the Serial monitor
 // if(debug){ 
    Serial.begin(9600);  
  //}

  pinMode( switchReleayFan, OUTPUT );
  pinMode( switchReleayHDD , OUTPUT );
  pinMode(switchReleayArduino  , OUTPUT );
  
  pinMode(ledSignalCPU_1,OUTPUT ); 
  pinMode(ledSignalCPU_2,OUTPUT ); 
  pinMode(ledSignalRam,  OUTPUT ); 
   
  pinMode(laserPowerNet , OUTPUT );
  pinMode(laserPowerFan , OUTPUT );
  pinMode(laserPowerCPU , OUTPUT );
  pinMode(laserPowerRam , OUTPUT );
  pinMode(laserPowerHDD , OUTPUT );
  pinMode(laserPowerGra , OUTPUT );  

  digitalWrite( ledSignalCPU_1 , LOW ); 
  digitalWrite( ledSignalCPU_2 , LOW ); 
  digitalWrite( ledSignalRam , LOW ); 
  
  digitalWrite( switchReleayFan , HIGH );   
  digitalWrite( switchReleayHDD , HIGH ); 
  digitalWrite( switchReleayArduino , HIGH ); 
  
 
    
}

void print_debug(){   
    //erial.print(" Analog photocell reading = ");
    //Serial.println(photocellReadingFan);     // the raw analog reading

    Serial.print(" Digital swicht = ");
    Serial.println(progress);   
    
    
}

// first
void check_input(int photocellRead,  int laserPower){     
    
     delay(laserPulseDelay);     
  
    
      if (photocellRead < 300) {         
        progress++;                
      } 
      
      
    //  progress %= 5;
       
}

void loop(void) {
  
  if(debug){check_all_lasers();}
  
  photocellReadingFan = analogRead( photocellPinFan );  
  photocellReadingCPU = analogRead( photocellPinCPU ); 
  photocellReadingRam = analogRead( photocellPinRam ); 
  photocellReadingHDD = analogRead( photocellPinHDD ); 
  photocellReadingGra = analogRead( photocellPinGra );

   
   if( ( current_tick_time - last_tick_time ) >= laserPulseDelay ){  
     
   
           if(progress == 4){
              check_input(photocellReadingGra, laserPowerGra);}
            
           if(progress == 3){
              check_input(photocellReadingHDD, laserPowerHDD);}
             
            if(progress == 2){
              check_input(photocellReadingRam, laserPowerRam);}
             
            if(progress == 1){
                check_input(photocellReadingCPU, laserPowerCPU);}
             
            if(progress == 0){
              check_input(photocellReadingFan, laserPowerFan);}
             
  
   last_tick_time = current_tick_time;   
  }
        
  /*
  if (photocellReadingFan < 300) { 
    //  digitalWrite( switchReleayHDD , LOW );
  } 
  else{  
     digitalWrite( switchReleayHDD , HIGH );  
  }   
 */
      
  if(debug){
     print_debug();
  }  
    
  current_tick_time = millis();
  
  delay( globalDelay );
    digitalWrite(  switchReleayArduino  , LOW );
   // high is out
  digitalWrite( laserPowerNet , LOW );
  
  if(progress == 0){
       digitalWrite( laserPowerFan , HIGH ); 
       digitalWrite( switchReleayFan , HIGH );     
      }
  else{ 
      digitalWrite( laserPowerFan , LOW );
      digitalWrite( switchReleayFan , LOW );
    }
    
  if(progress <= 1){  
      digitalWrite( laserPowerCPU , HIGH );
      digitalWrite( ledSignalCPU_1 , LOW ); 
      digitalWrite( ledSignalCPU_2 , LOW );  
    }
  else { 
      digitalWrite( laserPowerCPU , LOW ); 
      digitalWrite( ledSignalCPU_1 , HIGH ); 
      digitalWrite( ledSignalCPU_2 , HIGH); 
    }     
    
  if(progress <= 2){
      digitalWrite( ledSignalRam , LOW ); 
      digitalWrite( laserPowerRam , HIGH );      
  }
  else { 
      digitalWrite( ledSignalRam , HIGH ); 
      digitalWrite( laserPowerRam , LOW );    
  }           
  
  if(progress <= 3){    
       digitalWrite( switchReleayHDD , HIGH );  
       digitalWrite( laserPowerHDD , HIGH );}
  else {
        digitalWrite( switchReleayHDD , LOW ); 
       digitalWrite( laserPowerHDD , LOW );       
      }     
  
  if(progress <= 4){
      digitalWrite( laserPowerGra , HIGH );
       digitalWrite(  switchReleayArduino  , HIGH ); } 
  else {
       digitalWrite( laserPowerGra , LOW );
       digitalWrite(  switchReleayArduino  , LOW );
     }   
       
}

void check_all_lasers(){
   delay( 500);
  digitalWrite( laserPowerNet , HIGH );
   delay( 500);
  digitalWrite( laserPowerFan , HIGH );
   delay( 500);
  digitalWrite( laserPowerCPU , HIGH );
   delay( 500);
  digitalWrite( laserPowerRam , HIGH );
   delay( 500);
  digitalWrite( laserPowerHDD , HIGH );
   delay( 500);
  digitalWrite( laserPowerGra , HIGH );   
}



