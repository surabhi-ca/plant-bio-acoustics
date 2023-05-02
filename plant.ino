#include "DHT.h" 
#define DHTPIN 7 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27,16,2);
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int soundpin=6; 

#define waterSensPin A0 
const int soundSensorPin = A5; 
const float Vref = 5.0;  // Reference voltage of the sound sensor
const float SPLref = 94.0;  // Reference sound pressure level for calibration
int LDR = A4;          
int LED = 3;         
int LDRReading = 0;      // to store input value of LDR
int lEDBrightness = 0;   // to store the value of LED Brightness
int threshold_val = 200; // Check the threshold value based on the surrounding environment




void setup() {

Serial.begin(9600);
pinMode(soundpin,OUTPUT);//output of the buzzer(piezo-electric passive buzzer)

pinMode(LED, OUTPUT);
Serial.println(F("Experiment started!"));
dht.begin();//to start the temp-hum sensor

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Plant");
  
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("Bio Acoustics");

}


void loop() {

//temp and humidity
  delay(2000);//micro seconds
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print(F(" Humidity: "));
  Serial.println(h);
  Serial.print(F("Temperature: "));
  Serial.println(t);

//water level
  unsigned int waterSensorValue = analogRead(waterSensPin);
  Serial.print(F(" water_level_sensor_val: "));
  Serial.println(waterSensorValue);
  delay(2000);
  if(waterSensorValue >200)
    {
    tone(soundpin, 440,2000);}//440 is freq, 2000 is duration

//sound sensor
  int soundSensorValue=analogRead(soundSensorPin);
  Serial.print(F("sound sens: "));
  Serial.println(soundSensorValue);
  // Convert analog value to voltage
  float voltage = (float)soundSensorValue * Vref / 1023.0;
  Serial.print("voltage: ");
  Serial.println(voltage);

  // Convert voltage to dB
  float dB = 20 * log10(voltage / Vref) + SPLref;//Reference sound pressure level for calibration SPLref = 94.0 dB

  // Print dB to serial monitor
  Serial.print("dB: ");
  Serial.println(dB);
  

  
//lcd
lcd.clear();
lcd.backlight();
 lcd.setCursor(1,0);
  lcd.print("Temp: ") ;
  lcd.setCursor(7,0);  
  lcd.print(t);
 lcd.setCursor(1,1);
  lcd.print("Humidity: ") ;
  lcd.setCursor(10,1);  
  lcd.print(h); 
   

//ldr
  LDRReading = analogRead(LDR);    // Reading LDR Input.
  Serial.print("light intensity sens:");
  Serial.println(LDRReading);      // Printing LDR input value.
  
  if (LDRReading >threshold_val){                   // Condition to make LED ON. threshold val = 1000 units of intensity
  lEDBrightness = map(LDRReading, 0, 1023, 0, 255); // Converting LDR to LED Brightness.
  //convert ldr output that's ranging from 0-1023 into led's input ranging from 0-255
  analogWrite(LED, lEDBrightness); // Writing Brightness to LED.
  }
  else{
  analogWrite(LED, 0);             // If LDR is below threshold make LED OFF.
  }
  Serial.println(" ");

}
