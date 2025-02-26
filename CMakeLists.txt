cmake_minimum_required(VERSION 3.16)

project(Lommer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(CheckIncludeFile)
include(CheckIncludeFiles)

# Включаем автоматическую обработку MOC, UIC и RCC для Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MSGPACK_USE_BOOST OFF) #Disable use boost

FetchContent_Declare(
    msgpack-cxx
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
    GIT_TAG        cpp-7.0.0)
FetchContent_MakeAvailable(msgpack-cxx)

add_subdirectory(ClientDesktop)
add_subdirectory(Server)
