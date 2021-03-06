cmake_minimum_required (VERSION 2.8.11)
project (amicaLeap)

# use c++ 11
add_definitions(-std=c++11)

# The version number.
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Releases")

if( CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message ( STATUS "compiling for Windows systems" )
    set(CMAKE_PREFIX_PATH "C:/usr")
    # Widgets finds its own dependencies.
    set(QT5_CMAKE_HOME "C:/Qt/5.4/mingw491_32/lib/cmake")
    include_directories("C:/usr/leap/lib/MinGW/V2")
    link_directories("C:/usr/leap/lib/MinGW/V2")
else()
    #enable arms logging only on linux cause the mingw version of the lib does not still support it
    add_definitions(-DUSE_LEAP_ARM)

    message ( STATUS "compiling for unix systems" )
    set(CMAKE_PREFIX_PATH "~/usr")
    # Widgets finds its own dependencies.
    set(QT5_CMAKE_HOME "~/Qt5.4.1/5.4/gcc_64/lib/cmake")
    include_directories("~/.leap/include")
    link_directories("~/.leap/lib/x64")
endif()

set(CMAKE_PREFIX_PATH "/home/hara/usr")
message(WARNING "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
#########################################################


# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


#########################################################
#########################################################

#########################################################
# LEAP
#########################################################



# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
set(QT5_CMAKE_HOME "/home/hara/Qt5.4.1/5.4/gcc_64/lib/cmake")

find_package(Qt5Core PATHS ${QT5_CMAKE_HOME}/Qt5Core)
find_package(Qt5Widgets PATHS ${QT5_CMAKE_HOME}/Qt5Widgets)
find_package(Qt5Gui PATHS ${QT5_CMAKE_HOME}/Qt5Gui)
#find_package(Qt5OpenGL)
#find_package(Qt5Xml)

#qt5_add_resources(RESOURCES res.qrc)

QT5_WRAP_UI(UIS_HDRS
		mainwindow.ui)

add_executable(amicaLeap main.cpp
			mainwindow.cpp
			loggerconfig.cpp
			leaplogger.cpp
            ${UIS_HDRS} ${RESOURCES})

target_link_libraries(amicaLeap Qt5::Core Qt5::Gui Qt5::Widgets -lLeap)

set(CMAKE_CXX_FLAGS "-Wreorder")


