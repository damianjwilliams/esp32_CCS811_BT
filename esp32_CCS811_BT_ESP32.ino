
 
#include <Wire.h>
#include <SparkFunBME280.h> //Click here to get the library: http://librarymanager/All#SparkFun_BME280
#include <SparkFunCCS811.h> //Click here to get the library: http://librarymanager/All#SparkFun_CCS811


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial blueSer;

int input;

#define CCS811_ADDR 0x5B //Default I2C Address



//Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;

void setup()
{
  Serial.begin(115200);
  //Serial.println();
  Serial.println("Apply BME280 data to CCS811 for compensation.");
   blueSer.begin("ESP32_enviroment_sensor"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  Wire.begin();

  //This begins the CCS811 sensor and prints error status of .beginWithStatus()
  CCS811Core::CCS811_Status_e returnCode = myCCS811.beginWithStatus();
  Serial.print("CCS811 begin exited with: ");
 Serial.println(myCCS811.statusString(returnCode));

  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x77;

  //Initialize BME280
  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x77;
  myBME280.settings.runMode = 3; //Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  //Calling .begin() causes the settings to be loaded
  delay(10); //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  myBME280.begin();
}
//---------------------------------------------------------------
void loop()
{
  //Check to see if data is available
  if (myCCS811.dataAvailable())
  {
    //Calling this function updates the global tVOC and eCO2 variables
    myCCS811.readAlgorithmResults();
    //printInfoSerial fetches the values of tVOC and eCO2
    printInfoSerial();

    float BMEtempC = myBME280.readTempC();
    float BMEhumid = myBME280.readFloatHumidity();

   // Serial.print("Applying new values (deg C, %): ");
   // Serial.print(BMEtempC);
    //Serial.print(",");
    //Serial.println(BMEhumid);
    //Serial.println();

    //This sends the temperature data to the CCS811
    myCCS811.setEnvironmentalData(BMEhumid, BMEtempC);
  }
  else if (myCCS811.checkForStatusError())
  {
    //If the CCS811 found an internal error, print it.
    printSensorError();
  }

  delay(500); //Wait for next reading
}

//---------------------------------------------------------------
void printInfoSerial()
{
  //getCO2() gets the previously read data from the library
 Serial.println("CCS811 data:");
  Serial.print(" CO2 concentration : ");
  Serial.print(myCCS811.getCO2()); 
  Serial.println(" ppm");
   float CO2 = myCCS811.getCO2();

  //getTVOC() gets the previously read data from the library
  Serial.print(" TVOC concentration : ");
  Serial.print(myCCS811.getTVOC());  
  Serial.println(" ppb");
  float TVOC = myCCS811.getTVOC();

  Serial.println("BME280 data:");
  Serial.print(" Temperature: ");
  Serial.print(myBME280.readTempC(), 2);
  Serial.println(" degrees C");
  float temp_bme = myBME280.readTempC();



  Serial.print(" Pressure: ");
  Serial.print((myBME280.readFloatPressure() * 0.0002953), 2);
  Serial.println(" Pa");
  float pressure = myBME280.readFloatPressure();

  Serial.print(" Altitude: ");
 Serial.print(myBME280.readFloatAltitudeMeters(), 2);
 Serial.println("m");
  float altitude = myBME280.readFloatAltitudeMeters();

  Serial.print(" Altitude: ");
  Serial.print(myBME280.readFloatAltitudeFeet(), 2);
  Serial.println("ft");

  Serial.print(" %RH: ");
 Serial.print(myBME280.readFloatHumidity(), 2);
 Serial.println(" %");
  float humidity = myBME280.readFloatHumidity();

  if (blueSer.available()) 
  {
    input = blueSer.read(); 
    Serial.print("Received:"); Serial.println(input);
    
    blueSer.println(String(temp_bme)+","+String(CO2)+","+String(pressure)+","+String(humidity)","+String(TVOC));

  
    
  }
 
  
 
}

//printSensorError gets, clears, then prints the errors
//saved within the error register.
void printSensorError()
{
  uint8_t error = myCCS811.getErrorRegister();

  if (error == 0xFF) //comm error
  {
    Serial.println("Failed to get ERROR_ID register.");
  }
  else
  {
    Serial.print("Error: ");
    if (error & 1 << 5)
      Serial.print("HeaterSupply");
    if (error & 1 << 4)
      Serial.print("HeaterFault");
    if (error & 1 << 3)
      Serial.print("MaxResistance");
    if (error & 1 << 2)
      Serial.print("MeasModeInvalid");
    if (error & 1 << 1)
      Serial.print("ReadRegInvalid");
    if (error & 1 << 0)
      Serial.print("MsgInvalid");
    Serial.println();
  }
}
