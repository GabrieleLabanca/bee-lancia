#include <SPI.h>
#include <SD.h> //to use sd card  

//The library used for arduino  https://github.com/bogde/HX711
// LCD can also be used instead of serial 
#include "HX711.h"
// HX711.DOUT  -  8 (BEFORE: pin 10)
// HX711.PD_SCK - 7 (BEFORE: pin 11)
//scale(DOUT,SCK)
HX711 scale(10,11); // parameter "gain" is omitted; the default value 128 is used by the lib
  
File outfile;
File myFile;
  
void setup() 
{
  scale.power_up();
  delay(1000);;
  Serial.begin(38400);
  Serial.println("BEGIN SERIAL");
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB port only
  //}


  Serial.println("HX711 Demo");  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t ");
  Serial.println(scale.read());     // print a raw reading from the ADC 
  Serial.print("read average:\t\t ");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
  Serial.print("get value: \t\t ");
  Serial.println(scale.get_value(5));   
  // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get units: \t\t ");
  Serial.println(scale.get_units(5), 1);  
  /// print the average of 5 readings from the ADC minus tare weight 
  //(not set) divided by the SCALE parameter (not set yet) 
  scale.set_scale(114.f);//scale.set_scale(2280.f);  
  // this value is obtained by calibrating the scale with known weights; 
  //see the README for details
  scale.tare();   // reset the scale to 0
  Serial.println("After setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());                        // print a raw reading from the ADC
  Serial.print("read average:\t\t ");
  Serial.println(scale.read_average(20));       
  // print the average of 20 readings from the ADC
  Serial.print("get value: \t\t ");    
  // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.println(scale.get_value(5));
  Serial.print("get units: ");
  Serial.println(scale.get_units(5), 1);      
  // print the average of 5 readings from the ADC minus tare weight, 
  //divided by the SCALE parameter set with set_scale


  // initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if(SD.remove("data.txt")) Serial.println("Removed data.txt");

  Serial.println("Readings:");

}


void loop() 
{
  //Serial.print("Weight :");                        
  /* ---------Weight in terms of KG-------------*/
  float measure = scale.get_units()* .005600966442953021;
  Serial.println(measure, 3); //tara con 3.870kg 

  
  outfile = SD.open("data.txt", FILE_WRITE);
  if(outfile){
    outfile.println(measure);
    //outfile.print('\n');
    outfile.close();
  }
  else {
    Serial.println("error opening data.txt");
  }
  



  //Serial.print(scale.read());
  //Serial.print("KG");

  //scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  //Serial.print("\n");  
  //scale.power_up();
}
