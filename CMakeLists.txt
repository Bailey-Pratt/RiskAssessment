cmake_minimum_required(VERSION 3.5.0)
project(MyCMakeproject VERSION 0.1.0 LANGUAGES CXX) 

# Add MySQL paths specifically for Mac
set(MYSQL_DIR "/usr/local/mysql/")
set(MYSQL_INCLUDE_DIRS "/usr/local/include/mysql")
set(MYSQL_LIBRARY_DIRS "/usr/local/lib")

include_directories(${MYSQL_INCLUDE_DIRS})
link_directories(${MYSQL_LIBRARY_DIRS})

add_executable(RiskAssessment main-3.cpp)
target_link_libraries(RiskAssessment mysqlclient)

set_property(TARGET RiskAssessment PROPERTY CXX_STANDARD 23)
