#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1

// use 4 bit precission for LEDC timer
#define LEDC_TIMER_10_BIT  10

// use 2.7MHz Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     50000 //100khz
#define LED_HIGH            5    // HIGH
#define LED_LOW           18   // LOW
#define inh          17  // LOW
#define inl          16
//#include "max6675.h"


int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;
int duty =0;
int pwmH=400;
String stric;
bool start=false,confi=false,indietro=false,dx=true,aut=false;
float runTime, prevT = 0, timeDif, stateT;
int timeInterval = 400;
//MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  // Setup timer and attach timer to a led pin
  Serial.begin(112500);
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_10_BIT);
  //ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_10_BIT);
  GPIO.func_out_sel_cfg[LED_HIGH].inv_sel = 1;
   GPIO.func_out_sel_cfg[LED_LOW].inv_sel = 0;
  //ledcAttachPin(LED_HIGH, LEDC_CHANNEL_0);
 
  //ledcWrite(LEDC_CHANNEL_0, 125);
  delay(500); 

}

void loop() {
   
   while (Serial.available()) {
    // ricevo byte:
    char inChar = (char)Serial.read();
    // aggiungo char a str:
    stric += inChar;
    // controllo H
    if (inChar == 'x') {
      // execute the user command
      pwmH =  constrain(atoi(stric.c_str()), 0, 1000);;
      // reset the command buffer 
      stric = "";
    }
    if (inChar == 's') {
      // execute the user command
      if (aut){
        start=false;
        aut=false;
      }else{
      start=!start;
      }
      // reset the command buffer 
      stric = "";
    }
    if (inChar == 'a') {
      // execute the user command
      aut=!aut;
      // reset the command buffer 
      stric = "";
    }
    if (inChar == 'i') {
      // execute the user command
      indietro=!indietro;
      // reset the command buffer 
      stric = "";
    }
    if (inChar == '\n') {
      // execute the user command
      Serial.println("STATO"+String(start));
      // reset the command buffer 
      stric = "";
      Serial.println("HIGH:"+String(pwmH));
      confi=true;
    }
  }
 if (confi) {
  if (indietro==false) {
  if (start) {
    ledcAttachPin(LED_LOW, LEDC_CHANNEL_0);
    ledcAttachPin(LED_HIGH, LEDC_CHANNEL_0);
    ledcAttachPin(inh, LEDC_CHANNEL_0);
    ledcAttachPin(inl, LEDC_CHANNEL_0);
    GPIO.func_out_sel_cfg[LED_HIGH].inv_sel = 1;
    GPIO.func_out_sel_cfg[LED_LOW].inv_sel = 0;
    GPIO.func_out_sel_cfg[inh].inv_sel = 1;
    GPIO.func_out_sel_cfg[inl].inv_sel = 0;
    ledcWrite(LEDC_CHANNEL_0, pwmH);
    //ledcWrite(LEDC_CHANNEL_1, pwmH);
  
  }else{
    //ledcWrite(LEDC_CHANNEL_0, 1023);
    //ledcWrite(LEDC_CHANNEL_1, 1023);
    //ledcChangeFrequency(LEDC_CHANNEL_0, 4000, 10);
    //GPIO.func_out_sel_cfg[LED_HIGH].inv_sel = 0;
    //GPIO.func_out_sel_cfg[LED_LOW].inv_sel = 0;
    ledcDetachPin(LED_HIGH);
    ledcDetachPin(inh);
    ledcDetachPin(LED_LOW);
    ledcDetachPin(inl);
  }
  } else {
    if (start) {
    ledcAttachPin(LED_LOW, LEDC_CHANNEL_0);
    ledcAttachPin(LED_HIGH, LEDC_CHANNEL_0);
    ledcAttachPin(inh, LEDC_CHANNEL_0);
    ledcAttachPin(inl, LEDC_CHANNEL_0);
    GPIO.func_out_sel_cfg[LED_HIGH].inv_sel = 0;
    GPIO.func_out_sel_cfg[LED_LOW].inv_sel = 1;
    GPIO.func_out_sel_cfg[inh].inv_sel = 0;
    GPIO.func_out_sel_cfg[inl].inv_sel = 1;
    ledcWrite(LEDC_CHANNEL_0, pwmH);
    //ledcWrite(LEDC_CHANNEL_1, pwmH);
  }else{
    //ledcWrite(LEDC_CHANNEL_0, 1023);
    //ledcWrite(LEDC_CHANNEL_1, 1023);
    //ledcChangeFrequency(LEDC_CHANNEL_0, 4000, 10);
    
    ledcDetachPin(LED_LOW);
    ledcDetachPin(inl);
    ledcDetachPin(LED_HIGH);
    ledcDetachPin(inh);  
  }
  }
  confi=false;
 }
  if (aut) {
  runTime = millis();
  timeDif = runTime - prevT;  //How much time each loop takes
  prevT = runTime;
  stateT += timeDif;
 

 
  
  if(stateT >= timeInterval){
    // e220ttl.setMode(MODE_1_WOR_TRANSMITTER);
    //ResponseStatus rs;
    if (pwmH==50){
      dx=false;
    } 
    if (pwmH>=450){
      dx=true;
    } 
    if (dx) {
      pwmH=pwmH-1;
    }else{
      pwmH=pwmH+1;
    }
    //ledcWrite(LEDC_CHANNEL_1, pwmH);
    ledcWrite(LEDC_CHANNEL_0, pwmH);
    //Serial.println(rs.getResponseDescription());
    stateT = 0;
    //Serial.println(sp);
  
  } 
   } 
   //delay(200);
}
