cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Setup project, output and linker file
project(SAMD-Demo)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/samd21d/gcc/gcc/samd21g17d_flash.ld)

#-T"../samd21d/gcc/gcc/samd21g17d_flash.ld"
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# List of source files
set(SRC_FILES
        src/main.c
        src/driver_init.c
        hal/src/hal_atomic.c
        hal/src/hal_dac_os.c
        hal/src/hal_delay.c
        hal/src/hal_gpio.c
        hal/src/hal_init.c
        hal/src/hal_io.c
        hal/src/hal_sleep.c
        hal/utils/src/utils_assert.c
        hal/utils/src/utils_event.c
        hal/utils/src/utils_list.c
        hpl/core/hpl_core_m0plus_base.c
        hpl/core/hpl_init.c
        hpl/dac/hpl_dac.c
        hpl/dmac/hpl_dmac.c
        hpl/gclk/hpl_gclk.c
        hpl/pm/hpl_pm.c
        hpl/sysctrl/hpl_sysctrl.c
        hpl/tc/tc_lite.c
        samd21d/gcc/gcc/startup_samd21.c
        samd21d/gcc/system_samd21.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM0/port.c
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_2.c
        )

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        -D__SAMD21G17D__
        )

# List of include directories
target_include_directories(${EXECUTABLE} PRIVATE
        src 
        CMSIS
        CMSIS/Core/Include
        config
        hal/include
        hal/utils/include
        hpl/core
        hpl/include
        hpl/port
        hpl/tc
        hpl/gclk
        hpl/pm
        hri
        samd21d/include
        samd21d/include/component
        samd21d/include/pio
        thirdparty/RTOS
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include
        thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM0
        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m0plus
        -mthumb

        -fdata-sections
        -ffunction-sections

        -Wall
        -O0
        -g3
        )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${LINKER_FILE}
        -mcpu=cortex-m0plus
        -mthumb
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -Xlinker -print-memory-usage -Xlinker
        )

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)