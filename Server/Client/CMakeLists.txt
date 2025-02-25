# Client/CMakeLists.txt

project(Client LANGUAGES CXX)

# Находим Qt6 и требуемые модули
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    config.h
    enums.h
    m_pack.h

    resources.qrc
)

# Создаем исполняемый файл
qt_add_executable(Client
    ${PROJECT_SOURCES}
)

# Линкуем Qt модули
target_link_libraries(Client PRIVATE
    Qt6::Widgets
    Qt6::Network
)

# Устанавливаем путь для конфигурационного файла
configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)

# Указываем директорию сборки
set_target_properties(Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

