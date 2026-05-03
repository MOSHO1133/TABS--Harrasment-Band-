# TABS — Tragedy Assistance Band System

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32-red?style=flat-square&logo=espressif" />
  <img src="https://img.shields.io/badge/GSM-SIM800L-blue?style=flat-square" />
  <img src="https://img.shields.io/badge/GPS-NEO--6M-green?style=flat-square" />
  <img src="https://img.shields.io/badge/Arduino-IDE-teal?style=flat-square&logo=arduino" />
  <img src="https://img.shields.io/badge/Protocol-SMS%20%2F%20AT%20Commands-orange?style=flat-square" />
  <img src="https://img.shields.io/badge/License-MIT-lightgrey?style=flat-square" />
</p>

A **GSM + GPS–based emergency response system** designed to transmit real-time location alerts via SMS when triggered by an authorized command. Built for personal safety and emergency scenarios where rapid, reliable location sharing can be the difference between life and death.

TABS integrates a **SIM800L GSM module**, a **NEO-6M GPS receiver**, and an **ESP32 microcontroller** to automatically respond with live GPS coordinates formatted as a Google Maps link — ensuring instant, universal access to location data from any mobile device, with no app required.

---

## 📸 Hardware Preview

> ESP32 · SIM800L · NEO-6M GPS · Assembled Prototype

---

## 🧠 What is TABS?

TABS is a hardware-software co-design emergency system that:

| Layer | Component | Role |
|-------|-----------|------|
| Microcontroller | ESP32 | System brain — runs all logic, manages serial buses |
| GSM Module | SIM800L | Sends and receives SMS via AT commands |
| GPS Module | NEO-6M (or equivalent) | Provides real-time latitude/longitude fix |
| Parsing Library | TinyGPS++ | Decodes raw NMEA sentences into usable coordinates |
| Communication | SMS (GSM network) | Trigger input + location output — no internet needed |
| Location Format | Google Maps URL | Universal, one-tap navigation from any phone |

The system continuously parses GPS data in the background. When an SMS containing the trigger keyword arrives from an authorized number, TABS immediately responds with the current coordinates and continues sending periodic updates every 5 minutes.

---

## 🚀 Features

- 📡 **Real-time GPS tracking** using TinyGPS++ library
- 📱 **SMS-based command and response** — no app, no internet required
- 🌍 **Automatic Google Maps link** in every response SMS
- 🔁 **Periodic location updates** every 5 minutes (configurable)
- 🔐 **Authorized number filtering** — only trusted senders get a response
- 🧪 **Serial debugging** — full AT command and GPS log monitoring at 9600 baud
- ⚙️ **Modular architecture** — easy to extend with new triggers or output channels

---

## 🛠️ Hardware Requirements

| Component | Description |
|-----------|-------------|
| **Microcontroller** | ESP32 (multiple hardware serial ports required) |
| **GSM Module** | SIM800L with GSM antenna |
| **GPS Module** | NEO-6M or equivalent with GPS antenna |
| **SIM Card** | Any carrier with SMS capability |
| **Power Supply** | External regulated supply recommended for SIM800L (peak current ~2A) |
| **Antennas** | Dedicated GSM + GPS antennas for reliable signal |

> ⚠️ **Power Note:** The SIM800L draws up to **2A peak** during transmission. A USB port alone is insufficient — use a dedicated 3.7–4.2V regulated supply with adequate current capacity.

---

## 📚 Software Requirements

| Tool / Library | Purpose |
|---------------|---------|
| **Arduino IDE** | Flashing firmware to ESP32 |
| **TinyGPS++** | NMEA sentence parsing → lat/lng coordinates |
| **HardwareSerial** | Built-in ESP32 library for dual serial bus management |

### Install TinyGPS++

In Arduino IDE:
```
Tools → Manage Libraries → Search "TinyGPSPlus" → Install
```

---

## 🔌 Pin Configuration

| Module | ESP32 TX Pin | ESP32 RX Pin |
|--------|-------------|-------------|
| **SIM800L** | GPIO 4 | GPIO 2 |
| **GPS (NEO-6M)** | GPIO 16 | GPIO 17 |

> ⚠️ Pin assignments can be modified in the source code to match your specific wiring.

```cpp
// In source code — modify as needed
HardwareSerial sim800(1);   // SIM800L on Serial1
HardwareSerial gps(2);      // GPS on Serial2

sim800.begin(9600, SERIAL_8N1, 2, 4);    // RX=2, TX=4
gps.begin(9600, SERIAL_8N1, 17, 16);     // RX=17, TX=16
```

---

## ⚙️ System Workflow

```
┌─────────────────────────────────────────────────────┐
│                    ESP32 Boot                        │
│   Initialize SIM800L → Configure SMS Text Mode      │
│   Initialize GPS → Begin NMEA parsing loop          │
└──────────────────────┬──────────────────────────────┘
                       │
          ┌────────────▼────────────┐
          │   Continuous GPS Parse  │◄── NMEA sentences from NEO-6M
          │   (background loop)     │
          └────────────┬────────────┘
                       │
          ┌────────────▼────────────┐
          │  Incoming SMS Received? │
          └────────────┬────────────┘
                       │
            ┌──────────▼──────────┐
            │ From authorized     │
            │ number?  +  keyword │
            │ matches trigger?    │
            └──────────┬──────────┘
                       │ YES
          ┌────────────▼────────────┐
          │  GPS fix available?     │
          └────────────┬────────────┘
                       │ YES
          ┌────────────▼────────────┐
          │  Send Google Maps SMS   │
          │  Reply to sender        │
          └────────────┬────────────┘
                       │
          ┌────────────▼────────────┐
          │  Start periodic timer   │
          │  (every 5 minutes)      │
          │  Re-send updated coords │
          └─────────────────────────┘
```

