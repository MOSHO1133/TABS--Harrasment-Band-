#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Function prototypes
void sendCommand(String command, const int timeout, boolean debug);
void readSMS(int index);
void sendSMS(String number, String text);
void sendGPSLocation(String number);

// Define the hardware serial port for SIM800L
#define SIM800LSerial Serial2

// Define the baud rate for SIM800L
#define SIM800L_BAUD 9600

// Define the serial port connected to the GPS module
#define GPS_SERIAL_PORT Serial1

// Define the baud rate of the GPS module
#define GPS_BAUD 9600

// Define the GPS data parser object
TinyGPSPlus gps;
//==========================================================
// Define the phone number to send the response to
#define RESPONSE_NUMBER "+92##########"
//==========================================================
// Define the response message
#define RESPONSE_MESSAGE "Hello"

// Define the interval for sending location (5 minutes in milliseconds)
#define LOCATION_SEND_INTERVAL 300000

unsigned long lastLocationSendTime = 0;

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  SIM800LSerial.begin(SIM800L_BAUD, SERIAL_8N1, 4, 2); // Initialize SIM800L hardware serial
  GPS_SERIAL_PORT.begin(GPS_BAUD, SERIAL_8N1, 16, 17); // Initialize GPS serial port
  delay(30000); // Allow time for the SIM800L module to initialize
  Serial.println("Initializing SIM800L...");

  // Send initialization commands to SIM800L
  sendCommand("AT", 1000, true); 
  sendCommand("AT+CMGF=1", 1000, true); 
  sendCommand("AT+CNMI=1,2,0,0,0", 1000, true); 

  Serial.println("Setup complete.");
}

void loop() {
  if (SIM800LSerial.available()) {
    String message = SIM800LSerial.readString();
    Serial.println("Received from SIM800L: " + message);

    if (message.indexOf("+CMT:") != -1) {
      int index = message.substring(message.lastIndexOf(",") + 1).toInt();
      readSMS(index);

      if (message.indexOf(RESPONSE_NUMBER) != -1 &&
          message.indexOf(RESPONSE_MESSAGE) != -1) {

        if (gps.location.isValid()) {
          sendGPSLocation(RESPONSE_NUMBER);
        }
      }
    }
  }

  while (GPS_SERIAL_PORT.available() > 0) {
    gps.encode(GPS_SERIAL_PORT.read());
  }

  if (gps.location.isValid()) {
    if (lastLocationSendTime == 0 ||
        millis() - lastLocationSendTime >= LOCATION_SEND_INTERVAL) {

      sendGPSLocation(RESPONSE_NUMBER);
      lastLocationSendTime = millis();
    }
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("sendSMS")) {
      int firstQuote = command.indexOf('\"');
      int secondQuote = command.indexOf('\"', firstQuote + 1);
      int thirdQuote = command.indexOf('\"', secondQuote + 1);
      int fourthQuote = command.indexOf('\"', thirdQuote + 1);

      String number = command.substring(firstQuote + 1, secondQuote);
      String text = command.substring(thirdQuote + 1, fourthQuote);

      sendSMS(number, text);
    } else {
      sendCommand(command, 1000, true);
    }
  }
}

void sendCommand(String command, const int timeout, boolean debug) {
  SIM800LSerial.println(command);
  delay(timeout);
  if (debug) {
    while (SIM800LSerial.available()) {
      Serial.write(SIM800LSerial.read());
    }
    Serial.println();
  }
}

void readSMS(int index) {
  sendCommand("AT+CMGR=" + String(index), 1000, true);
}

void sendSMS(String number, String text) {
  sendCommand("AT+CMGS=\"" + number + "\"", 1000, true);
  delay(1000);
  SIM800LSerial.print(text);
  SIM800LSerial.write(26);
}

void sendGPSLocation(String number) {
  if (gps.location.isValid()) {
    String url = "http://maps.google.com/maps?q=" +
                 String(gps.location.lat(), 6) + "," +
                 String(gps.location.lng(), 6);
    sendSMS(number, url);
  }
}