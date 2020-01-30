find_path(Fixedpoint_INCLUDE_DIR
    NAMES fixedpoint.h fixedpoint_avx.h fixedpoint_msa.h fixedpoint_neon.h fixedpoint_sse.h
    HINTS /usr/include/tensorflow/fixedpoint
    CMAKE_FIND_ROOT_PATH_BOTH
    )

MESSAGE(STATUS "Fixedpoint Include Dir: ${Fixedpoint_INCLUDE_DIR}")
