if (UNIX)
	message(STATUS "unix system detected using mingw")
	set(CMAKE_TOOLCHAIN_FILE "./cmake/mingw-w64-x86_64.cmake")
endif()

cmake_minimum_required(VERSION 3.14)
project("SataAndagi")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive -O3 -w")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable("${CMAKE_PROJECT_NAME}" WIN32 src/main.cpp)
target_link_libraries("${CMAKE_PROJECT_NAME}" gdi32 winmm user32)






