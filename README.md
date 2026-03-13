# Conveyor Simulator

Conveyor Simulator is a C++ desktop application that simulates a small conveyor-based sorting cell used in industrial automation. It combines core machine logic, configurable routing rules, automated tests, and a Qt-based Human-Machine Interaction (HMI) to demonstrate machine state handling, sensor monitoring, fault detection, and operator interaction.

## Overview

This project models a simple conveyor system with a controller, entry sensor, sorter gate, alarm handling, and a simulation engine. It also includes a Qt Widgets interface that presents machine overview data, operator controls, and timestamped alarm/event history in a dashboard-style layout inspired by industrial HMI screens.

## Features

- Conveyor control and machine state simulation.
- Config-driven routing using JSON.
- Sensor monitoring and blocked-duration tracking.
- Jam fault detection based on sensor blocked time.
- Alarm and event history with timestamps.
- Qt-based HMI with machine overview, operator controls, and history table.
- Unit tests with GoogleTest.
- Cross-platform CMake build setup.

## Machine Overview

The HMI displays key runtime information such as machine state, conveyor status, gate position, sensor state, blocked duration, and operating mode. These values are shown as color-coded status labels so the UI is easier to scan, which matches common HMI practice where operators need a quick overview of current conditions and abnormal states.

## Alarm and Event History

The application logs operator actions and machine events with timestamps in a history table. Alarm and event history is an important HMI feature because it supports troubleshooting, root-cause analysis, and review of abnormal machine behavior after a fault occurs.

## Tech Stack

- C++17
- Qt 6 Widgets
- CMake
- GoogleTest
- nlohmann/json

The project uses CMake `FetchContent` to bring in dependencies such as GoogleTest and nlohmann/json, and links JSON support through the `nlohmann_json::nlohmann_json` target. [web:113][web:156][web:588]

## Build Requirements

Before building, make sure you have:

- CMake 3.16 or newer
- A C++17 compiler
- Qt 6 with Widgets
- Internet access during the first CMake configure step for FetchContent dependencies

Qt’s CMake workflow for widget applications relies on `find_package(Qt6 REQUIRED COMPONENTS Widgets)` and standard CMake target setup.

## Build Instructions

### macOS example

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/macos
cmake --build build
```

### Run the console simulator

```bash
./build/conveyor_sim
```

### Run the GUI

If built as a normal executable:

```bash
./build/conveyor_gui
```

## Running Tests

```bash
ctest --test-dir build --output-on-failure
```

This project uses GoogleTest with CMake test discovery, which is a common pattern for C++ unit testing in CMake-based projects.

## Simulation Concepts

### Sensor Status

The sensor status label shows whether the sensor is currently blocked.

### Blocked Duration

The blocked duration label shows how long the sensor has remained blocked in simulation ticks. This is useful for fault detection because abnormal conditions are often identified not only by state, but by how long a signal remains active.

### Jam Detection

A jam is simulated when the sensor remains blocked longer than the configured threshold. In automation systems, timeout-based fault detection is a common strategy for detecting stuck parts or abnormal process conditions.

## Possible Improvements

- Auto and Manual operating modes
- Recipe selection from multiple JSON configurations
- Alarm acknowledgement workflow
- Cycle counters and production metrics
- More realistic conveyor animation
- Persistence for alarm/event history
- Trend charts for diagnostics

Modern HMI systems often expand beyond status display into counters, historical records, alarm filtering, and deeper diagnostic views.

## License

This project is available under the MIT License.
