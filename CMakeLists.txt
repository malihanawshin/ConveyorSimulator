cmake_minimum_required(VERSION 3.16)

project(ConveyorSimulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(googletest json)

enable_testing()

add_library(conveyor_core
    src/MachineController.cpp
    src/Conveyor.cpp
    src/Sensor.cpp
    src/SorterGate.cpp
    src/AlarmManager.cpp
    src/ConfigLoader.cpp
    src/SimulationEngine.cpp
)

target_include_directories(conveyor_core PUBLIC include)

target_link_libraries(conveyor_core PUBLIC nlohmann_json::nlohmann_json)

add_executable(conveyor_sim
    src/main.cpp
)

target_link_libraries(conveyor_sim PRIVATE conveyor_core)

add_custom_command(TARGET conveyor_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config
    $<TARGET_FILE_DIR:conveyor_sim>/config
)


add_executable(conveyor_tests
    tests/MachineControllerTests.cpp
    tests/SensorTests.cpp
    tests/AlarmManagerTests.cpp
    tests/ConveyorSystemTests.cpp
    tests/SimulationEngineTests.cpp
)

target_link_libraries(conveyor_tests
    PRIVATE
    conveyor_core
    GTest::gtest_main
)

add_custom_command(TARGET conveyor_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config
    $<TARGET_FILE_DIR:conveyor_tests>/config
)


include(GoogleTest)
gtest_discover_tests(conveyor_tests)
