cmake_minimum_required(VERSION 3.16)

project(FinalApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Multimedia MultimediaWidgets)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Sql)

set(PROJECT_SOURCES
        mainwindow.cpp mainwindow.h
        LoginPage.cpp LoginPage.h LoginPage.ui
        AdminPage.cpp AdminHomePage.h
        DatabaseManager.cpp DatabaseManager.h
        StudentHomePage.cpp StudentHomePage.h
        TeacherHomePage.cpp TeacherHomePage.h
        registerUser.cpp registerUser.h
        BackButton.cpp BackButton.h
        createcourse.cpp createcourse.h
        allusers.cpp allusers.h
        ProfilePage.cpp ProfilePage.h
        allCourses.cpp allCourses.h
        enrollStudent.cpp enrollStudent.h
        uploadpdf.cpp uploadpdf.h
        mesgboxutil.cpp mesgboxutil.h
        uploadlecture.cpp uploadlecture.h  # Ensure both .cpp and .h are included
        TopBar.cpp TopBar.h     main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FinalApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        AllLecture.h
        AllLecture.cpp
        allassignments.cpp
        allassignments.h
        studentlecture.h
        studentlecture.cpp
        studentuploadpdf.h
        studentuploadpdf.cpp
        resources.qrc
        SupabaseConfig.h
        StudentAllCourses.cpp
        StudentAllCourses.h
    )
target_link_libraries(FinalApp PRIVATE Qt6::Widgets Qt6::Sql Qt6::Multimedia Qt6::MultimediaWidgets)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FinalApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FinalApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FinalApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FinalApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FinalApp)
endif()
set_target_properties(FinalApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FinalApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FinalApp)
endif()
