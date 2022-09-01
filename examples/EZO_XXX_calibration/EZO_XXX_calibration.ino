#define DO_I2C_ADDR 0x61
#define EC_I2C_ADDR 0x64
#define RTD_I2C_ADDR 0x66
#define PH_I2C_ADDR 0x63

// uncomment the specific sesnor to initiate the calibration sequence
#define CALIBRATION_STABILIZE_DELAY 60 // time in seconds
//#define CALIBRATE_RTD
//#define CALIBRATE_EC
#define CALIBRATE_PH
//#define CALIBRATE_DO
#define TEMPERATURE_COMPENSATION



#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>    //include arduinos i2c library
#include <sequencer2.h> //imports a 2 function sequencer 
#include <Ezo_i2c_util.h> //brings in common print statements

Ezo_board DO = Ezo_board(DO_I2C_ADDR, "DO");       //create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board EC = Ezo_board(EC_I2C_ADDR, "EC");      //create an EC circuit object who's address is 100 and name is "EC"
Ezo_board RTD = Ezo_board(RTD_I2C_ADDR, "RTD");       //create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board PH = Ezo_board(PH_I2C_ADDR, "PH");      //create an EC circuit object who's address is 100 and name is "EC"

void step1();  //forward declarations of functions to use them in the sequencer before defining them
void step2();

Sequencer2 Seq(&step1, 1000, &step2, 0);  //calls the steps in sequence with time in between them
int VDD_EN = 13;
void setup() {
   pinMode(VDD_EN, OUTPUT);
  digitalWrite(VDD_EN, HIGH);
  Wire.begin();                           //start the I2C
  Serial.begin(115200);                     //start the serial communication to the computer


  #ifdef CALIBRATE_RTD
    RTD_calib();
  #endif
  
  #ifdef CALIBRATE_PH
    PH_calib();
  #endif
  
  #ifdef CALIBRATE_DO
    DO_calib();
  #endif
  
  #ifdef CALIBRATE_EC
    EC_calib();
  #endif

  Seq.reset();                            //initialize the sequencer
}

void DO_calib(){
  int delay_ = 1000, delay_times = CALIBRATION_STABILIZE_DELAY;
    float do_;
    float calibDo = 0;
    Serial.println("-------------------initializing DO calibration----------------");
    for(int i = 0; i < delay_times; i++){
      DO.send_read_cmd();
      delay(delay_);
      DO.receive_read_cmd(); 
      do_ = DO.get_last_received_reading();
      Serial.println("Awaiting for DO dry atm to stabilize DO: "+String(do_, 3));
    }
    
    Serial.println("Calibrating DO using atmospheric refernce DO_cmd: Cal");
    DO.send_cmd("Cal");
    delay(2000);

   for(int i = 0; i < delay_times; i++){
    DO.send_read_cmd();
    delay(delay_);
    DO.receive_read_cmd(); 
    do_ = DO.get_last_received_reading();
    Serial.println("DO calibrated DO: "+String(do_, 3)+" "+String(i)+"/"+String(delay_times));
  } 
  
}

void EC_calib(){
  int delay_ = 1000, delay_times = CALIBRATION_STABILIZE_DELAY;
  float ec, temp;
  Serial.println("-------------------initializing EC calibration----------------");
  for(int i = 0; i < delay_times; i++){
    EC.send_read_cmd();
    delay(delay_);
    EC.receive_read_cmd(); 
    ec = EC.get_last_received_reading();
    Serial.println("Awaiting for EC  dry to stabilize EC: "+String(ec, 3)+" "+String(i)+"/"+String(delay_times));
  }

  Serial.println("Calibrating dry, EC_cmd: Cal,dry");
  EC.send_cmd("Cal,dry");
  delay(2000);
 for(int i = 0; i < delay_times; i++){
    EC.send_read_cmd();
    delay(delay_);
    EC.receive_read_cmd(); 
    ec = EC.get_last_received_reading();
    Serial.println("Awaiting for EC low to stabilize EC: "+String(ec, 3)+" "+String(i)+"/"+String(delay_times));
  }
  
  Serial.println("Calibrating low: EC_cmd: Cal,low,12880");
  EC.send_cmd("Cal,low,12880");
  delay(2000);

  for(int i = 0; i < delay_times; i++){
    EC.send_read_cmd();
    delay(delay_);
    EC.receive_read_cmd(); 
    ec = EC.get_last_received_reading();
    Serial.println("Awaiting for EC high to stabilize EC: "+String(ec, 3)+" "+String(i)+"/"+String(delay_times));
  }

  Serial.println("Calibrating low: EC_cmd: Cal,high,80000");
  EC.send_cmd("Cal,high,80000");
  delay(2000);
  for(int i = 0; i < delay_times/2; i++){
    EC.send_read_cmd();
    delay(delay_);
    EC.receive_read_cmd(); 
    ec = EC.get_last_received_reading();
    Serial.println("EC calibrated EC: "+String(ec, 3)+" "+String(i)+"/"+String(delay_times));
  } 

}

