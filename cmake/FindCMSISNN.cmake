INCLUDE(FetchContent)

SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)
FetchContent_Declare(
    arm_cmsis
    GIT_REPOSITORY https://github.com/ARM-software/CMSIS_5.git
    )

FetchContent_GetProperties(arm_cmsis)
IF(NOT arm_cmsis_POPULATED)
    MESSAGE(STATUS "Getting most recent ARM CMSIS sources")
    FetchContent_Populate(arm_cmsis)
    EXECUTE_PROCESS(COMMAND git -C ${arm_cmsis_SOURCE_DIR} checkout develop)
ENDIF()

SET(ARM_CMSIS_DIR ${arm_cmsis_SOURCE_DIR}/CMSIS)

SET(CMSISNN_CORE_HEADERS
    cmsis_compiler.h
    cmsis_gcc.h
    cmsis_version.h
    core_cm3.h
    )

SET(CMSIS_NN_HEADERS
    arm_nnfunctions.h
    arm_nnsupportfunctions.h
    arm_nn_tables.h
    )

SET(CMSIS_NN_SRCS
    arm_relu_q7.c
    arm_fully_connected_q7.c
    arm_softmax_q7.c
    arm_q7_to_q15_reordered_no_shift.c
    arm_convolve_HWC_q7_basic_nonsquare.c
    arm_q7_to_q15_no_shift.c
    arm_nn_mat_mult_kernel_q7_q15.c
    arm_depthwise_separable_conv_HWC_q7_nonsquare.c
    arm_convolve_1x1_HWC_q7_fast_nonsquare.c
    arm_nn_mat_mult_kernel_q7_q15_reordered.c
    )

FIND_PATH(CMSIS_NN_INC_DIR ${CMSIS_NN_HEADERS}
    PATHS ${ARM_CMSIS_DIR}/NN/Include
    CMAKE_FIND_ROOT_PATH_BOTH
    )

SET(CMSIS_DSP_HEADERS
    arm_common_tables.h
    arm_const_structs.h
    # arm_helium_utils.h
    arm_math.h
    # arm_mve_tables.h
    # arm_vec_math.h
    )

SET(CMSIS_DSP_PRIVATE_HEADERS
    arm_sorting.h
    arm_vec_fft.h
    arm_vec_filtering.h
    )

SET(CMSIS_DSP_SRCS
    arm_rfft_fast_init_f32.c
    arm_rfft_fast_f32.c
    arm_common_tables.c
    arm_const_structs.c
    arm_cfft_f32.c
    arm_cfft_init_f32.c
    arm_cfft_radix8_f32.c
    arm_bitreversal2.S
    arm_copy_q7.c
    )

FOREACH(SRC ${CMSIS_DSP_SRCS})
    STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
    SET(CMSIS_DSP_${SRC_CLEAN}_FILE ${SRC_CLEAN}-NOTFOUND)
    FIND_FILE(CMSIS_DSP_${SRC_CLEAN}_FILE ${SRC}
        PATH_SUFFIXES
        BasicMathFunctions
        BayesFunctions
        CommonTables
        ComplexMathFunctions
        ControllerFunctions
        DistanceFunctions
        FastMathFunctions
        FilteringFunctions
        MatrixFunctions
        StatisticsFunctions
        SupportFunctions
        SVMFunctions
        TransformFunctions
        PATHS ${ARM_CMSIS_DIR}/DSP/Source
        CMAKE_FIND_ROOT_PATH_BOTH
        )
    LIST(APPEND CMSIS_DSP_SOURCES ${CMSIS_DSP_${SRC_CLEAN}_FILE})
ENDFOREACH()

FOREACH(SRC ${CMSIS_NN_SRCS})
    STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
    SET(CMSIS_NN_${SRC_CLEAN}_FILE ${SRC_CLEAN}-NOTFOUND)
    FIND_FILE(CMSIS_NN_${SRC_CLEAN}_FILE ${SRC}
        PATH_SUFFIXES
        ActivationFunctions
        ConvolutionFunctions
        FullyConnectedFunctions
        NNSupportFunctions
        PoolingFunctions
        SoftmaxFunctions
        PATHS ${ARM_CMSIS_DIR}/NN/Source
        CMAKE_FIND_ROOT_PATH_BOTH
        )
    LIST(APPEND CMSIS_NN_SOURCES ${CMSIS_NN_${SRC_CLEAN}_FILE})
ENDFOREACH()

FIND_PATH(CMSIS_DSP_PRIVATE_INC_DIR ${CMSIS_DSP_PRIVATE_HEADERS}
    PATHS ${ARM_CMSIS_DIR}/DSP/PrivateInclude
    CMAKE_FIND_ROOT_PATH_BOTH
    )

FIND_PATH(CMSIS_DSP_INC_DIR ${CMSIS_DSP_HEADERS}
    PATHS ${ARM_CMSIS_DIR}/DSP/Include
    CMAKE_FIND_ROOT_PATH_BOTH
    )

SET(CMSIS_DSP_INC_DIRS
    ${CMSIS_DSP_PRIVATE_INC_DIR}
    ${CMSIS_DSP_INC_DIR}
    )

SET(CMSISNN_INCLUDE_DIRS
    ${CMSIS_DSP_INC_DIRS}
    ${CMSIS_NN_INC_DIR}
    )

SET(CMSISNN_SOURCES
    ${CMSIS_DSP_SOURCES}
    ${CMSIS_NN_SOURCES}
    )

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CMSISNN DEFAULT_MSG CMSISNN_INCLUDE_DIRS CMSISNN_SOURCES)
