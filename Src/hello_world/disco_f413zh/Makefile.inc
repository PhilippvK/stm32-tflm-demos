# Settings for the Discovery STM32F413ZH board.
ifneq ($(filter disco_f413zh,$(ALL_TAGS)),)
  hello_world_MBED_PROJECT_FILES += \
    BSP_DISCO_F413ZH.lib
endif