---

## 📩 SMS Command Logic

### Trigger Configuration

| Parameter | Value |
|-----------|-------|
| **Trigger Keyword** | `Hello` |
| **Authorized Number** | `+92XXXXXXXXXX` |
| **Response** | Google Maps URL with live coordinates |

Only SMS messages that **both** match the trigger keyword **and** originate from the authorized number will receive a location response. All other messages are silently ignored.

```cpp
#define TRIGGER_KEYWORD    "Hello"
#define AUTHORIZED_NUMBER  "+92XXXXXXXXXX"
```

---

## 🌍 Location Message Format

Every response SMS contains a direct Google Maps link:

```
http://maps.google.com/maps?q=<latitude>,<longitude>
```

**Example:**
```
http://maps.google.com/maps?q=31.5204,74.3587
```

Opening the link on any phone immediately shows the exact location pinned in Google Maps — no app installation, no account required.

---

## ⏱️ Location Update Interval

After the initial trigger response, TABS automatically sends updated location every **5 minutes**:

```cpp
#define LOCATION_SEND_INTERVAL 300000   // milliseconds (5 minutes)
```

Adjust this value to suit your application:

| Use Case | Recommended Interval |
|----------|---------------------|
| High urgency (active emergency) | 60,000 ms (1 minute) |
| Standard tracking | 300,000 ms (5 minutes) |
| Low power / battery conservation | 600,000 ms (10 minutes) |

---

## 🧪 Debugging & Testing

Open **Serial Monitor** in Arduino IDE at **9600 baud** to monitor:

| Log Type | What You'll See |
|----------|----------------|
| SIM800L init | AT command responses (`OK`, `ERROR`) |
| SMS reception | Sender number, message content |
| GPS status | Fix acquired / waiting for fix |
| SMS sent | Confirmation log with coordinates |

### Manual AT Command Testing

With the Serial Monitor open, you can send AT commands directly to the SIM800L for hardware verification:

```
AT                  → OK  (module alive)
AT+CMGF=1           → OK  (set SMS text mode)
AT+CSQ              → Signal quality (0–31)
AT+CREG?            → Network registration status
AT+CMGS="+92XXXXXXXXXX"   → Send a test SMS
```

---

## 🔒 Safety & Reliability Notes

| Concern | Recommendation |
|---------|---------------|
| **SIM800L power** | Use a dedicated regulated 3.7–4.2V supply, minimum 2A capacity |
| **GSM antenna** | Use the supplied stub antenna or an external 900/1800 MHz antenna |
| **GPS antenna** | Ceramic patch antenna — must have clear view of sky |
| **GPS cold start** | First fix can take 30–60 seconds outdoors; keep outdoors during testing |
| **Network coverage** | SIM800L supports 2G (GSM) only — verify 2G coverage in your area |

---

## 📈 Planned Enhancements

- [ ] **Physical panic button** — wearable hardware trigger (no phone needed)
- [ ] **AI-based distress detection** — anxiety / fall detection via sensors
- [ ] **Encrypted SMS communication** — end-to-end location security
- [ ] **Mobile application** — companion app for real-time map tracking
- [ ] **Cloud backend** — location history, analytics, alert logging
- [ ] **Miniaturized PCB** — custom board design for wearable deployment
- [ ] **Multi-recipient alerts** — broadcast to multiple trusted contacts
- [ ] **Low-power sleep mode** — extend battery life between triggers

---

## ⚡ Quick Start

### 1. Wire the hardware

Connect SIM800L and NEO-6M GPS to ESP32 per the pin table above.

### 2. Install dependencies

In Arduino IDE:
```
Tools → Manage Libraries → Install: TinyGPSPlus
```
Add ESP32 board support if not already installed:
```
File → Preferences → Additional Boards URL:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

### 3. Configure credentials

Open the source file and set your authorized number and trigger keyword:

```cpp
#define TRIGGER_KEYWORD    "Hello"
#define AUTHORIZED_NUMBER  "+92XXXXXXXXXX"   // your number with country code
```

### 4. Flash to ESP32

```
Select board: ESP32 Dev Module
Select port:  COMx (Windows) or /dev/ttyUSBx (Linux/Mac)
Upload
```

### 5. Test

1. Open Serial Monitor at 9600 baud
2. Take the device outdoors — wait for GPS fix (green LED or serial log)
3. Send `Hello` from your authorized number to the SIM card's number
4. You should receive a Google Maps link within seconds

---

## 🏛️ Architecture

```
Authorized Phone
      │
      │  SMS: "Hello"
      ▼
SIM800L GSM Module
      │
      │  AT+CMGR (read SMS)
      ▼
ESP32 Microcontroller
      │
      ├── Verify sender number
      ├── Verify trigger keyword
      ├── Read GPS (TinyGPS++)
      │         │
      │         └── NEO-6M GPS Module
      │                  │
      │                  └── NMEA data stream (lat, lng)
      │
      ├── Build Google Maps URL
      │
      └── AT+CMGS (send SMS reply)
              │
              ▼
      Authorized Phone receives:
      "http://maps.google.com/maps?q=31.52,74.35"
```

---

## 👨‍💻 Author

**Muhammad Shees**
BSCS Student
- GitHub: [@MOSHO1133](https://github.com/MOSHO1133)

---

## 📄 License

MIT — see [LICENSE](LICENSE)

---

*Built with ESP32 + SIM800L + NEO-6M · TABS – Tragedy Assistance Band System · 2026*