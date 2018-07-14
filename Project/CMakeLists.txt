
cmake_minimum_required(VERSION 2.6)
# Debug or release
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
set(PROJECT proj)
project(${PROJECT})

# Find OpenCV, instead handly write library name to link_libraries() option

find_package( OpenCV REQUIRED )

set(HEADERS SearchLines.h ApproxLines.h SearchLineOnField.h SearchGoal.h SearchBall.h Field.h)

set(SOURCES project.cpp SearchLines.cpp ApproxLines.cpp SearchLineOnField.cpp SearchGoal.cpp SearchBall.cpp Field.cpp)


link_libraries(X11 pthread)

# Add souce file to project for compile

add_executable (${PROJECT} ${HEADERS} ${SOURCES})


target_link_libraries( ${PROJECT} ${OpenCV_LIBS} )
