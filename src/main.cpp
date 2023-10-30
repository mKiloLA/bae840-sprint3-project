#include "Arduino.h"
#include "AS726X.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h"

void readSensors();

// Delcare sensor objects
U8G2_SSD1327_EA_W128128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
SFEVL53L1X distanceSensor;
AS726X spectralSensor;

// Sensor variables to hold most up-to-date reading
int distanceTOF;
int strengthRed;
int strengthBlue = 0;

// Spectral sensor settings
uint8_t gain = 3;
uint8_t measurementMode = 3;
uint8_t integrationTime = 10;

// Setup routine
void setup(void)
{
  Wire.begin();
  Serial.begin(115200);
  spectralSensor.begin();
  spectralSensor.setGain(gain);
  spectralSensor.setMeasurementMode(measurementMode);
  spectralSensor.setIntegrationTime(integrationTime);


  if (distanceSensor.begin() != 0)
  {
    Serial.println("TOF failed to begin. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Sensors online!");

  u8g2.setBusClock(400000);
  u8g2.begin();
}

void loop(void)
{
  readSensors();

  u8g2.firstPage();
  do{
    // TIME OF FLIGHT INDICATORS
    if (distanceTOF < 1500) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 0, 128, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 128, 5); 
    }
    if (distanceTOF < 1250) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 9, 128, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 9, 128, 5);
    }
    if (distanceTOF < 1000) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 18, 128, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 18, 128, 5);
    }
    if (distanceTOF < 750) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 27, 128, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 27, 128, 5);
    }

    // RED LIGHT INDICATORS
    if (strengthRed > 0) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 91, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 91, 62, 5);
    }
    if (strengthRed > 10) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 82, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 82, 62, 5);
    }
    if (strengthRed > 50) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 73, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 73, 62, 5);
    }
    if (strengthRed > 100) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 64, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 64, 62, 5);
    }
    if (strengthRed > 200) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 55, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 55, 62, 5);
    }

    // BLUE LIGHT INDICATORS
    if (strengthBlue > 0) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(66, 91, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(66, 91, 62, 5);
    }
    if (strengthBlue > 10) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(66, 82, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(66, 82, 62, 5);
    }
    if (strengthBlue > 50) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(66, 73, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(66, 73, 62, 5);
    }
    if (strengthBlue > 100) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(66, 64, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(66, 64, 62, 5);
    }
    if (strengthBlue > 200) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(66, 55, 62, 5);
    } else {
      u8g2.setDrawColor(0);
      u8g2.drawBox(66, 55, 62, 5);
    }

    // Write the distances to the OLED display
    u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
    u8g2.setDrawColor(2);
    u8g2.drawStr(16, 48, String(strengthRed).c_str());
    u8g2.drawStr(48, 48, String(distanceTOF).c_str());
    u8g2.drawStr(96, 48, String(strengthBlue).c_str());

  }
  while(u8g2.nextPage());
}

void readSensors(void) {
  // Read time of flight sensor
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady()) {
    delay(1);
  }
  distanceTOF = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  spectralSensor.takeMeasurements();
  strengthRed = spectralSensor.getCalibratedRed();
  strengthBlue = spectralSensor.getCalibratedViolet();
}
