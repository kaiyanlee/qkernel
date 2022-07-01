# Copyright (c) 2022 Kaiyan M. Lee
#
# QKernel is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License v3.0.

set(BOOT_DIR "${KERNEL_ROOT_DIR}/boot")

if(QKERNEL_KERNEL_ARCH STREQUAL "x86")
  # Create our kernel executable.
  add_executable(qkernel "x86/start.asm" "x86/start.cc")

  set(QEMU_ARGS
      -serial
      "mon:stdio"
      -nographic
      -machine
      "q35"
      -cpu
      "pentium"
      -no-reboot
      -m
      "32"
      -drive
      "file=fat:rw:${KERNEL_ROOT_DIR},index=0,media=disk,format=raw"
  )

  # To run the kernel in QEMU. Note that this does not work with
  # 64 bit kernels because QEMU uses Multiboot 1 for -kernel arguments. Multiboot 1
  # is old and does not support 64 bit kernels.
  add_custom_target(start-kernel
                    COMMAND mkdir -p ${BOOT_DIR}
                    COMMAND qemu-system-i386 -kernel ${BOOT_DIR}/qkernel
                            ${QEMU_ARGS}
                    DEPENDS qkernel
  )

  # To run the kernel in QEMU.
  # grub-mkstandalone root/ -o qkernel.img -O i386-efi --fonts="" --locales="" --themes=""
  # Broken ATM: will need to fix image generation.
  add_custom_target(start-kernel-iso
                    COMMAND mkdir -p ${BOOT_DIR}
                    COMMAND qemu-system-i386 -cdrom
                            ${CMAKE_BINARY_DIR}/qkernel.iso ${QEMU_ARGS}
                    DEPENDS qkernel
  )

elseif(QKERNEL_KERNEL_ARCH STREQUAL "x86_64")
  # TODO
elseif(QKERNEL_KERNEL_ARCH STREQUAL "aarch64")
  # TODO
endif()

# Link to main library.
target_link_libraries(qkernel PRIVATE kernel)

# Custom properties (remove default compile definitions)...
set_target_properties(qkernel
                      PROPERTIES LINK_OPTIONS "${LINK_OPTIONS}"
                                 LINK_DEPENDS "${LINKER_SCRIPT}"
                                 RUNTIME_OUTPUT_DIRECTORY "${BOOT_DIR}"
)

add_custom_target(generate-kernel-image
                  COMMAND sh ${CMAKE_SOURCE_DIR}/tools/mkimage.sh
                  BYPRODUCTS "${CMAKE_BINARY_DIR}/qkernel.img"
                  VERBATIM
)

# Copy data files
add_custom_target(copy-data
                  COMMAND mkdir -p ${BOOT_DIR}/grub
                  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/boot/grub.cfg
                          ${BOOT_DIR}/grub/grub.cfg
                  COMMAND rsync -a "${CMAKE_SOURCE_DIR}/examples/"
                          "${KERNEL_ROOT_DIR}/etc"
)
