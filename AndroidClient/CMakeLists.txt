cmake_minimum_required(VERSION 3.16)

project(mobApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

# ------ Основной исполняемый файл ------
qt_add_executable(appmobApp
    main.cpp
)

# ------ Подключаем QML модуль ------
qt_add_qml_module(appmobApp
    URI mobApp
    VERSION 1.0
     QML_FILES qml/Main.qml  # Указываем основной QML файл
    # QML_FILES qml/components/CountrySelector.qml
    # qml/pages/LoginPage.qml
    # qml/pages/PasswordPage.qml
    # qml/pages/RegisterPage.qml
)

# ------ Подключаем ресурсы (изображения и т.д.) ------
qt_add_resources(flags_resources assets/flags.qrc)
qt_add_resources(qml_resources qml/qml.qrc)

# ------ Добавляем ресурсы к таргету ------
target_sources(appmobApp PRIVATE
    ${flags_resources}
    ${qml_resources}
)

# ------ Настройки для MacOS/Windows ------
set_target_properties(appmobApp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ------ Линкуем библиотеки Qt ------
target_link_libraries(appmobApp
    PRIVATE Qt6::Quick
)

# ------ Установка ------
include(GNUInstallDirs)
install(TARGETS appmobApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ------ Логирование информации о проекте ------
message(STATUS "--------- Project Info ---------")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Qt Version: ${Qt6_VERSION}")
message(STATUS "---------------------------------")
