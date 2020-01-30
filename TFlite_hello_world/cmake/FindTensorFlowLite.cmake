IF(NOT TF_DIR)
    MESSAGE(ERROR "No TensorFlow directory (TF_DIR) has been set")
ENDIF()

# Tensorflow include
FIND_PATH(TENSORFLOW_INCLUDE_DIR 
    NAMES
    tensorflow/core
    tensorflow/include
    tensorflow/cc
    tensorflow/c
    HINTS
    /usr/include/tensorflow
    CMAKE_FIND_ROOT_PATH_BOTH
    )

LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${TENSORFLOW_INCLUDE_DIR})

SET(TFLITE_REQUIRED_CONTENT
    flatbuffers
    fixedpoint
    )

INCLUDE(FetchContent)

FetchContent_Declare(
    flatbuffers
    GIT_REPOSITORY https://github.com/google/flatbuffers.git
    )

FOREACH(CONTENT ${TFLITE_REQUIRED_CONTENT})
    IF(${CONTENT} STREQUAL flatbuffers)
        FetchContent_GetProperties(flatbuffers)
        IF(NOT flatbuffers_POPULATED)
            FetchContent_Populate(flatbuffers)
        ENDIF()
        LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${flatbuffers_SOURCE_DIR}/include)
    ELSEIF(${CONTENT} STREQUAL fixedpoint)
        FIND_PACKAGE(Fixedpoint)
        LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${Fixedpoint_INCLUDE_DIR})
    ENDIF()
ENDFOREACH()

SET(TFLITE_DIR ${TF_DIR}/tensorflow/lite)

SET(TFLITE_CORE_HEADERS
    macros.h
    subgraph.h
    api/error_reporter.h
    api/flatbutter_conversions.h
    api/op_resolver.h
    api/profiler.h
    api/tensor_utils.h
    )

SET(TFLITE_C_HEADERS
    builtin_op_data.h
    c_api_experimental.h
    c_api.h
    c_api_internal.h
    common.h
    )

SET(TFLITE_C_SOURCES
    c_api.cc
    c_api_experimental.cc
    common.c
    )

SET(TFLITE_CORE_SOURCES
    subgraph.cc
    api/error_reporter.cc
    api/flatbuffer_conversions.cc
    api/op_resolver.cc
    api/tensor_utils.cc
    )

SET(TFLITE_SCHEMA_HEADERS
    schema_generated.h
    )

SET(TFLITE_SCHEMA_SOURCES
    )

IF(NOT TFLITE_PLATFORM)
    MESSAGE(ERROR "TensaFlow Lite platform not set, please SET TFLITE_PLATFORM to {micro}")
ELSE()
IF(TFLITE_PLATFORM STREQUAL "micro")
    SET(TFLITE_PLATFORM_DIR ${TFLITE_DIR}/micro)

    SET(TFLITE_PLATFORM_HEADERS
        compatibility.h
        debug_log.h
        debug_log_numbers.h
        memory_helpers.h
        micro_allocator.h
        micro_error_reporter.h
        micro_interpreter.h
        micro_mutable_op_resolver.h
        micro_optional_debug_tools.h
        micro_utils.h
        simple_memory_allocator.h
        test_helpers.h
        )
    
    SET(TFLITE_PLATFORM_SOURCES
        debug_log.cc
        debug_log_numbers.cc
        memory_helpers.cc
        micro_allocator.cc
        micro_error_reporter.cc
        micro_interpreter.cc
        micro_optional_debug_tools.cc
        micro_utils.cc
        simple_memory_allocator.cc
        test_helpers.cc
        memory_planner/greedy_memory_planner.cc
        memory_planner/linear_memory_planner.cc
        )

    SET(TFLITE_KERNEL_DIR ${TFLITE_DIR}/micro/kernels)

    SET(TFLITE_KERNEL_HEADERS
        activation_utils.h
        all_ops_resolver.h
        micro_ops.h
        micro_utils.h
        )

    SET(TFLITE_KERNEL_SOURCES
        micro/kernels/activations.cc
        micro/kernels/add.cc
        micro/kernels/all_ops_resolver.cc
        micro/kernels/arg_min_max.cc
        micro/kernels/ceil.cc
        micro/kernels/comparisons.cc
        micro/kernels/concatenation.cc
        micro/kernels/conv.cc
        micro/kernels/depthwise_conv.cc
        micro/kernels/dequantize.cc
        micro/kernels/elementwise.cc
        micro/kernels/floor.cc
        micro/kernels/fully_connected.cc
        micro/kernels/logical.cc
        micro/kernels/logistic.cc
        micro/kernels/maximum_minimum.cc
        micro/kernels/mul.cc
        micro/kernels/neg.cc
        micro/kernels/pack.cc
        micro/kernels/pad.cc
        micro/kernels/pooling.cc
        micro/kernels/prelu.cc
        micro/kernels/quantize.cc
        micro/kernels/reshape.cc
        micro/kernels/round.cc
        micro/kernels/softmax.cc
        micro/kernels/split.cc
        micro/kernels/strided_slice.cc
        micro/kernels/svdf.cc
        micro/kernels/cmsis-nn/add.cc
        micro/kernels/cmsis-nn/conv.cc
        micro/kernels/cmsis-nn/depthwise_conv.cc
        micro/kernels/cmsis-nn/fully_connected.cc
        micro/kernels/cmsis-nn/mul.cc
        micro/kernels/cmsis-nn/pooling.cc
        micro/kernels/cmsis-nn/scratch_buffer.cc
        kernels/kernel_util.cc
        )
