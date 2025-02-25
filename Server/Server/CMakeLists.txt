# Server/CMakeLists.txt

project(Server LANGUAGES CXX)

# Находим Qt6 и требуемые модули
find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(PROJECT_SOURCES
    main.cpp
    server.cpp
    server.h
    sending.cpp
    sending.h
    config.h
    enums.h
)

# Создаем исполняемый файл
qt_add_executable(Server
    ${PROJECT_SOURCES}
)

# Линкуем Qt модули
target_link_libraries(Server PRIVATE
    Qt6::Core
    Qt6::Network
)

# Указываем директорию сборки
set_target_properties(Server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

