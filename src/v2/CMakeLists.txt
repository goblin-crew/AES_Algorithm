cmake_minimum_required(VERSION 3.9.1)
project(AES128)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIB lib/aes128.lib.h
        lib/aes128.lib.cpp
        lib/types/types.h
        lib/constants/constants.h
        lib/tools/converters.h
        lib/tools/converters.cpp
        lib/aes/key.h
        lib/aes/key.cpp
        lib/aes/statematrix.h
        lib/aes/statematrix.cpp
    )

set(SOURCES aes.cpp 
            ${LIB}        
    )
add_executable(aes ${SOURCES})