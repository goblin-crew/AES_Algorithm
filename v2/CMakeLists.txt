cmake_minimum_required(VERSION 3.9.1)
project(AES128)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIB lib/aes128.lib.h
        lib/HelloWorld/HelloWorld.h
        lib/HelloWorld/HelloWorld.cpp
    )

set(SOURCES main.cpp 
            ${LIB}        
    )
add_executable(aes128 ${SOURCES})