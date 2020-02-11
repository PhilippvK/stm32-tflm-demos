IF(NOT TF_DIR)
    MESSAGE(ERROR "No TensorFlow directory (TF_DIR) has been set")
ENDIF()
LIST(APPEND TensorFlowLite_INCLUDE_DIRS ${TF_DIR})

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

SET(TFLITE_COMMON_KERNEL_HEADERS
    tensorflow/lite/kernels/internal/common.h 
    tensorflow/lite/kernels/internal/compatibility.h                                
    tensorflow/lite/kernels/internal/optimized/neon_check.h                         
    tensorflow/lite/kernels/internal/quantization_util.h                            
    tensorflow/lite/kernels/internal/reference/add.h                                
    tensorflow/lite/kernels/internal/reference/arg_min_max.h                        
    tensorflow/lite/kernels/internal/reference/binary_function.h                    
    tensorflow/lite/kernels/internal/reference/ceil.h                               
    tensorflow/lite/kernels/internal/reference/comparisons.h                        
    tensorflow/lite/kernels/internal/reference/concatenation.h                      
    tensorflow/lite/kernels/internal/reference/conv.h                               
    tensorflow/lite/kernels/internal/reference/depthwiseconv_float.h                
    tensorflow/lite/kernels/internal/reference/depthwiseconv_uint8.h                
    tensorflow/lite/kernels/internal/reference/dequantize.h                         
    tensorflow/lite/kernels/internal/reference/floor.h                              
    tensorflow/lite/kernels/internal/reference/fully_connected.h                    
    tensorflow/lite/kernels/internal/reference/integer_ops/add.h                    
    tensorflow/lite/kernels/internal/reference/integer_ops/conv.h                   
    tensorflow/lite/kernels/internal/reference/integer_ops/depthwise_conv.h         
    tensorflow/lite/kernels/internal/reference/integer_ops/fully_connected.h           
    tensorflow/lite/kernels/internal/reference/integer_ops/mul.h                    
    tensorflow/lite/kernels/internal/reference/integer_ops/softmax.h                
    tensorflow/lite/kernels/internal/reference/maximum_minimum.h                    
    tensorflow/lite/kernels/internal/reference/mul.h                                
    tensorflow/lite/kernels/internal/reference/neg.h                                
    tensorflow/lite/kernels/internal/reference/pad.h                                
    tensorflow/lite/kernels/internal/reference/pooling.h                            
    tensorflow/lite/kernels/internal/reference/prelu.h                              
    tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h           
    tensorflow/lite/kernels/internal/reference/quantize.h                           
    tensorflow/lite/kernels/internal/reference/round.h                              
    tensorflow/lite/kernels/internal/reference/softmax.h                            
    tensorflow/lite/kernels/internal/reference/logistic.h                           
    tensorflow/lite/kernels/internal/reference/strided_slice.h                      
    tensorflow/lite/kernels/internal/reference/integer_ops/pooling.h                
    tensorflow/lite/kernels/internal/round.h                                        
    tensorflow/lite/kernels/internal/strided_slice_logic.h                          
    tensorflow/lite/kernels/internal/tensor.h                                       
    tensorflow/lite/kernels/internal/tensor_ctypes.h                                
    tensorflow/lite/kernels/internal/types.h                                        
    tensorflow/lite/kernels/kernel_util.h 
    tensorflow/lite/kernels/op_macros.h                                                
    tensorflow/lite/kernels/padding.h   
    )

SET(TFLITE_COMMON_KERNEL_SOURCES
    tensorflow/lite/kernels/internal/quantization_util.cc
    tensorflow/lite/kernels/kernel_util.cc 
    )

SET(TFLITE_CORE_HEADERS
    tensorflow/lite/core/api/error_reporter.h
    tensorflow/lite/core/api/flatbutter_conversions.h
    tensorflow/lite/core/api/op_resolver.h
    tensorflow/lite/core/api/tensor_utils.h
    )

SET(TFLITE_CORE_SOURCES
    tensorflow/lite/core/api/error_reporter.cc
    tensorflow/lite/core/api/flatbuffer_conversions.cc
    tensorflow/lite/core/api/op_resolver.cc
    tensorflow/lite/core/api/tensor_utils.cc
    )

SET(TFLITE_C_HEADERS
    common.h
    )

