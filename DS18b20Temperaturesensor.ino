// Include the required Arduino libraries:
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  // Fetch the temperature in degrees Celsius for device index:
  float tempC = sensors.getTempCByIndex(0);

  // Print the temperature in Celsius in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");

  // Wait 2 seconds:
  delay(2000);
}