void RTD_calib(){
    int delay_ = 1000, delay_times = CALIBRATION_STABILIZE_DELAY;
    float temp;
    Serial.println("-------------------initializing RTD calibration----------------");
    for(int i = 0; i < delay_times; i++){
      RTD.send_read_cmd();
      delay(delay_);
      RTD.receive_read_cmd(); 
      temp = RTD.get_last_received_reading();
      Serial.println("Awaiting for Temperature to stabilize RTD: "+String(temp, 3));
    }

    Serial.println("Calibrating calibration temperature: 100 cmd: Cal,100");
    RTD.send_cmd("Cal,100");
    delay(2000);

   for(int i = 0; i < delay_times/2; i++){
    RTD.send_read_cmd();
    delay(delay_);
    RTD.receive_read_cmd(); 
    temp = RTD.get_last_received_reading();
    Serial.println("RTD calibrated RTD: "+String(temp, 3)+" "+String(i)+"/"+String(delay_times));
  } 
  
}


void PH_calib(){
  int delay_ = 1000, delay_times = CALIBRATION_STABILIZE_DELAY;
  float temperature_compensation = 0, ph, temp;

  Serial.println("-------------------initializing PH calibration----------------");
  PH.send_cmd("Cal,clear");
  delay(2000);
  Serial.println("Send Clear calibration_cmd: Cal,clear");
  for(int i = 0; i < delay_times; i++){
    RTD.send_read_cmd();
    PH.send_read_cmd();
    delay(delay_);
    PH.receive_read_cmd(); 
    RTD.receive_read_cmd(); 
    ph = PH.get_last_received_reading();
    temp = RTD.get_last_received_reading();
    Serial.println("Awaiting for mid to stabilize RTD: "+String(temp, 3)+" PH: "+String(ph, 3)+" "+String(i)+"/"+String(delay_times));
  }
  
  
  String x = "T,"+String(temp,3);
  Serial.println("Calibrating mid: "+String(7)+" PH_cmd: Cal,mid,7");
  #ifdef TEMPERATURE_COMPENSATION
     Serial.println(" Tcompensation: "+String(temp)+" Tcompensation_cmd: "+x);
    PH.send_cmd(x.c_str());
  #endif
  delay(2000);
  PH.send_cmd("Cal,mid,7");

  for(int i = 0; i < delay_times; i++){
    RTD.send_read_cmd();
    PH.send_read_cmd();
    delay(delay_);
    PH.receive_read_cmd(); 
    RTD.receive_read_cmd(); 
    float ph = PH.get_last_received_reading();
    float temp = RTD.get_last_received_reading();
    Serial.println(" Awaiting for low to stabilize   RTD: "+String(temp, 3)+" PH: "+String(ph, 3)+" "+String(i)+"/"+String(delay_times));
  }
  x = "T,"+String(temp,3);
  Serial.println("Calibrating mid: "+String(4)+" PH_cmd: Cal,mid,4");
  #ifdef TEMPERATURE_COMPENSATION
     Serial.println(" Tcompensation: "+String(temp)+" Tcompensation_cmd: "+x);
    PH.send_cmd(x.c_str());
  #endif
  delay(2000);
  PH.send_cmd("Cal,low,4");
  delay(2000);

  for(int i = 0; i < delay_times; i++){
    RTD.send_read_cmd();
    PH.send_read_cmd();
    delay(delay_);
    PH.receive_read_cmd(); 
    RTD.receive_read_cmd(); 
    float ph = PH.get_last_received_reading();
    float temp = RTD.get_last_received_reading();
    Serial.println("Awaiting for high to stabilize RTD: "+String(temp, 3)+" PH: "+String(ph, 3)+" "+String(i)+"/"+String(delay_times));
  }

  x = "T,"+String(temp,3);
    Serial.println("Calibrating mid: "+String(10)+" PH_cmd: Cal,mid,10");
  #ifdef TEMPERATURE_COMPENSATION
     Serial.println(" Tcompensation: "+String(temp)+" Tcompensation_cmd: "+x);
    PH.send_cmd(x.c_str());
  #endif
  delay(2000);
  PH.send_cmd("Cal,high,10");
  delay(2000);

  for(int i = 0; i < delay_times/2; i++){
    RTD.send_read_cmd();
    PH.send_read_cmd();
    delay(delay_);
    PH.receive_read_cmd(); 
    RTD.receive_read_cmd(); 
    ph = PH.get_last_received_reading();
    temp = RTD.get_last_received_reading();
    Serial.println("PH calibrated RTD: "+String(temp, 3)+" PH: "+String(ph, 3)+" "+String(i)+"/"+String(delay_times));
  } 
}



void loop() {
  Seq.run();                              //run the sequncer to do the polling
}


void step1(){
   //send a read command. we use this command instead of PH.send_cmd("R"); 
  //to let the library know to parse the reading
  PH.send_read_cmd();                      
  EC.send_read_cmd();
  DO.send_read_cmd();
  RTD.send_read_cmd();
}

void step2(){
  receive_and_print_reading(PH);             //get the reading from the PH circuit
  Serial.print("  ");
  receive_and_print_reading(EC);             //get the reading from the EC circuit
 Serial.print("  "); 
  receive_and_print_reading(DO);             //get the reading from the EC circuit
  Serial.print("  ");
  receive_and_print_reading(RTD);             //get the reading from the EC circuit
  Serial.println();
}
