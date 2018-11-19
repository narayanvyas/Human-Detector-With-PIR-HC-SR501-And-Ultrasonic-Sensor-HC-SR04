/*  Human Detector Within Specified Range Using PIR Motion Sensor And Ultrasonic Distance Sensor
 * 
 * GitHub URL - https://github.com/narayanvyas/Human-Detector-With-PIR-HC-SR501-And-Ultrasonic-Sensor-HC-SR04
 * 
 *  Developed By Web Dev Fusion
 *  URL - https://www.webdevfusion.com
 *  
 * Components
 * ----------
 *  - Arduino Uno
 *  - Infrared Motion Sensor
 *  - Ultrasonic Distance Sensor
 *  - An LED
 *  - A 220 Ohm resistor for the LED
 *  - A Buzzer
 *  
 *  Libraries
 *  ---------
 *  - None
 *
 * Connections
 * -----------
 *  Break out    |    Arduino Uno
 *  -----------------------------
 *      PIR VCC             |        5V
 *      PIR GND             |        GND
 *      PIR OUT             |        3
 *      Ultrasonic VCC      |        5V
 *      Ultrasonic GND      |        GND
 *      Ultrasonic Echo     |        12
 *      Ultrasonic Trig     |        13
 *      
 * Also connect an LED to simulate a controlled device. 
 *           220 Ohm
 *    3 ----/\/\/\/\----(LED |)----GND
 *  
 */
 
int ledPin   = 4;       // choose the pin for the LED
int inputPin = 3;       // choose the input pin (for PIR sensor)
int pirState = LOW;     // we start, assuming no motion detected
int val      = 0;       // variable for reading the pin status
int echoPin = 12;       // variable for echo pin of Distanse Sensor
int trigPin = 13;       // variable for trigger pin of Distanse Sensor
int buzzer = 8;         // choose pin for the buzzer

//the time we give the PIR Sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30; //This time is in seconds
 
void setup() {
  pinMode(ledPin, OUTPUT);     // declare LED as output
  pinMode(inputPin, INPUT);    // declare PIR Sensor as input
  pinMode(trigPin, OUTPUT);    // declare Distance Sensor's trigger pin as output
  pinMode(echoPin, INPUT);     // declare Distance Sensor's echo pin as input
  pinMode(buzzer, OUTPUT);     // declare buzzer as output
 
  Serial.begin(9600);
  
  Serial.println("Waiting for the PIR sensor to warm up.");
  Serial.println("PIR SENSOR ACTIVE");
}
long duration, distance;

long calculateDistance() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

void loop() {
  calculateDistance();
  if (distance >= 200 || distance <= 0){  // Maximum Distance Here We Are Taking Is 200 CM
    Serial.println("No Object Found");
  }
  else {
    val = digitalRead(inputPin);  // read input value
    Serial.println("Object Found");
//    delay(4000);
    if (val == HIGH) {            // check if the input is HIGH
      digitalWrite(ledPin, HIGH);  // turn LED ON
      tone(buzzer, 1000);
      delay(7000);
      digitalWrite(ledPin, LOW);
      noTone(buzzer);
      Serial.println("Alarm Off");
      if (pirState == LOW) {
        // we have just turned on
        Serial.print("Human detected at ");
        Serial.print(distance);
        Serial.println(" cm");
        // We only want to print on the output change, not state
        pirState = HIGH;
      }
    } else {
      digitalWrite(ledPin, LOW); // turn LED OFF
      noTone(buzzer);
      if (pirState == HIGH){
        // we have just turned off
        // We only want to print on the output change, not state
        pirState = LOW;
        noTone(buzzer);
      }
    }
  }
  delay(10);
}
