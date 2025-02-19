cmake_minimum_required(VERSION 3.5.0)
project(MyCMakeproject VERSION 0.1.0 LANGUAGES CXX)

# Set MySQL paths - adjust these paths according to your MySQL installation
set(MYSQL_DIR "C:/Program Files/MySQL/MySQL Server 8.0")
set(MYSQL_INCLUDE_DIRS "${MYSQL_DIR}/include")
set(MYSQL_LIBRARY_DIRS "${MYSQL_DIR}/lib")

include_directories(${MYSQL_INCLUDE_DIRS})
link_directories(${MYSQL_LIBRARY_DIRS})

add_executable(LoginServer main-3.cpp)
target_link_libraries(LoginServer libmysql)

# Add Windows Sockets library
target_link_libraries(LoginServer wsock32 ws2_32)

set_property(TARGET LoginServer PROPERTY CXX_STANDARD 23)
