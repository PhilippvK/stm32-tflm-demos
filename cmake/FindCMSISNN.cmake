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
    arm_nn_types.h
    )

SET(CMSIS_NN_SRCS
    arm_elementwise_mul_s8.c
    arm_elementwise_add_s8.c
    arm_fully_connected_q7_opt.c
    arm_fully_connected_s8.c
    arm_fully_connected_q15_opt.c
    arm_fully_connected_q15.c
    arm_fully_connected_mat_q7_vec_q15.c
    arm_fully_connected_q7.c
    arm_fully_connected_mat_q7_vec_q15_opt.c
    arm_concatenation_s8_z.c
    arm_concatenation_s8_x.c
    arm_concatenation_s8_w.c
    arm_concatenation_s8_y.c
    arm_nn_depthwise_conv_s8_core.c
    arm_depthwise_conv_u8_basic_ver1.c
    arm_convolve_s8.c
    arm_convolve_1x1_s8_fast.c
    arm_convolve_HWC_q7_fast.c
    arm_nn_mat_mult_kernel_q7_q15_reordered.c
    arm_nn_mat_mult_kernel_q7_q15.c
    arm_convolve_HWC_q7_RGB.c
    arm_nn_mat_mult_s8.c
    arm_convolve_1x1_HWC_q7_fast_nonsquare.c
    arm_convolve_HWC_q7_basic.c
    arm_depthwise_separable_conv_HWC_q7_nonsquare.c
    arm_convolve_HWC_q15_fast_nonsquare.c
    arm_depthwise_conv_s8_opt.c
    arm_depthwise_conv_3x3_s8.c
    arm_nn_mat_mult_kernel_s8_s16.c
    arm_convolve_HWC_q15_basic.c
    arm_convolve_HWC_q15_fast.c
    arm_convolve_HWC_q7_fast_nonsquare.c
    arm_nn_mat_mult_kernel_s8_s16_reordered.c
    arm_depthwise_separable_conv_HWC_q7.c
    arm_depthwise_conv_s8.c
    arm_convolve_1_x_n_s8.c
    arm_convolve_HWC_q7_basic_nonsquare.c
    arm_convolve_wrapper_s8.c
    arm_depthwise_conv_wrapper_s8.c
    arm_pool_q7_HWC.c
    arm_avgpool_s8.c
    arm_max_pool_s8.c
    arm_relu_q15.c
    arm_relu_q7.c
    arm_nn_activations_q7.c
    arm_relu6_s8.c
    arm_nn_activations_q15.c
    arm_nn_add_q7.c
    arm_nn_depthwise_conv_nt_t_padded_s8.c
    arm_q7_to_q15_reordered_no_shift.c
    arm_nn_depthwise_conv_nt_t_s8.c
    arm_nntables.c
    arm_nn_accumulate_q7_to_q15.c
    arm_nn_mult_q7.c
    arm_nn_mult_q15.c
    arm_q7_to_q15_reordered_with_offset.c
    arm_q7_to_q15_with_offset.c
    arm_nn_mat_mul_core_4x_s8.c
    arm_nn_mat_mult_nt_t_s8.c
    arm_nn_vec_mat_mult_t_s8.c
    arm_nn_mat_mul_core_1x_s8.c
    arm_q7_to_q15_no_shift.c
    arm_softmax_s8.c
    arm_softmax_q15.c
    arm_softmax_u8.c
    arm_softmax_q7.c
    arm_softmax_with_batch_q7.c
    arm_reshape_s8.c
    )

FIND_PATH(CMSIS_NN_INC_DIR ${CMSIS_NN_HEADERS}
    PATHS ${ARM_CMSIS_DIR}/NN/Include
    CMAKE_FIND_ROOT_PATH_BOTH
    )

SET(CMSIS_DSP_HEADERS
    arm_common_tables.h
    arm_const_structs.h
    arm_math.h
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
        BasicMathFunctions
        ActivationFunctions
        ConvolutionFunctions
        FullyConnectedFunctions
        ConcatenationFunctions
        NNSupportFunctions
        PoolingFunctions
        SoftmaxFunctions
        ReshapeFunctions
        PATHS ${ARM_CMSIS_DIR}/NN/Source
        CMAKE_FIND_ROOT_PATH_BOTH
        )
    LIST(APPEND CMSIS_NN_SOURCES ${CMSIS_NN_${SRC_CLEAN}_FILE})
ENDFOREACH()

# Workaround (See https://github.com/ARMmbed/mbed-os/issues/12568)
MESSAGE(STATUS "Adding Workaround for '__SXTB16_RORn' to CMSIS-NN...")
EXECUTE_PROCESS(COMMAND "bash" "-c" "grep -rq '__patched_SXTB16_RORn' ${ARM_CMSIS_DIR}/NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c || sed -i -E 's@__SXTB16_RORn@__patched_SXTB16_RORn@g' ${ARM_CMSIS_DIR}/NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c")
EXECUTE_PROCESS(COMMAND "bash" "-c" "grep -rq 'uint32_t __patched_SXTB16_RORn' ${ARM_CMSIS_DIR}/NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c || sed -i -E $'35 a \\\n\\\n// Work around for https://github.com/ARMmbed/mbed-os/issues/12568\\\n__STATIC_FORCEINLINE uint32_t __patched_SXTB16_RORn(uint32_t op1, uint32_t rotate) {\\\n  uint32_t result;\\\n  __ASM (\"sxtb16 %0, %1, ROR %2\" : \"=r\" (result) : \"r\" (op1), \"i\" (rotate) );\\\n  return result;\\\n}' ${ARM_CMSIS_DIR}/NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c")

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
