cmake_minimum_required(VERSION 3.10)
project(CyberRiskAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon REQUIRED)

add_executable(cyber_risk_api
    main.cpp
    backend.cpp
)

target_link_libraries(cyber_risk_api PRIVATE Drogon::Drogon)