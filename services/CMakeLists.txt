# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License v3.0.

macro(qkernel_add_service TARGET)
  message(STATUS "Service ${COUNTER}: ${TARGET}")

  file(GLOB_RECURSE SRC_FILES "*.cc")
  add_executable(${TARGET} ${SRC_FILES})

  set_target_properties(${TARGET}
                        PROPERTIES OUTPUT_NAME "${TARGET}"
                                   RUNTIME_OUTPUT_DIRECTORY
                                   "${KERNEL_ROOT_DIR}/bin"
  )

  target_link_libraries(${TARGET} PRIVATE core::service)
endmacro()

# ----------------------------------------------------------------------------

add_subdirectory("./cat")
add_subdirectory("./ls")
add_subdirectory("./yes")
