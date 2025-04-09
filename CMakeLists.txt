cmake_minimum_required(VERSION 3.16)

project(login VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)  # Ensure debug build mode
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 components (Added MultimediaWidgets for QVideoWidget)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Multimedia MultimediaWidgets)

# Copy images directory to build directory
file(COPY ${CMAKE_SOURCE_DIR}/images DESTINATION ${CMAKE_BINARY_DIR})

# Define project sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h
    LoginPage.cpp LoginPage.h LoginPage.ui
    AdminPage.cpp AdminHomePage.h
    DatabaseManager.cpp DatabaseManager.h
    StudentHomePage.cpp StudentHomePage.h
    TeacherHomePage.cpp TeacherHomePage.h
    registerUser.cpp registerUser.h
    resources.qrc
    BackButton.cpp BackButton.h
    createcourse.cpp createcourse.h
    allusers.cpp allusers.h
    ProfilePage.cpp ProfilePage.h
    allCourses.cpp allCourses.h
    enrollStudent.cpp enrollStudent.h
    uploadpdf.cpp uploadpdf.h
    uploadlecture.cpp uploadlecture.h  # Ensure both .cpp and .h are included
    TopBar.cpp TopBar.h
)

# Create executable
qt_add_executable(login MANUAL_FINALIZATION ${PROJECT_SOURCES}
    mesgboxutil.h
    mesgboxutil.cpp)

# Link Qt components (Added MultimediaWidgets)
target_link_libraries(login PRIVATE Qt6::Widgets Qt6::Sql Qt6::Multimedia Qt6::MultimediaWidgets)

# Install instructions
include(GNUInstallDirs)
install(TARGETS login
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize Qt executable
qt_finalize_executable(login)

# Include directory
include_directories(${CMAKE_SOURCE_DIR}/include)
