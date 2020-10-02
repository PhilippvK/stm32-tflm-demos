add_definitions(-DTFLM_MODE_COMPILER)

set(TFLM_COMPILER_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Src/${TFLM_EXAMPLE}/")

ADD_CUSTOM_TARGET(
    convert
    COMMAND test ! -z $$FILE || (echo 'Please define .tflite file using `make convert FILE=...`' && false)
    COMMAND test -f $$FILE || (echo 'File not found!' && false)
    COMMAND test -f ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc && cp ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc.old || :
    COMMAND test -f ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h && cp ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h.old || :
    COMMAND ${TFLM_COMPILER_EXECUTABLE} $$FILE ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc ${TFLM_EXAMPLE}_
    COMMAND mv ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.cc.h ${TFLM_COMPILER_OUTPUT_DIRECTORY}/offline_model.h
    )