ELSE()
    #TODO when are these used?
    SET(TFLITE_COMMON_HEADERS
        allocation.h
        arena_planner.h
        allocation.h
        arena_planner.h
        builtin_op_data.h
        builtin_ops.h
        context.h
        context_util.h
        error_reporter.h
        external_cpu_backend_context.h
        graph_info.cc
        graph_info.h
        interpreter.h
        memory_planner.h
        minimal_logging.h
        model.h
        mutable_op_resolver.h
        op_resolver.h
        optional_debug_tools.h
        simple_memory_arena.h
        stderr_reporter.h
        string_type.h
        string_util.h
        type_to_tflitetype.h
        util.h
        version.h
        )

    SET(TFLITE_COMMON_SOURCES
        allocation.cc
        arena_planner.cc
        external_cpu_backend_context.cc
        graph_info.cc
        interpreter.cc
        minimal_logging_android.cc
        minimal_logging.cc
        minimal_logging_default.cc
        minimal_logging_ios.cc
        mmap_allocation.cc
        mmap_allocation_disabled.cc
        model.cc
        mutable_op_resolver.cc
        optional_debug_tools.cc
        simple_memory_arena.cc
        stderr_reporter.cc
        string_util.cc
        util.cc
        )

    # COMMON

    # FOREACH(HEADER ${TFLITE_COMMON_HEADERS})
    #     FIND_PATH(${HEADER}_INC_DIR ${HEADER}
    #         HINTS ${TFLITE_DIR}
    #         CMAKE_FIND_ROOT_PATH_BOTH
    #         )
    #     LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${${HEADER}_INC_DIR})
    # ENDFOREACH()

    FOREACH(SRC ${TFLITE_COMMON_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TFLITE_DIR}
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()

    MESSAGE(ERROR "Find TensaFlow Lite only supports platforms {micro} at the moment")
ENDIF()

    # C
    FOREACH(SRC ${TFLITE_C_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TFLITE_DIR}/c
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()
    
    # CORE
    FOREACH(SRC ${TFLITE_CORE_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TFLITE_DIR}/core
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()

    # SCHEMA

    # FOREACH(HEADER ${TFLITE_SCHEMA_HEADERS})
    #     FIND_PATH(${HEADER}_INC_DIR ${HEADER}
    #         HINTS ${TFLITE_DIR}/schema
    #         CMAKE_FIND_ROOT_PATH_BOTH
    #         )
    #     LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${${HEADER}_INC_DIR})
    # ENDFOREACH()

    FOREACH(SRC ${TFLITE_SCHEMA_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TFLITE_DIR}/schema
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()

    # PLATFORM

    # FOREACH(HEADER ${TFLITE_PLATFORM_HEADERS})
    #     FIND_PATH(${HEADER}_INC_DIR ${HEADER}
    #         HINTS ${TFLITE_PLATFORM_DIR}
    #         CMAKE_FIND_ROOT_PATH_BOTH
    #         )
    #     LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${${HEADER}_INC_DIR})
    # ENDFOREACH()

    FOREACH(SRC ${TFLITE_PLATFORM_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TFLITE_PLATFORM_DIR}
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()

    # KERNEL

    # FOREACH(HEADER ${TFLITE_KERNEL_HEADERS})
    #     FIND_PATH(${HEADER}_INC_DIR ${HEADER}
    #         PATH_SIFFIXES ${TFLITE_KERNEL_SUFFIX}
    #         HINTS ${TFLITE_DIR}
    #         CMAKE_FIND_ROOT_PATH_BOTH
    #         )
    #     LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${${HEADER}_INC_DIR})
    # ENDFOREACH()

    FOREACH(SRC ${TFLITE_KERNEL_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TFLITE_DIR}
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()

    LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${TF_DIR})

    LIST(REMOVE_DUPLICATES TensorFlowLite_INCLUDE_DIRS)
    LIST(REMOVE_DUPLICATES TensorFlowLite_SOURCES)

ENDIF()

MESSAGE(STATUS "TF Lite Sources: ${TensorFlowLite_SOURCES}")
# MESSAGE(STATUS "TF Lite Include Dirs: ${TensorFlowLite_INCLUDE_DIRS}")

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(TensorFlowLite DEFAULT_MSG TensorFlowLite_INCLUDE_DIRS TensorFlowLite_SOURCES)
