
cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Debug)
set(PROJECT proj)
project(${PROJECT})

# Find OpenCV, instead handly write library name to link_libraries() option

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

set(HEADERS SearchLines.h ApproxLines.h SearchLineOnField.h SearchGoal.h SearchBall.h Field.h)

set(SOURCES project.cpp SearchLines.cpp ApproxLines.cpp SearchLineOnField.cpp SearchGoal.cpp SearchBall.cpp Field.cpp)


#link_libraries(X11 pthread -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_viz -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core)

# Add souce file to project for compile

add_executable (${PROJECT} ${HEADERS} ${SOURCES})


target_link_libraries( ${PROJECT} ${OpenCV_LIBS} )
