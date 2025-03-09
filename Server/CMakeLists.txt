# Client/CMakeLists.txt

project(Server LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

file(GLOB_RECURSE Server_SRC CONFIGURE_DEPENDS "*.h" "*.hpp" "*.cpp" "*.qrc")

qt_add_executable(Server
    ${Server_SRC}
)

target_link_libraries(Server
    PRIVATE
        Qt6::Core
        Qt6::Network
        Core
)

# Устанавливаем путь для конфигурационного файла
configure_file(config.json ${CMAKE_BINARY_DIR}/bin/config.json COPYONLY)

set_target_properties(Server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
