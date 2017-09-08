#include <SPI.h>
#include <SD.h> //to use sd card  
#include "HX711.h"//The library used for arduino  https://github.com/bogde/HX711

/////////////////////////////////
// setup HX711
// HX711.DOUT  -  8 (BEFORE: pin 10)
// HX711.PD_SCK - 7 (BEFORE: pin 11)
//scale(DOUT,SCK)
HX711 scale(7,6); // parameter "gain" is omitted; the default value 128 is used by the lib
  
  
File outfile;
File myFile;

void setup() 
{
  Serial.begin(38400);
  Serial.println("BEGIN SERIAL");

  /////////////////////////////////////
  // set HX711 parameters
  scale.power_up();
  delay(1000);
  
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t ");
  Serial.println(scale.read());     // print a raw reading from the ADC 
  float myscale = 114. / .005600966442953021;
  scale.set_scale(myscale); // this value is obtained by calibrating the scale with known weights;
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


  ////////////////////////////////////////
  // initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  SD.remove("DATA.txt");
  SD.remove("data.txt"); // problems with upper-/lowercase
  

  Serial.println("Readings:");

}

void loop() 
{
  //Serial.print("Weight :");                        
  /* ---------Weight in terms of KG-------------*/
  float measure = scale.get_units();//* .005600966442953021;
  Serial.println(measure, 3); //tara con 3.870kg 

  outfile = SD.open("data.txt", FILE_WRITE);
  if(outfile){
    outfile.println(measure,3);
    //outfile.print('\n');
    outfile.close();
  }
  else {
    Serial.println("error opening data.txt");
  }

  //scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  //scale.power_up();
  

  
}
