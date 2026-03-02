cmake_minimum_required(VERSION 3.16)

project(GBCartFlasher VERSION 1.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

## Add libftdi, making sure we don't build more than necessary
set(SHAREDLIBS OFF CACHE BOOL "Build shared/dynamic libraries" FORCE)
set(EXAMPLES OFF CACHE BOOL "Build example programs" FORCE)
set(FTDI_EEPROM OFF CACHE BOOL "Build ftdi_eeprom" FORCE)

add_subdirectory(vendor/libftdi)

set(TS_FILES
        src/translations/gbcflsh_english.ts
        src/translations/gbcflsh_french.ts
        src/translations/gbcflsh_german.ts
        src/translations/gbcflsh_polish.ts
)

set(ASSET_FILES
        src/assets/about.xpm
        src/assets/flasher.xpm
        src/assets/icon.xpm
)

SET(PROJECT_HEADERS
        src/headers/About.h
        src/headers/AbstractPort.h
        src/headers/Console.h
        src/headers/const.h
        src/headers/EraseThread.h
        src/headers/Gui.h
        src/headers/Logic.h
        src/headers/ReadFlashThread.h
        src/headers/ReadRamThread.h
        src/headers/Settings.h
        src/headers/WriteFlashThread.h
        src/headers/WriteRamThread.h
)

set(PROJECT_SOURCES
        src/gbcflsh.cpp
        src/EraseThread.cpp
        src/Logic.cpp
        src/ReadFlashThread.cpp
        src/ReadRamThread.cpp
        src/Settings.cpp
        src/WriteFlashThread.cpp
        src/WriteRamThread.cpp
        src/views/About.cpp
        src/views/Gui.cpp
        ${PROJECT_HEADERS}
        ${TS_FILES}
        ${ASSET_FILES}
)

if (WIN32)
    list(APPEND PROJECT_SOURCES
            src/headers/platform/windows/SerialPortWin.h
            src/headers/platform/windows/USBPortWin.h
            src/platform/windows/SerialPortWin.cpp
            src/platform/windows/USBPortWin.cpp
    )
else ()
    list(APPEND PROJECT_SOURCES
            src/headers/platform/linux/SerialPort.h
            src/headers/platform/linux/USBPort.h
            src/platform/linux/SerialPort.cpp
            src/platform/linux/USBPort.cpp
    )
endif ()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GBCartFlasher
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )

    qt_add_translations(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    add_executable(GBCartFlasher
            ${PROJECT_SOURCES}
    )

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(GBCartFlasher PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ftdi1-static
)

# libftdi doesn't seem to make headers available to us until we install?
target_include_directories(GBCartFlasher PUBLIC vendor/libftdi/src)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.ixalsera.gbcartflasher)
endif ()
set_target_properties(GBCartFlasher PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GBCartFlasher
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GBCartFlasher)
endif ()
