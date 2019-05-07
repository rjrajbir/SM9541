// This code is designed to work with the TEMPERATURE COMPENSATED PRESSURE SENSOR -0.5 TO 10 CMH₂O ±1.5 PSI I2C Mini Module 


#include <Wire.h>

//P_min = -5              Full Scale Minimum (for SM9541-100C-S-C-3-S)
//P_max = 100             Full Scale Maximum (for SM9541-100C-S-C-3-S)
//T_min = -5
//T_max = 65
//Digoutp_min = 1638      Pressure Output Minimum
//Digoutp_max = 14745     Pressure Output Maximal

/*The current pressure in PSI (or bar) is calculated from the digital pressure value using the following formulas:

Sensp = (Digoutp_max - Digoutp_min)/(P_max - P_min)

P=(Digoutp(P)-Digoutp_min)/(Sensp) + P_min


The current sensor temperature in °C is computed from the digital temperature value analogous to the above
formulas by replacing the character "p" with a "T

*/

// SM9541 I2C address is 0x28(40)
#define Addr 0x28

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);
}

void loop()
{
  unsigned int data[4];
  delay(500);

  // Request 4 bytes of data
  Wire.requestFrom(Addr, 4);

  // Read 4 bytes of data
  // pressure msb, pressure lsb, temp msb, temp lsb
  if (Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }

  // Convert the data
  float pressure = ((data[0] & 0xFF) * 256 + (data[1] & 0xFF));
  float temp = ((data[2] & 0xFF) * 256 + (data[3] & 0xFF));

  pressure = ((pressure - 1638.0) / ((13107.0) / 105.0)) - 5.0;
  float cTemp = ((temp - 1638.0) / ((13107.0) / 70.0)) - 5.0;
  float fTemp = (cTemp * 1.8 ) + 32;

  // Output data to serial monitor
  Serial.print("Pressure : ");
  Serial.print(pressure);
  Serial.println(" PSI");
  Serial.print("Temperature in Celsius : ");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit : ");
  Serial.print(fTemp);
  Serial.println(" F");
  delay(500);
}
