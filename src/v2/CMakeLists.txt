cmake_minimum_required(VERSION 3.9.1)
project(AES)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIB aes_lib/aes.lib.h
        aes_lib/aes.cipherblock.lib.h
        aes_lib/types/aes.types.h
        aes_lib/types/aes.types.cpp
        aes_lib/modes/aes.modes.h
        aes_lib/modes/aes.modes.cpp
        aes_lib/cipherblock/aes.cipherblock.h
        aes_lib/cipherblock/aes.cipherblock.cpp
        aes_lib/cipherblock/aes.cipherblock.constants.h
    )

set(SOURCES aes.cpp 
            ${LIB}        
    )
add_executable(aes ${SOURCES})