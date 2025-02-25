cmake_minimum_required(VERSION 3.16)

project(Lommer1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматическую обработку MOC, UIC и RCC для Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Добавляем подкаталоги Client и Server
add_subdirectory(Client)
add_subdirectory(Server)

