# Real-Time Cloud Data Pipeline & IoT Validation Engine
*An Interactive Digital System (IDS) designed to bridge the gap between regional meteorological data and local micro-climate data analytics.*

This project establishes an end-to-end data pipeline that ingests official API data, streams it via an MQTT broker using WebSockets, and processes it on an ESP32 edge device. The core architectural focus is enforcing strict data governance and multi-source validation to mitigate physical sensor unreliability.

---

## 🏗️ System Architecture & Data Flow

## System Architecture & Data Flow
[DMI Open Data API]
│  (Asynchronous HTTP GET Requests)
-->
[p5.js Cloud Ingestion Engine] (sketch.js)
│  (JSON Parsing & Payload Tokenization)
-->  (Secure WebSockets MQTT Stream - wss://)
[Shiftr.io MQTT Broker]
│  (Pub/Sub Topic: esp32IDS/DMI)
-->
[ESP32 Edge Processing Unit] (Temp_Humid_arduino.ino)
│  (Real-Time Boundary & Absolute Variance Validation vs. DHT11 Sensor)
-->
[Physical Hardware / UI] (WS2812B LED Strips via FastLED)

Det ser ud til, at der sneg sig en lille smule af vores chat-dialog og nogle rå formateringsfejl (dobbelte overskrifter og løse kodeblokke) med ind i din tekst, da du kopierede den.

Her er den absolut mest polerede, fejlfrie og fuldstændig rene version af din `README.md`. Al overflødig tekst er renset ud, diagrammet er sat i en korrekt kodeblok, så det renderes smukt på GitHub, og formateringen sidder lige i skabet.

Udskift hele indholdet i din `README.md` med dette stykke tekst:

```markdown
# Real-Time Cloud Data Pipeline & IoT Validation Engine
*An Interactive Digital System (IDS) designed to bridge the gap between regional meteorological data and local micro-climate data analytics.*

This project establishes an end-to-end data pipeline that ingests official API data, streams it via an MQTT broker using WebSockets, and processes it on an ESP32 edge device. The core architectural focus is enforcing strict data governance and multi-source validation to mitigate physical sensor unreliability.

---

## 🏗️ System Architecture & Data Flow


```

[DMI Open Data API]
│  (Asynchronous HTTP GET Requests)
▼
[p5.js Cloud Ingestion Engine] (sketch.js)
│  (JSON Parsing & Payload Tokenization)
▼  (Secure WebSockets MQTT Stream - wss://)
[Shiftr.io MQTT Broker]
│  (Pub/Sub Topic: esp32IDS/DMI)
▼
[ESP32 Edge Processing Unit] (Temp_Humid_arduino.ino)
│  (Real-Time Boundary & Absolute Variance Validation vs. DHT11 Sensor)
▼
[Physical Hardware / UI] (WS2812B LED Strips via FastLED)

```

---

## Resolving the Reliability Gap via Data Governance

Low-cost edge sensors, such as the DHT11, are inherently prone to environmental "noise" caused by wind fluctuations, localized heat interference, or hardware degradation. Conversely, while professional meteorological data from DMI (Station 06180, Kastrup Lufthavn) offers high data integrity, it represents a regional macro-climate rather than the specific local micro-climate at Roskilde University (RUC).

To resolve this reliability gap, this architecture does not rely blindly on a single data source. Instead, it treats both inputs as data vectors that must pass through a **dual-layered validation engine** before executing downstream commands:

1. **Boundary Filtering:** Evaluates incoming metrics against strict operational limits (filtering out extreme anomalies outside -30°C to 40°C based on historical regional limits).
2. **Absolute Variance Resolution:** Calculates the real-time absolute variance (`abs()`) between the local micro-climate (DHT11) and the regional cloud data (DMI). If the variance delta exceeds a strict limit ($\le 5$ units), it flags a potential anomaly and safely halts physical environment updates, isolating the hardware UI from unvalidated data drifts.

---

## Key Technical Achievements

### 🔹 Asynchronous Ingestion & API Integration (`sketch.js`)
- **Automated Harvester:** Built an interval-driven data harvester using JavaScript's asynchronous `async/await` syntax to concurrently query live metrics from the **DMI Open Data API**.
- **JSON Traversal:** Engineered logic to handle multi-layered API responses, dynamically extracting nested data points (`temp_dry` and `humidity`) from deep JSON object graphs.
- **Payload Tokenization:** Optimized network bandwidth by extracting relevant metrics and compressing them into a lightweight, comma-separated string payload prior to transmission.

### 🔹 Advanced Parsing & State Optimization (`Temp_Humid_arduino.ino`)
- **Asynchronous Callback Manager:** Implemented a token-based parsing mechanism using `indexOf` and `substring` to split, isolate, and cast raw incoming network strings into floating-point variables.
- **State-Tracking Efficiency:** Implemented delta-tracking variables (`lastTemp`, `lastHumid`) to evaluate changes in environment states, successfully preventing redundant hardware processing loops.

---

## 🛠️Tech Stack & Protocols
- **Data Ingestion:** JavaScript (ES6+), p5.js, Fetch API, JSON Frameworks.
- **Hardware & Processing:** ESP32 Microcontroller, DHT11 Sensor, WS2812B NeoPixel LED Strips.
- **Protocols & Communication:** MQTT (Pub/Sub architecture), Secure WebSockets (WSS), C++/Arduino.

```
