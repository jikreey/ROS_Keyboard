cmake_minimum_required(VERSION 2.8.3)
project(ardrone_keyboard_controller)

find_package(catkin REQUIRED COMPONENTS 
roscpp 
std_msgs 
message_generation
cv_bridge
image_transport
sensor_msgs)

find_package(SDL REQUIRED)
set(LIBS ${SDL_LIBRARY})

find_package( OpenCV REQUIRED )

#######################################
## Declare ROS messages and services ##
#######################################
add_message_files(
  FILES
  Key.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

###################################
## catkin specific configuration ##
###################################

catkin_package(
  #INCLUDE_DIRS include
#  LIBRARIES keyboard
  CATKIN_DEPENDS 
  roscpp 
  std_msgs 
  message_runtime 
  cv_bridge 
  image_transport 
  sensor_msgs
  DEPENDS ${LIBS}
)

###########
## Build ##
###########
include_directories(include)
include_directories(
${catkin_INCLUDE_DIRS} 
${SDL_INCLUDE_DIR}
${OpenCV_INCLUDE_DIRS})

add_executable(keyboard src/main_keyboard.cpp src/keyboard.cpp)
target_link_libraries(keyboard
  ${LIBS}
  ${catkin_LIBRARIES}
)
add_dependencies(keyboard ardrone_keyboard_controller_gencpp)

add_executable(ardrone src/main_ardrone.cpp)
add_dependencies(ardrone ardrone_keyboard_controller_generate_messages_cpp)
target_link_libraries(ardrone 
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)
#############
## Install ##
#############


install(TARGETS keyboard
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
