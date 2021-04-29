#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define SERIESRESISTOR 560
#define SENSORPIN A0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Adafruit_MMA8451 mma = Adafruit_MMA8451();

int pump_pin = 12;
int water_pin = 9;
  
void setup() {

    Serial.begin(9600);
    mma.setRange(MMA8451_RANGE_2_G);
    sensors.begin();
    pinMode(pump_pin, OUPUT);
  
}

void loop() {
  
    /******* COOLING BLOCK *******/
    // Set maximum and minimum temperatures of the water
    int max_temp = 30;
    int min_temp = 18;

    // Retrieving Water Temperature
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);

    // Make water flow to cool off water
    if (temp >= max_temp){
        // Set pump to on to cool water
        digitalWrite(pump_pin, HIGH);
    } else if (temp <= temp_min){
        // Set pump to off for water cooling
        digitalWrite(pump_pin, LOW);
    }
  
    /******* INFLOW BLOCK *******/
    // Determine Volume of dog bowl in in^3
    float max_ratio =  0.85;
    float half_ratio = 0.5
    float top_radius = 4;
    float bottom_radius = 5;
    float height = 2.5;
    float total_volume = (1/3)*(sq(top_radius)
                        + top_radius*bottom_radius
                        + sq(bottom_radius))*height;

    // Value of the Liquid Level Meter
    float liquidLevelRead;
    liquidLevelRead = analogRead(SENSORPIN);

    // Convert Liquid Level Sensor Value to Resistance
    liquidLevelRead = (1023 / liquidLevelRead) - 1;
    liquidLevelRead = SERIESRESISTOR / liquidLevelRead;

    // To get level of water in bowl
    liquidLevelRead = liquidLevelRead / 150;

    float current_volume = (1/3)*(sq(top_radius) + top_radius*bottom_radius
                            + sq(bottom_radius))*liquidLevelRead;

    if ((current_volume / total_volume) <= half_ratio){
        // Pump on to add water
        digitalWrite(water_pin, HIGH);
    } else if ((current_volume / total_volume) >=  max_ratio){
        // Pump off to stop water inflow
        digitalWrite(water_pin, HIGH);
    }
    
    /******* ANTI-SPILL BLOCK *******/
    // Opening a channel for the Accelerometer to read data
    mma.read();

    // Sensor event catch
    sensors_event_t event; 
    mma.getEvent(&event);

    // Acceleration Values for each Cardinal Axis
    float x_accel = event.acceleration.x;
    float y_accel = event.acceleration.y;
    float z_accel = event.acceleration.z;

    // Only check values every 0.5 seconds
    delay(500);
 
}