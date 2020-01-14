cmake_minimum_required (VERSION 2.8)
# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

# Create an executable file called helloworld from sources:
# add all files in the current directory
file(GLOB CPSCHEDULER_SRCS "*.cpp")

add_executable(CPSCHEDULER ${CPSCHEDULER_SRCS})

include_directories(${CPLEX_INC_DIR})
include_directories(${CONCERT_INC_DIR})
include_directories(${CP_INC_DIR})

target_link_libraries(CPSCHEDULER ${CP_STATIC_LIBRARY} dl)
target_link_libraries(CPSCHEDULER ${CONCERT_STATIC_LIBRARY} dl)
target_link_libraries(CPSCHEDULER ${CPLEX_STATIC_LIBRARY} dl)
target_link_libraries(CPSCHEDULER ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(CPSCHEDULER m)
