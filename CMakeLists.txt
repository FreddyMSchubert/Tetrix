cmake_minimum_required(VERSION 3.14)
project(tetrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wshadow -g -fsanitize=address")

include(FetchContent)

find_package(raylib CONFIG QUIET)

if(NOT raylib_FOUND)
	message(STATUS "raylib not found, fetching from repository...")
	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG 4.5.0  # Specify the version you need
	)
	FetchContent_MakeAvailable(raylib)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(tetrix ${SOURCES})

target_link_libraries(tetrix raylib)

if(APPLE)
	target_link_libraries(tetrix "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()
