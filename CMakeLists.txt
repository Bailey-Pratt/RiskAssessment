cmake_minimum_required(VERSION 3.5.0)
project(MyCMakeproject VERSION 0.1.0 LANGUAGES CXX) 

set(MYSQL_DIR "/usr/local/mysql/")
set(MYSQL_INCLUDE_DIRS "/usr/local/mysql/include")
set(MYSQL_LIBRARY_DIRS "/usr/local/mysql/lib")

include_directories(${MYSQL_INCLUDE_DIRS})
link_directories(${MYSQL_LIBRARY_DIRS})

add_executable(LoginServer main-3.cpp)
target_link_libraries(LoginServer mysqlclient)

set_property(TARGET LoginServer PROPERTY CXX_STANDARD 23)
