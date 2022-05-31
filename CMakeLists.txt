cmake_minimum_required(VERSION 3.18)
project(serialportAssistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets SerialPort)
set(REQUIRED_LIBS_QUALIFIED
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::SerialPort
        )

add_executable(${PROJECT_NAME}
        main.cc
        #ui类
        mainwindow.cc
        mainwindow.hh
        #qrc文件
        resource/res.qrc
        )

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})