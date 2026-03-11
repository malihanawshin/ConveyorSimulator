cmake_minimum_required(VERSION 3.16)

project(ConveyorSimulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(conveyor_core
    src/MachineController.cpp
    src/Conveyor.cpp
    src/Sensor.cpp
    src/SorterGate.cpp
    src/AlarmManager.cpp
    src/SimulationEngine.cpp
)

target_include_directories(conveyor_core PUBLIC include)

add_executable(conveyor_sim
    src/main.cpp
)

target_link_libraries(conveyor_sim PRIVATE conveyor_core)

add_executable(conveyor_tests
    tests/MachineControllerTests.cpp
    tests/SensorTests.cpp
    tests/AlarmManagerTests.cpp
    tests/ConveyorSystemTests.cpp
)

target_link_libraries(conveyor_tests
    PRIVATE
    conveyor_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(conveyor_tests)
