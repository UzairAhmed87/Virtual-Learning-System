cmake_minimum_required(VERSION 3.16)

project(login VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)  # Ensure debug build mode
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 or Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Define project sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    AdminPage.cpp
    AdminHomePage.h
    DatabaseManager.cpp
    DatabaseManager.h
    StudentHomePage.cpp
    StudentHomePage.h
    TeacherHomePage.cpp
    TeacherHomePage.h
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(login
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        AdminHomePage.h
        .gitignore
    )
else()
    if(ANDROID)
        add_library(login SHARED ${PROJECT_SOURCES})
    else()
        add_executable(login ${PROJECT_SOURCES})
    endif()
endif()

# Link required Qt components
target_link_libraries(login PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# macOS/iOS specific settings
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.login)
endif()

set_target_properties(login PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install instructions
include(GNUInstallDirs)
install(TARGETS login
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize Qt executable if using Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(login)
endif()
include_directories(${CMAKE_SOURCE_DIR}/include)
