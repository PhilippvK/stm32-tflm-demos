# Settings for the Discovery STM32F769NI board.
ifneq ($(filter disco_f769ni,$(ALL_TAGS)),)
  hello_world_MBED_PROJECT_FILES += \
    BSP_DISCO_F769NI.lib
endif
