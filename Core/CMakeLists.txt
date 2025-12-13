cmake_minimum_required(VERSION 3.16)

project(Core LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

FetchContent_Declare(
    msgpack-cxx
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
    GIT_TAG        cpp-7.0.0)
FetchContent_MakeAvailable(msgpack-cxx)

FetchContent_Declare(
    libsodium
    GIT_REPOSITORY https://github.com/Sania2280/sodium
    GIT_TAG master
)

FetchContent_MakeAvailable(libsodium)

message(STATUS "libsodium source dir: ${libsodium_SOURCE_DIR}")
include_directories("C:/OpenSSL/build/include")
link_directories("C:/OpenSSL/build/lib64")



add_library(Core STATIC
  Mpack.hpp
  Mpack.cpp
  message.h
  enums.h
  keygenerator.h keygenerator.cpp
  aes.h aes.cpp
)

message(STATUS "libsodium_SOURCE_DIR = ${libsodium_SOURCE_DIR}")


target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${libsodium_SOURCE_DIR}/libsodium-win64/include

)

target_link_directories(Core
    PUBLIC
        ${libsodium_SOURCE_DIR}/libsodium-win64/lib
)

target_link_libraries(Core
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core

    PUBLIC
        msgpack-cxx
        libsodium
)

target_link_libraries(Core
    PUBLIC
        msgpack-cxx
        libsodium
        "C:/OpenSSL/build/lib64/libssl.a"
        "C:/OpenSSL/build/lib64/libcrypto.a"
        ws2_32
        gdi32
        crypt32
)

target_compile_definitions(Core PRIVATE CORE_LIBRARY)

