# ESP01 DHT11 Monitor

Lê temperatura e umidade de um DHT11 em um ESP8266 ESP-01 e publica os valores no ThingSpeak.

## Destaques

- Monitoramento em tempo real de temperatura e umidade do sensor DHT11
- Conectividade WiFi com reconexão automática no ESP-01
- Transmissão de dados para a plataforma de nuvem ThingSpeak
- Saída serial de debug em 115200 baud
- Monitoramento de uso de memória via heap livre
- Tratamento de erros para leituras do sensor e conectividade de rede

## Pré-requisitos

### Requisitos de Hardware

- Módulo ESP8266 ESP-01 (1MB flash)
- Sensor de temperatura e umidade DHT11
- Resistor pull-up (4.7kΩ - 10kΩ) para a linha de dados do DHT11
- Fonte de alimentação 3.3V
- Adaptador USB-Serial para programação

### Conexão do Circuito

```
ESP01 Pin 2 (GPIO2) ─────[4.7kΩ]───── 3.3V
                    │
                    └─── Pino de Dados DHT11
DHT11 VCC ──────────────────────────── 3.3V
DHT11 GND ──────────────────────────── GND
```

### Requisitos de Software

- **PlatformIO** — necessário para compilar e gravar; instale com `make install-pio`
- ESP8266 Arduino Core (instalado automaticamente pelo PlatformIO)
- Biblioteca de sensor DHT (declarada em `platformio.ini`)

## Instalação

### Compilar a partir do Código-Fonte

```bash
git clone https://github.com/carlosrabelo/esp01dht11.git
cd esp01dht11
cp src/secret.h.template src/secret.h
cp .env.example .env
```

Edite `src/secret.h` com suas credenciais:

```cpp
const char* ssid     = "sua_rede_wifi";
const char* password = "sua_senha_wifi";
const char* privateKey = "sua_chave_api_escrita_thingspeak";
```

Configure o ThingSpeak:

- Crie uma conta gratuita no [ThingSpeak](https://thingspeak.com)
- Crie um novo canal com dois campos:
  - Campo 1: Temperatura (°C)
  - Campo 2: Umidade (%)
- Copie a Chave de API de Escrita para `src/secret.h`

```bash
make build
```

## Uso

### Compilar e Enviar

```bash
make build      # Compile firmware
make upload     # Upload to ESP-01 (device must be in programming mode)
make monitor    # Open serial monitor at 115200 baud
make flash      # Compile and upload
```

### Saída do Monitor Serial

O dispositivo imprime informações de debug na porta serial em 115200 baud:

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

## Configuração

### Intervalo de Leitura

O intervalo de leitura do sensor é 60 segundos por padrão. Altere `READING_INTERVAL` em `src/main.cpp`:

```cpp
#define READING_INTERVAL 60000 // milliseconds
```

### Pino DHT11

O pino de dados do DHT11 é GPIO2 (pino 2 do ESP-01) por padrão. Para alterá-lo:

```cpp
#define DHT_PIN 2 // GPIO pin number
```

### Porta Serial

Copie `.env.example` para `.env` e defina a porta USB-serial:

```bash
cp .env.example .env
# Edit .env to set your port (e.g., /dev/ttyUSB0)
```

Ou deixe o Make detectar:

```bash
make detect-port
```

## Estrutura do Projeto

```
src/             # Firmware source (PlatformIO requires this)
.make/           # PlatformIO wrapper scripts
platformio.ini   # Board and library configuration
.env.example     # Port and speed overrides template
```

## Desenvolvimento

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

## Licença

Este projeto está licenciado sob a GNU General Public License v2 — veja [LICENSE](LICENSE) para detalhes.
