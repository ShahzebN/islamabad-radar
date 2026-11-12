# islamabad-radar
IoT-based real-time ADS-B airspace radar dashboard powered by ESP32-S3 and cloud web architectures.
# 📡 IoT Real-Time Airspace Radar & Cloud Streamer

An enterprise-grade IoT-based air traffic surveillance and telemetry tracking system engineered around an **ESP32-S3 N16R8** microcontroller and a client-side cloud proxy web architecture.

![GitHub Top Language](https://img.shields.io/github/languages/top/ShahzebN/islamabad-radar?style=flat-square&color=58a6ff)
![License](https://img.shields.io/github/license/ShahzebN/islamabad-radar?style=flat-square&color=3fb950)

---

## 🚀 Project Overview
This system ingests high-frequency Automatic Dependent Surveillance-Broadcast (ADS-B) packets from public transponder feeds (`adsb.lol`), correlates callsigns against open-source aviation databases (`HexDB.io`) for route identification, resolves carrier branding via FlightAware CDNs, and streams real-time positional data, altitudes, ground speeds, and routing to a responsive, auto-refreshing web dashboard.

---

## 🛠️ Technical Stack & Architecture

* **Hardware Layer:** ESP32-S3 N16R8 microcontroller running FreeRTOS tasks and custom Wi-Fi socket handling. Optimized memory management by configuring PSRAM constraints and flash parameters.
* **Data Ingestion & Proxy Layer:** Custom HTTP client and server integration leveraging `WebServer.h` and `ArduinoJson`, utilizing client-side browser fetch configurations to bypass microcontroller memory bottlenecks.
* **Aviation & Route Intelligence:** Integrated real-time ICAO flight routing lookups with intelligent memory caching (`routeCache`) to minimize redundant queries and optimize latency.
* **Cloud Deployment:** Hosted statically via **GitHub Pages**, allowing global tracking independent of local hardware uptime.

---

## 📌 Future Hardware Iterations (Roadmap)

To evolve this project into a fully self-contained standalone desktop radar unit, upcoming development milestones include:
1. **Standalone TFT/OLED Hardware Display:** Interfacing an SPI-driven **ILI9341 2.8-inch TFT LCD** or **SSD1306 OLED module** directly to the ESP32-S3 to cycle through critical airspace stats locally.
2. **On-Device Data Caching (LittleFS):** Utilizing internal flash file systems for offline route-caching during intermittent network dropouts.
3. **Proximity Provisional Alarms:** Implementing active buzzers for localized tactical radius boundary alerts.

---

## 📄 License
Distributed under the **MIT License**. See `LICENSE` for more information.
