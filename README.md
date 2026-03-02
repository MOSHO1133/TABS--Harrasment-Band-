TABS – Tragedy Assistance Band System

TABS (Tragedy Assistance Band System) is a GSM + GPS–based emergency response system designed to transmit real-time location alerts via SMS when triggered by an authorized command. The system is built for personal safety and emergency scenarios, where rapid and reliable location sharing can be critical.

TABS integrates a SIM800L GSM module, a GPS receiver, and a microcontroller to automatically respond with live GPS coordinates formatted as a Google Maps link, ensuring easy and universal access to location data.

🚀 Features

📡 Real-time GPS tracking using TinyGPS++

📱 SMS-based command and response mechanism

🌍 Automatic Google Maps location sharing

🔁 Periodic location updates (default: every 5 minutes)

🔐 Authorized number filtering for secure operation

🧪 Serial debugging and monitoring support

⚙️ Modular and extensible code architecture

🛠️ Hardware Requirements

Microcontroller with multiple hardware serial ports (e.g., ESP32)

SIM800L GSM module

GPS module (NEO-6M or equivalent)

SIM card with SMS capability

External power supply (recommended for SIM800L current stability)

GSM and GPS antennas

📚 Software Requirements

Arduino IDE

Required libraries:

TinyGPS++

HardwareSerial (built-in)

🔌 Pin Configuration
Module	TX Pin	RX Pin
SIM800L	GPIO 4	GPIO 2
GPS	GPIO 16	GPIO 17

⚠️ Pin assignments can be modified in the source code to match your hardware configuration.

⚙️ System Workflow

The system initializes the SIM800L module and configures it for SMS text mode.

The GPS module continuously feeds location data to the TinyGPS++ parser.

When an SMS containing the predefined trigger keyword is received from an authorized number, the system:

Verifies GPS data availability

Sends back a Google Maps link containing the current coordinates

If GPS data remains valid, the system automatically sends location updates at fixed intervals.

📩 SMS Command Logic

Trigger Message

Hello

Authorized Number

+92XXXXXXXXXX

Only SMS messages that match both the trigger keyword and the authorized sender number will receive a response.

🌍 Location Message Format

The response SMS contains a Google Maps URL:

http://maps.google.com/maps?q=<latitude>,<longitude>

Opening the link displays the exact location directly in Google Maps.

🧪 Debugging & Testing

Open Serial Monitor at 9600 baud

Monitor:

SIM800L AT command responses

GPS fix and signal status

SMS send/receive logs

Manual AT commands can also be sent through the Serial Monitor for testing.

⏱️ Location Update Interval

Location updates are sent every 5 minutes by default:

#define LOCATION_SEND_INTERVAL 300000

This value can be adjusted according to application requirements.

🔒 Safety & Reliability Notes

Ensure stable power delivery to the SIM800L module (it draws high peak current).

Use appropriate antennas for reliable GSM and GPS communication.

GPS performance requires a clear view of the sky for accurate location fixes.

📈 Planned Enhancements

Physical panic button / wearable trigger

AI-based anxiety or distress detection

Encrypted SMS communication

Mobile application integration

Cloud backend for location history and analytics

Miniaturized PCB design for wearable deployment

👨‍💻 Author

Muhammad Shees
BSCS Student
Project: TABS – Tragedy Assistance Band System

📄 License

This project is licensed under the MIT License.
See the LICENSE file for full license details.