SET(TFLITE_C_SOURCES
    tensorflow/lite/c/common.c
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

        SET(TFLITE_PLATFORM_HEADERS
            tensorflow/lite/micro/compatibility.h
            tensorflow/lite/micro/debug_log.h
            tensorflow/lite/micro/debug_log_numbers.h
            tensorflow/lite/micro/memory_helpers.h
            tensorflow/lite/micro/micro_allocator.h
            tensorflow/lite/micro/micro_error_reporter.h
            tensorflow/lite/micro/micro_interpreter.h
            tensorflow/lite/micro/micro_mutable_op_resolver.h
            tensorflow/lite/micro/micro_optional_debug_tools.h
            tensorflow/lite/micro/micro_utils.h
            tensorflow/lite/micro/simple_memory_allocator.h
            tensorflow/lite/micro/test_helpers.h
            tensorflow/lite/micro/memory_planner/greedy_memory_planner.h 
            tensorflow/lite/micro/memory_planner/linear_memory_planner.h
            tensorflow/lite/micro/memory_planner/memory_planner.h
            )
        
        SET(TFLITE_PLATFORM_SOURCES
            # tensorflow/lite/micro/debug_log.cc
            tensorflow/lite/micro/debug_log_numbers.cc
            tensorflow/lite/micro/memory_helpers.cc
            tensorflow/lite/micro/micro_allocator.cc
            tensorflow/lite/micro/micro_error_reporter.cc
            tensorflow/lite/micro/micro_interpreter.cc
            tensorflow/lite/micro/micro_optional_debug_tools.cc
            tensorflow/lite/micro/micro_utils.cc
            tensorflow/lite/micro/simple_memory_allocator.cc
            tensorflow/lite/micro/test_helpers.cc
            tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
            tensorflow/lite/micro/memory_planner/linear_memory_planner.cc
            )

        IF(TARGET)
            LIST(APPEND TFLITE_PLATFORM_SOURCES tensorflow/lite/micro/${TARGET}/debug_log.cc)
        ELSE()
            LIST(APPEND TFLITE_PLATFORM_SOURCES tensorflow/lite/micro/debug_log.cc)
        ENDIF()

        SET(TFLITE_KERNEL_HEADERS
            tensorflow/lite/micro/kernels/activation_utils.h
            tensorflow/lite/micro/kernels/all_ops_resolver.h
            tensorflow/lite/micro/kernels/micro_ops.h
            tensorflow/lite/micro/kernels/micro_utils.h
            )

        SET(TFLITE_KERNEL_SOURCES
            tensorflow/lite/micro/kernels/activations.cc
            tensorflow/lite/micro/kernels/add.cc
            tensorflow/lite/micro/kernels/all_ops_resolver.cc
            tensorflow/lite/micro/kernels/arg_min_max.cc
            tensorflow/lite/micro/kernels/ceil.cc
            tensorflow/lite/micro/kernels/comparisons.cc
            tensorflow/lite/micro/kernels/concatenation.cc
            tensorflow/lite/micro/kernels/conv.cc
            tensorflow/lite/micro/kernels/depthwise_conv.cc
            tensorflow/lite/micro/kernels/dequantize.cc
            tensorflow/lite/micro/kernels/elementwise.cc
            tensorflow/lite/micro/kernels/floor.cc
            tensorflow/lite/micro/kernels/fully_connected.cc
            tensorflow/lite/micro/kernels/logical.cc
            tensorflow/lite/micro/kernels/logistic.cc
            tensorflow/lite/micro/kernels/maximum_minimum.cc
            tensorflow/lite/micro/kernels/mul.cc
            tensorflow/lite/micro/kernels/neg.cc
            tensorflow/lite/micro/kernels/pack.cc
            tensorflow/lite/micro/kernels/pad.cc
            tensorflow/lite/micro/kernels/pooling.cc
            tensorflow/lite/micro/kernels/prelu.cc
            tensorflow/lite/micro/kernels/quantize.cc
            tensorflow/lite/micro/kernels/reduce.cc
            tensorflow/lite/micro/kernels/reshape.cc
            tensorflow/lite/micro/kernels/round.cc
            tensorflow/lite/micro/kernels/softmax.cc
            tensorflow/lite/micro/kernels/split.cc
            tensorflow/lite/micro/kernels/strided_slice.cc
            tensorflow/lite/micro/kernels/svdf.cc
            tensorflow/lite/micro/kernels/unpack.cc
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

        MESSAGE(ERROR "Find TensaFlow Lite only supports platforms {micro} at the moment")
    ENDIF()

    SET(TFLITE_HEADERS 
        ${TFLITE_COMMON_KERNEL_HEADERS}
        ${TFLITE_CORE_HEADERS}
        ${TFLITE_C_HEADERS}
        ${TFLITE_SCHEMA_HEADERS}
        ${TFLITE_PLATFORM_HEADERS}
        ${TFLITE_KERNEL_HEADERS}
        )

    SET(TFLITE_SOURCES 
        ${TFLITE_COMMON_KERNEL_SOURCES}
        ${TFLITE_CORE_SOURCES}
        ${TFLITE_C_SOURCES}
        ${TFLITE_SCHEMA_SOURCES}
        ${TFLITE_PLATFORM_SOURCES}
        ${TFLITE_KERNEL_SOURCES}
        )

    FOREACH(SRC ${TFLITE_SOURCES})
        STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
        FIND_FILE(TFLITE_${SRC_CLEAN}_FILE ${SRC}
            HINTS ${TF_DIR}
            CMAKE_FIND_ROOT_PATH_BOTH
            )
        LIST(APPEND TensorFlowLite_SOURCES ${TFLITE_${SRC_CLEAN}_FILE})
    ENDFOREACH()
    
    LIST(REMOVE_DUPLICATES TensorFlowLite_INCLUDE_DIRS)
    LIST(REMOVE_DUPLICATES TensorFlowLite_SOURCES)

ENDIF()

MESSAGE(STATUS "TF Lite Sources: ${TensorFlowLite_SOURCES}")
MESSAGE(STATUS "TF Lite Include Dirs: ${TensorFlowLite_INCLUDE_DIRS}")

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(TensorFlowLite DEFAULT_MSG TensorFlowLite_INCLUDE_DIRS TensorFlowLite_SOURCES)
