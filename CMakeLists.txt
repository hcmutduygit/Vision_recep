cmake_minimum_required(VERSION 3.5)
project(ZED_Robot_Receptionist)

# Tìm gói ZED SDK
find_package(ZED 3 REQUIRED)

# Tìm gói CUDA (ZED cần cái này)
find_package(CUDA REQUIRED)

# Khai báo file chạy
add_executable(robot_vision main.cpp)

# Liên kết thư viện
target_link_libraries(robot_vision ${ZED_LIBRARIES})
target_link_libraries(robot_vision ${CUDA_LIBRARIES})
target_link_libraries(robot_vision ${CUDA_CUDA_LIBRARY})
target_link_libraries(robot_vision ${CUDA_CUDART_LIBRARY})

# Cấu hình chuẩn C++ 14
set_target_properties(robot_vision PROPERTIES CXX_STANDARD 14)