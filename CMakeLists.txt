# Fix For MACOS compability
if (APPLE)
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
endif()

PROJECT(STM32F413H-Discovery-TensorFlow-Demo LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
ENABLE_LANGUAGE(ASM)

SET(TARGET stm32f4HAL)
ADD_DEFINITIONS(-DDEBUG_UART_HANDLE=UartHandle)
#SET(TOOLCHAIN_PREFIX "/usr/local") # Set depending on System (f.e. Homebrew)

# Remove -rdynamic option from linking that is not supported by arm-none-eabi
# Also remove "undefined reference to `_sbrk'" error
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-specs=nano.specs -specs=nosys.specs")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})
SET(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -Wall -mfpu=fpv4-sp-d16 -Wl,-Map=output.map -Wl,--gc-sections -g3")

set(TFLM_MODES interpreter compiler)
set(TFLM_COMPILER_EXECUTABLE "/Users/Philipp/src/work/fp/git/tflite_micro_compiler_working/build/compiler") # TODO

set(STM32_CHIP STM32F413ZHT6)
set(STM_BOARD STM32F413H-Discovery)

set(TFLM_BOARD disco_f413zh)
#set(TFLM_MODE compiler)
set(TFLM_MODE interpreter)
#if(TFLM_MODE EQUAL compiler)
#    add_definitions(-DTFLM_MODE_COMPILER)
    #endif()

# Enable Benchmarking (TODO)
add_definitions(-DBENCHMARKING)

# Disable MBED (TODO)
add_definitions(-DNOMBED)

# Enable Fake Microphone
add_definitions(-DFAKE_MIC)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/gcc_stm32.cmake)
include(${CMAKE_TOOLCHAIN_FILE})

FIND_PACKAGE(Git REQUIRED)

set(CUBE_SOURCE "${CMAKE_BINARY_DIR}/STM32F4-Cube-Firmware")
if(EXISTS ${CUBE_SOURCE})
    message("Cube FW directory exists")
else()
    message("Getting Cube Firmware")
    set(GIT_REPOSITORY "https://github.com/alxhoff/STM32F4-Cube-Firmware.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone ${GIT_REPOSITORY} )
endif()
set(STM32Cube_DIR ${CUBE_SOURCE})

SET(TFLM_USE_CMSIS_NN ON)
FIND_PACKAGE(TFLite REQUIRED)

SET(TFLITE_PLATFORM micro)

set(STM32Cube_DIR ${CUBE_SOURCE})

#SET(FREERTOS_HEAP_IMPL 1)
#FIND_PACKAGE(FreeRTOS REQUIRED)
SET(STORAGE_DRIVER "SDCARD")
FIND_PACKAGE(FATFS REQUIRED)
FIND_PACKAGE(USBHost COMPONENTS MSC REQUIRED)

FIND_PACKAGE(CMSIS REQUIRED)
FIND_PACKAGE(CMSISNN REQUIRED)
FIND_PACKAGE(STM32HAL COMPONENTS sram gpio uart fmpi2c i2s dac adc dfsdm sd dma qspi REQUIRED)
FIND_PACKAGE(STM32LL COMPONENTS fsmc sdmmc REQUIRED)
FIND_PACKAGE(STM32BSP COMPONENTS ts lcd audio ili9325 stmpe811 st7789h2 ft6x06 wm8994 psram sd REQUIRED)
FIND_PACKAGE(TFLMExamples COMPONENTS micro_speech REQUIRED)

add_definitions(-DTFLM_EXAMPLE="${TFLM_EXAMPLE}" -DTFLM_BOARD="${TFLM_BOARD}")
set(TFLM_COMPILER_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Src/${TFLM_EXAMPLE}/")

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/Inc"
    "${PROJECT_SOURCE_DIR}/Inc/tests"
    ${TFLMExamples_INCLUDE_DIR}
    ${STM32HAL_INCLUDE_DIR}
    ${STM32LL_INCLUDE_DIR}
    ${CMSIS_INCLUDE_DIRS}
    ${CMSISNN_INCLUDE_DIRS}
    ${FreeRTOS_INCLUDE_DIRS}
    ${STM32BSP_INCLUDE_DIR}
    ${TFLite_INCLUDE_DIRS}
    ${FATFS_INCLUDE_DIRS}
    )

file(GLOB PROJECT_SOURCES
    "Src/*.c"
    "Src/*.cc"
    "Src/*.cpp"
    )

MESSAGE(STATUS "PROJECT_SOURCES: ${PROJECT_SOURCES}")

file(GLOB TEST_SOURCES
    "Src/tests/*.c"
    "Src/tests/*.cc"
    "Src/tests/*.cpp"
    )

  MESSAGE(STATUS "TEST_SOURCES: ${TEST_SOURCES}")
SET(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})

ADD_EXECUTABLE(
    ${CMAKE_PROJECT_NAME}.elf
    ${PROJECT_SOURCES}
    ${TEST_SOURCES}
    ${TFLMExamples_SOURCES}
    ${STM32HAL_SOURCES}
    ${STM32BSP_SOURCES}
    ${STM32LL_SOURCES}
    ${FreeRTOS_SOURCES}
    ${TFLite_SOURCES}
    ${CMSIS_SOURCES}
    ${CMSISNN_SOURCES}
    ${FATFS_SOURCES}
    )

TARGET_COMPILE_DEFINITIONS(${CMAKE_PROJECT_NAME}.elf PUBLIC STM_BSP)

ADD_CUSTOM_TARGET(
    flash
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
    COMMAND st-flash write ${CMAKE_PROJECT_NAME}.bin 0x8000000
    )

ADD_DEPENDENCIES(flash ${CMAKE_PROJECT_NAME}.elf)

set(OPENOCD_BOARD "board/st_nucleo_f4.cfg")

ADD_CUSTOM_TARGET(
    debug
    COMMAND openocd -f ${OPENOCD_BOARD} -c "program ${CMAKE_PROJECT_NAME}.elf reset exit" > /dev/null 2>&1
    COMMAND openocd -f ${OPENOCD_BOARD} > /dev/null 2>&1 & sleep 2
    COMMAND ${TARGET_TRIPLET}-gdb -quiet -tui -command=${CMAKE_CURRENT_LIST_DIR}/GDBCommands -se ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
    COMMAND killall -15 openocd
    )

ADD_DEPENDENCIES(debug ${CMAKE_PROJECT_NAME}.elf)

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}.elf)
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME}.elf)
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME}.elf)

# TODO: also clean .bin file on `make clean`

ADD_CUSTOM_TARGET(
    convert
    COMMAND test ! -z $$FILE || (echo 'Please define .tflite file using `make convert FILE=...`' && false)
    COMMAND test -f $$FILE || (echo 'File not found!' && false)
    COMMAND test -f ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc && cp ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc.old || :
    COMMAND test -f ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h && cp ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h.old || :
    COMMAND ${TFLM_COMPILER_EXECUTABLE} $$FILE ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc ${TFLM_EXAMPLE}_
    COMMAND mv ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc.h ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h
    )
