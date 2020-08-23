# esp32_CCS811_BT
Environmental data from a CCS811 using Bluetooth, displayed on a Python GUI



Arduino and python code for streaming CCS811 sensor data over Bluetooth. The measurements are displayed on a Python tkinter GUI. The arduino BluetoothSerial ESP32 library, provides an emulated Serial connection which operates using Bluetooth classic. The arduino code is based on the ESP32 SerialtoSerial example in the ESP32 core and the [SparkFun CCS811 Arduino Library](https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library). A BME280 is integrated to enable temperature compensation of the CCS811 readings. Much of the python code for the GUI is from an [adafruit tutorial](https://learn.adafruit.com/bluetooth-temperature-and-humidity-sensor/python-interface). 


## Requirements
- ESP32 -  I used the HUZZAH32 – ESP32 Feather Board from [Adafruit](https://www.adafruit.com/product/3405)

- BME280 - Sparkfun Atmospheric Sensor Breakout - BME280 from [SparkFun](https://www.sparkfun.com/products/13676)

- CCS811 - Air Quality Breakout from [SparkFun](https://www.sparkfun.com/products/14193)

- Python 3.6+


## Usage

An overview of the process can be found on [Youtube](https://www.youtube.com/watch?v=l7pM1rQDjBs). The instructions are for Windows 10, but easily adapted for OSX, etc.

Upload ESP32 code and libraries described in the links above. An overview of the process can be found on Youtube.

Once the code is uploaded, "The device started, now you can pair it with bluetooth!" is shown on the arduino serial monitor and environmental data begins to appear. "ESP32_enviroment_sensor"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");


If the you encounter an error using I2C communication protocol for the BME, try switching the address from  0x76 to  0x77 at `status = bme.begin(0x77)`.

To switch to a Json output, remove
```char msg[200];
sprintf(msg, "%f,%f,%f,%f", temperature,pressure,altitude,humidity);
webSocket.sendTXT(0,msg);
```
and uncomment
```
//Using JSON format

//DynamicJsonDocument  doc(200);

//doc["Temperature"] = tempstr;
//  doc["Pressure"] = presstr;
//
//  doc["Pressure"] = altstr;
//
//  doc["Humidity"] = humstr;

//String output;
//serializeJson(doc, output);
//webSocket.sendTXT(0,output);
```

Works well with the python application [socscrollsave](https://github.com/damianjwilliams/socscrollsave), which allows real-time plotting of sensor data and saving data as a csv file.
See my [website](https://www.ephys.org/esp32) for examples.

## License
The project is distributed under MIT License

