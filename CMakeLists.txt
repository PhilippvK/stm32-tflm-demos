# Fix For MACOS compability
if (APPLE)
    set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
    SET(TOOLCHAIN_PREFIX "/usr/local") # Set depending on System (f.e. Homebrew)
endif()

PROJECT(STM32F413H-Discovery-TensorFlow-Demo LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
ENABLE_LANGUAGE(ASM)

SET(TARGET stm32f4HAL)
ADD_DEFINITIONS(-DDEBUG_UART_HANDLE=UartHandle)

# Remove -rdynamic option from linking that is not supported by arm-none-eabi
# Also remove "undefined reference to `_sbrk'" error
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-specs=nano.specs -specs=nosys.specs")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})
SET(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -Wall -mfpu=fpv4-sp-d16 -Wl,-Map=output.map -Wl,--gc-sections -g3")

set(TFLM_MODES interpreter compiler)
set(TFLM_COMPILER_EXECUTABLE "/Users/Philipp/src/work/fp/git/tflite_micro_compiler_working/build/compiler") # TODO

### Configure STM32 Board here ###
#set(STM32_CHIP STM32F413ZHT6)
set(STM32_CHIP STM32F769NIH6)
#set(STM_BOARD STM32F413H-Discovery)
set(STM_BOARD STM32F769I-Discovery)
STRING(REPLACE "-" "_" STM_BOARD_CLEAN ${STM_BOARD})
STRING(TOUPPER ${STM_BOARD_CLEAN} STM_BOARD_CLEAN_UPPER)
ADD_DEFINITIONS(-DSTM32_BOARD_${STM_BOARD_CLEAN_UPPER})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/gcc_stm32.cmake)
include(${CMAKE_TOOLCHAIN_FILE})

# Configure TFML Board
IF(${STM_BOARD} STREQUAL "STM32F413H-Discovery")
  set(TFLM_BOARD disco_f413zh)
ELSEIF(${STM_BOARD} STREQUAL "STM32F769I-Discovery")
  set(TFLM_BOARD disco_f769ni)
ENDIF()

### Configure TFML Mode here ###
#set(TFLM_MODE compiler)
set(TFLM_MODE interpreter)

# (TODO)
if(${TFLM_MODE} STREQUAL compiler)
  set(CMAKE_TFLM_COMPILER_FILE ${CMAKE_MODULE_PATH}/tflm_compiler.cmake)
  include(${CMAKE_TFLM_COMPILER_FILE})
endif()

### Enable/Disable usage of CMSISNN custom kernels ###
SET(TFLM_USE_CMSIS_NN ON)
IF(${TFLM_USE_CMSIS_NN})
  add_definitions(-DCMSIS_NN)
ENDIF()

### Enable/Disable Benchmarking ###
SET(ENABLE_BENCHMARKING OFF)
IF(${ENABLE_BENCHMARKING})
  add_definitions(-DBENCHMARKING)
ENDIF()

# Disable MBED (Only valid for Tensorflow Toolchain)
add_definitions(-DNOMBED)

### Configure 'hello_world' Example ###
add_definitions(-DHELLO_WOLRD_DELAY_MS=100)

### Configure 'micro_speech' Example ###
set(FAKE_MIC OFF) # Use SD Card instead of Microphone

### Configure 'mnist' Example ###
set(FAKE_TOUCH OFF) # Use SD Card instead of Touchscreen

### Edit User/Name for using a custom Repository ###
SET(CUBE_REPO_USER "STMicroelectronics")
#SET(CUBE_REPO_USER "alxhoff")
SET(CUBE_REPO_NAME "STM32Cube${STM32_FAMILY}")
#SET(CUBE_REPO_NAME "STM32F${CUBE_SERIES}-Cube-Firmware")

# Fetching Cube Firmware Repository
FIND_PACKAGE(Git REQUIRED)
set(CUBE_SOURCE "${CMAKE_BINARY_DIR}/STM32${STM32_FAMILY}-Cube-Firmware")
if(EXISTS ${CUBE_SOURCE})
  message(STATUS "Cube FW directory exists")
else()
  message(STATUS "Getting Cube Firmware")
    set(GIT_REPOSITORY "https://github.com/${CUBE_REPO_USER}/${CUBE_REPO_NAME}.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone ${GIT_REPOSITORY} ${CUBE_SOURCE})
endif()
set(STM32Cube_DIR ${CUBE_SOURCE})

# Prepare Tensorflow Lite (micro)
#SET(TF_COMMIT d6426459bca2971b611ca9773858e55f6cc1a9af) # Comment in if TF update introduces errors
FIND_PACKAGE(TFLite REQUIRED)
SET(TFLITE_PLATFORM micro)

### Configure FreeRTOS (optional, not supported!)
#SET(FREERTOS_HEAP_IMPL 1)
#FIND_PACKAGE(FreeRTOS REQUIRED)

### Configure SD/USB-Support ###
SET(STORAGE_DRIVER "SDCARD")
FIND_PACKAGE(FATFS REQUIRED)
FIND_PACKAGE(USBHost COMPONENTS MSC REQUIRED)

### Configure CMSIS(-NN) ##
FIND_PACKAGE(CMSIS REQUIRED)
IF(${TFLM_USE_CMSIS_NN})
FIND_PACKAGE(CMSISNN REQUIRED)
ENDIF()

### Configure STM32 HAL/BSP/LL ###
IF(${TFLM_BOARD} STREQUAL "disco_f413zh")
  FIND_PACKAGE(STM32HAL COMPONENTS sram gpio uart fmpi2c i2s dac adc dfsdm sd dma qspi REQUIRED)
  FIND_PACKAGE(STM32LL COMPONENTS fsmc sdmmc REQUIRED)
  FIND_PACKAGE(STM32BSP COMPONENTS ts lcd audio ili9325 stmpe811 st7789h2 ft6x06 wm8994 psram sd REQUIRED)
ELSEIF(${TFLM_BOARD} STREQUAL "disco_f769ni")
  FIND_PACKAGE(STM32HAL COMPONENTS sram gpio uart i2s dac adc dfsdm sd dma dma2d qspi dsi ltdc sdram sai i2c REQUIRED)
  FIND_PACKAGE(STM32LL COMPONENTS fmc sdmmc REQUIRED)
  FIND_PACKAGE(STM32BSP COMPONENTS ts lcd audio stmpe811 st7789h2 ft6x06 wm8994 sdram sd otm8009a REQUIRED)
ENDIF()

### Comment in the target TFLM Example ###
#FIND_PACKAGE(TFLMExamples COMPONENTS hello_world REQUIRED)
#FIND_PACKAGE(TFLMExamples COMPONENTS micro_speech REQUIRED)
FIND_PACKAGE(TFLMExamples COMPONENTS mnist REQUIRED)

# Setting up tests (optional)
SET(TEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Inc/tests")
file(GLOB TEST_SOURCES
    "${PROJECT_SOURCE_DIR}/Src/tests/*.c"
    "${PROJECT_SOURCE_DIR}/Src/tests/*.cc"
    "${PROJECT_SOURCE_DIR}/Src/tests/*.cpp"
    )

# Gather all project source files
file(GLOB PROJECT_SOURCES
    "Src/*.c"
    "Src/*.cc"
    "Src/*.cpp"
    )

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/Inc"
    ${TEST_INCLUDE_DIR}
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

SET(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})

TARGET_COMPILE_DEFINITIONS(${CMAKE_PROJECT_NAME}.elf PUBLIC STM_BSP)

ADD_CUSTOM_TARGET(
    flash
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
    COMMAND st-flash write ${CMAKE_PROJECT_NAME}.bin 0x8000000
    )

ADD_DEPENDENCIES(flash ${CMAKE_PROJECT_NAME}.elf)

IF(${STM_BOARD} STREQUAL "STM32F413H-Discovery")
  set(OPENOCD_BOARD "board/st_nucleo_f4.cfg")
ELSEIF(${STM_BOARD} STREQUAL "STM32F769I-Discovery")
  set(OPENOCD_BOARD "board/stm32f7discovery.cfg")
ENDIF()

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
