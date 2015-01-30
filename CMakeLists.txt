cmake_minimum_required(VERSION 2.8)

# Enable release symbols by default
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build.")
endif()

# Set project name
set(PROJECT_NAME game_of_life)
project(${PROJECT_NAME})

IF (WIN32)
  add_subdirectory("deps/glfw-3.1")
  include_directories("deps/glfw-3.1/include")
ELSE()
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
ENDIF()

find_package(OpenGL REQUIRED)

# Set compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "GCC detected, adding compile flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -W -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNZS_DISABLE_LOG")

set(SRC_DIR "src")
set(INC_DIR "include")
file(GLOB SRC_LIST "${SRC_DIR}/*" "${INC_DIR}/*")

file(COPY "data/brushs.txt" DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${INC_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
