TABS – Tragedy Assistance Band System

TABS (Tragedy Assistance Band System) is a GSM + GPS–based emergency response system designed to send real-time location alerts via SMS when triggered by an authorized message. The system is intended for personal safety applications, particularly for harassment or emergency scenarios, where fast and reliable location sharing is critical.

This project integrates a SIM800L GSM module, a GPS receiver, and a microcontroller to automatically respond with live GPS coordinates formatted as a Google Maps link.

🚀 Features

📡 Real-time GPS tracking using TinyGPS++

📱 SMS-based command and response system

🌍 Automatic Google Maps location sharing

🔁 Periodic location updates (every 5 minutes)

🔐 Authorized number filtering for secure responses

🧪 Serial debugging support

⚙️ Modular and extensible code structure

🛠️ Hardware Requirements

Microcontroller with multiple hardware serial ports (e.g., ESP32)

SIM800L GSM module

GPS module (NEO-6M or equivalent)

SIM card with SMS support

External power supply (recommended for SIM800L stability)

Antennas for GSM and GPS

📚 Software Requirements

Arduino IDE

Required libraries:

TinyGPS++

HardwareSerial (built-in)

🔌 Pin Configuration
Module	TX Pin	RX Pin
SIM800L	GPIO 4	GPIO 2
GPS	GPIO 16	GPIO 17

⚠️ Pins can be modified in the code to match your hardware layout.

⚙️ How It Works

The system initializes the SIM800L and configures it for SMS text mode.

The GPS module continuously feeds location data to the TinyGPS++ parser.

When an SMS containing the predefined keyword (Hello) is received from the authorized number, the system:

Validates GPS availability

Sends back a Google Maps location link

The system also sends location updates automatically every 5 minutes when valid GPS data is available.

📩 SMS Command Logic

Trigger Message:

Hello

Authorized Number:

+92############

Only messages matching both the content and sender number will trigger a response.

🌍 Location Message Format

The response SMS contains a Google Maps link:

http://maps.google.com/maps?q=<latitude>,<longitude>

This link opens directly in Google Maps on any smartphone.

🧪 Debugging & Testing

Open Serial Monitor at 9600 baud

Monitor:

SIM800L responses

GPS fix status

SMS send/receive logs

Manual AT commands can be sent through the Serial Monitor.

⏱️ Location Update Interval

The system automatically sends the GPS location every 5 minutes:

#define LOCATION_SEND_INTERVAL 300000

You can modify this value as needed.

🔒 Safety & Reliability Notes

Ensure stable power delivery to SIM800L (current spikes can reset it).

Use antennas for reliable GSM/GPS performance.

GPS requires clear sky view for accurate location fixes.

📈 Future Improvements (Planned)

Panic button / wearable trigger

AI-based anxiety or distress detection

Encrypted SMS communication

Mobile application integration

Cloud backend for tracking history

Miniaturized PCB design for wearable form factor

👨‍💻 Author

Syed Muhammad Khizer Haider
BSCS – Batch 23 (NBC)

📄 License

This project is provided for educational and research purposes.
Commercial use requires proper testing, certification, and compliance with local regulations.
