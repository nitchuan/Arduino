#include "HX711.h"

// HX711 Wiring
#define DOUT  D5
#define CLK  D6 

HX711 scale;

float calibration_factor = -96.00; // Hasil kalibrasi
float grams;

void setup() {
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  grams = scale.get_units(), 10;
  if (grams < 0)
  {
    grams = 0.00;
  }

  Serial.print(grams);
  Serial.print(" grams"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 2;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 2;
  }
}
