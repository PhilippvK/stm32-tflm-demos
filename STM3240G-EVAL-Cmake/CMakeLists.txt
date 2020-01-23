PROJECT(STM32F40G_Display)

set(CMAKE_BUILD_TYPE Debug)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
ENABLE_LANGUAGE(ASM)

# Remove -rdynamic option from linking that is not supported by arm-none-eabi
# Also remove "undefined reference to `_sbrk'" error
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-specs=nano.specs -specs=nosys.specs")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(STM32_CHIP STM32F457IG)
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

# SET(FREERTOS_HEAP_IMPL 1)
# SET(CMAKE_C_FLAGS "-mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4 -mlittle-endian -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=c99 -Wall -Wunused-value -O3 -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-common --param max-inline-insns-single=1000 -DUSE_STDPERIPH_DRIVER -DGFX_USE_OS_FREERTOS=TRUE -DSTM32F429_439xx=TRUE -g3")
#
FIND_PACKAGE(CMSIS required)
FIND_PACKAGE
# FIND_PACKAGE(STM32STD COMPONENTS dma dma2d fmc i2c ltdc gpio rcc spi usart adc tim exti syscfg REQUIRED)
# FIND_PACKAGE(STM32Discovery COMPONENTS ioe lcd sdram REQUIRED)
# FIND_PACKAGE(uGFX COMPONENTS gdisp_mcufont gdriver gdisp gos REQUIRED)

INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc"
    "${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include"
    "${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
    "${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    ${STM32LL_INCLUDE_DIR}
    )

file(GLOB PROJECT_SOURCES
    "Src/*.c"
    "Middlewares/Third_Party/FreeRTOS/portable/MemMang/*.c"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c"
    "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/*.c"
    "Middlewares/Third_Party/FreeRTOS/Source/*.c"
    "Drivers/STM32F4xx_HAL_Driver/Src/*.c"
    )

SET(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})

ADD_EXECUTABLE(
    ${CMAKE_PROJECT_NAME}.elf
    ${PROJECT_SOURCES}
    ${STM32LL_SOURCES}
    startup_stm32f407xx.s
    )

# ADD_CUSTOM_TARGET(
#     flash
#     COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin # TODO
#     COMMAND st-flash write ${CMAKE_PROJECT_NAME}.bin 0x8000000
#     )
#
# ADD_DEPENDENCIES(flash ${CMAKE_PROJECT_NAME}.elf)
#
# if(${DISCOVERY_BOARD_DISC1})
#     set(OPENOCD_BOARD "board/stm32f429disc1.cfg")
# else()
#     set(OPENOCD_BOARD "board/stm32f429discovery.cfg")
# endif()
#
# ADD_CUSTOM_TARGET(
#     debug
#     COMMAND openocd -f ${OPENOCD_BOARD} >/dev/null 2>&1 & sleep 2
#     COMMAND ${TARGET_TRIPLET}-gdb -quiet -tui -command=GDBCommands
#     COMMAND killall -15 openocd
#     )
#
# ADD_DEPENDENCIES(debug flash)
#
# STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}.elf)
# STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME}.elf)
# STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME}.elf)

# TODO: also clean .bin file on `make clean`
