cmake_minimum_required(VERSION 3.20)

# ZWINGEND VOR 'project()': Compiler-Prüfung für Mikrocontroller anpassen
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# 1. Projekt-Name und Sprache definieren
project(Wio_E5_Sensor C ASM)

# 2. Compiler-Flags für den STM32WLE5 (Cortex-M4, soft-float da keine Hardware-FPU)
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MCU_FLAGS} -Wall -O0 -g3")

# HIER ERGÄNZT: --specs=nosys.specs hinzugefügt, um die fehlenden Dummys wie _exit bereitzustellen
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MCU_FLAGS} --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections")

# 3. Globale Definitionen für die ST-Treiber
add_compile_definitions(STM32WLE5xx USE_HAL_DRIVER)

# 4. Quellcode-Dateien einbinden
file(GLOB_RECURSE SOURCES "src/*.c")

# 5. Das ausführbare Programm (Executable) erstellen
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
