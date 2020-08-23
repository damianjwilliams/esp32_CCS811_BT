# Graphical User Interface for the Bluetooth sensor

# Imports

import serial
from tkinter import *

# Serial port parameters
serial_speed = 115200
serial_port = 'COM4'



ser = serial.Serial(serial_port, serial_speed, timeout=1)


# Main Tkinter application
class Application(Frame):

    # Measure data from the sensor
    def measure(self):
        # Request data and read the answer
        ser.write("q".encode('utf_8'))

        data = ser.readline()
        data = data.decode('utf_8')
        print(data)

        if (data != ""):
            processed_data = data.split(",")

            temperature = float(processed_data[0].strip())
            temperature = round(temperature,1)

            self.temp_data.set("Temperature (C):\t " + str(temperature))
            self.temperature.pack()

            co2 = float(processed_data[1].strip())
            co2 = round(co2)

            self.co2_data.set("CO2 (ppm):\t" + str(co2))
            self.co2.pack()

            humidity = float(processed_data[3].strip())
            humidity = round(humidity)

            self.hum_data.set("Humidity (%):\t" + str(humidity))
            self.humidity.pack()

            pressure = float(processed_data[2].strip())
            pressure = (round(pressure/10))*10

            self.pressure_data.set("Pressure (Pa):\t" + str(pressure))
            self.pressure.pack()
        
            tvoc = float(processed_data[4].strip())
            tvoc = (round(tvoc/10))*10
            
            self.tvoc_data.set("TVOCs (ppb):\t" + str(tvoc))
            self.tvoc.pack()

        # Wait 0.5 second between each measurement
        self.after(500, self.measure)

    # Create display elements
    def createWidgets(self):
        self.temperature = Label(self, textvariable=self.temp_data, font=('Verdana', 10, 'bold'))
        self.temp_data.set("Temperature")
        self.temperature.pack()

        self.humidity = Label(self, textvariable=self.hum_data, font=('Verdana', 10, 'bold'))
        self.hum_data.set("Humidity")
        self.humidity.pack()

        self.pressure = Label(self, textvariable=self.pressure_data, font=('Verdana', 10, 'bold'))
        self.pressure_data.set("Pressure")
        self.pressure.pack()

        self.co2 = Label(self, textvariable=self.co2_data, font=('Verdana', 10, 'bold'))
        self.co2_data.set("CO2")
        self.co2.pack()

        self.tvoc = Label(self, textvariable=self.tvoc_data, font=('Verdana', 10, 'bold'))
        self.tvoc_data.set("TVOCs")
        self.tvoc.pack()



    # Init the variables & start measurements
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.temp_data = StringVar()
        self.hum_data = StringVar()
        self.pressure_data = StringVar()
        self.co2_data = StringVar()
        self.tvoc_data = StringVar()
        self.createWidgets()
        self.pack()
        self.measure()


# Create and run the GUI
root = Tk()
root.title('Enviroment measurments')
app = Application(master=root)
app.mainloop()
