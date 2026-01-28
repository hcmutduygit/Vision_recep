cmake_minimum_required(VERSION 3.5)
project(robot_vision)

# --- THÊM DÒNG NÀY ĐỂ FIX WARNING ---
set(CMAKE_BUILD_TYPE Release)
# ------------------------------------

find_package(CUDA REQUIRED)

# Các đường dẫn giữ nguyên như bạn đã làm đúng
set(ZED_INCLUDE_DIRS "/usr/local/zed/include")
set(ZED_LIBRARY_DIR "/usr/local/zed/lib")

include_directories(${ZED_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})
include_directories("/usr/local/cuda/include")

add_executable(robot_vision main.cpp)

find_library(ZED_LIBRARY NAMES sl_zed HINTS ${ZED_LIBRARY_DIR})

target_link_libraries(robot_vision ${ZED_LIBRARY})
target_link_libraries(robot_vision ${CUDA_LIBRARIES})
target_link_libraries(robot_vision ${CUDA_CUDA_LIBRARY})
target_link_libraries(robot_vision ${CUDA_CUDART_LIBRARY})
target_link_libraries(robot_vision -lpthread)

set_target_properties(robot_vision PROPERTIES CXX_STANDARD 14)