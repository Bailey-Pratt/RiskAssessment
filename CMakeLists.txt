
cmake_minimum_required(VERSION 3.5.0)
project(MyCMakeproject VERSION 0.1.0 LANGUAGES CXX) 

include(CTest)
enable_testing()

add_executable(RiskAssessment main.cpp)

set_property(TARGET MyCMakeproject PROPERTY CXX_STANDARD 23)