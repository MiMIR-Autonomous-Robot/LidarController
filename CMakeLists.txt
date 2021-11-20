cmake_minimum_required (VERSION 3.1.0)

project(newTest)

file(GLOB MAIN_APP_SRC
	"./src/*.h"
	"./src/*.cpp"
	"./src/hal/*.cpp"
	"./scr/hal/*.h"
	"./src/arch/linux/*.cpp"
	"./src/arch/linux/*.h"
	"./include/*.h"
	"*.cpp"
)

add_executable(main_app ${MAIN_APP_SRC})

target_include_directories(main_app PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(main_app PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/hal)
target_include_directories(main_app PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/arch/linux)
target_include_directories(main_app PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(main_app 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/rplidar_driver.lib
        Threads::Threads
)
