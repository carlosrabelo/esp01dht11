# ESP01 DHT11 Monitor

Reads temperature and humidity from a DHT11 on an ESP8266 ESP-01 and publishes the values to ThingSpeak.

## Highlights

- Real-time temperature and humidity monitoring from a DHT11 sensor
- WiFi connectivity with automatic reconnection on the ESP-01
- Data transmission to the ThingSpeak cloud platform
- Serial debug output at 115200 baud
- Memory usage monitoring via free heap tracking
- Error handling for sensor readings and network connectivity

## Prerequisites

### Hardware Requirements

- ESP8266 ESP-01 module (1MB flash)
- DHT11 temperature and humidity sensor
- Pull-up resistor (4.7kΩ - 10kΩ) for the DHT11 data line
- 3.3V power supply
- USB-to-Serial adapter for programming

### Circuit Connection

```
ESP01 Pin 2 (GPIO2) ─────[4.7kΩ]───── 3.3V
                    │
                    └─── DHT11 Data Pin
DHT11 VCC ──────────────────────────── 3.3V
DHT11 GND ──────────────────────────── GND
```

### Software Requirements

- **PlatformIO** — required to build and flash; install with `make install-pio`
- ESP8266 Arduino Core (installed automatically by PlatformIO)
- DHT sensor library (declared in `platformio.ini`)

## Installation

### Build from Source

```bash
git clone https://github.com/carlosrabelo/esp01dht11.git
cd esp01dht11
cp src/secret.h.template src/secret.h
cp .env.example .env
```

Edit `src/secret.h` with your credentials:

```cpp
const char* ssid     = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* privateKey = "your_thingspeak_write_api_key";
```

Set up ThingSpeak:

- Create a free account at [ThingSpeak](https://thingspeak.com)
- Create a new channel with two fields:
  - Field 1: Temperature (°C)
  - Field 2: Humidity (%)
- Copy the Write API Key into `src/secret.h`

```bash
make build
```

## Usage

### Build and Upload

```bash
make build      # Compile firmware
make upload     # Upload to ESP-01 (device must be in programming mode)
make monitor    # Open serial monitor at 115200 baud
make flash      # Compile and upload
```

### Serial Monitor Output

The device prints debug information on the serial port at 115200 baud:

```
=== ESP01 DHT11 ThingSpeak Monitor ===
Free heap: 45128
Chip ID: 12345678
Flash size: 4194304
Connecting to WiFi.....
WiFi connected! IP: 192.168.1.100
Signal strength: -45 dBm

--- Reading DHT11 sensor ---
Temperature: 23C, Humidity: 65%
Sending to ThingSpeak - Temp: 23, Humidity: 65
HTTP 200
Response: 1
Data sent successfully!
Free heap: 44856
Next reading in 60 seconds...
```

## Configuration

### Reading Interval

The sensor reading interval defaults to 60 seconds. Change `READING_INTERVAL` in `src/main.cpp`:

```cpp
#define READING_INTERVAL 60000 // milliseconds
```

### DHT11 Pin

The DHT11 data pin defaults to GPIO2 (ESP-01 pin 2). To change it:

```cpp
#define DHT_PIN 2 // GPIO pin number
```

### Serial Port

Copy `.env.example` to `.env` and set the USB-serial port:

```bash
cp .env.example .env
# Edit .env to set your port (e.g., /dev/ttyUSB0)
```

Or let Make detect it:

```bash
make detect-port
```

## Project Layout

```
src/             # Firmware source (PlatformIO requires this)
.make/           # PlatformIO wrapper scripts
platformio.ini   # Board and library configuration
.env.example     # Port and speed overrides template
```

## Development

```bash
make build        # Compile firmware
make upload       # Upload firmware to the device
make flash        # Compile and upload
make monitor      # Open serial monitor
make detect-port  # Auto-detect board USB port and save to .env
make test         # Run unit tests
make check        # Run static analysis
make deps         # Install dependencies
make clean        # Remove build artifacts
make erase        # Erase device flash memory
make check-pio    # Verify PlatformIO is installed
make install-pio  # Install PlatformIO
```

## License

This project is licensed under the GNU General Public License v2 — see [LICENSE](LICENSE) for details.
