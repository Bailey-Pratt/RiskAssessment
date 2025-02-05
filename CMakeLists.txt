cmake_minimum_required(VERSION 3.10)
project(CyberRiskAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add PostgreSQL paths for Mac
set(PostgreSQL_INCLUDE_DIRS "/usr/local/opt/libpq/include")
set(PostgreSQL_LIBRARY_DIRS "/usr/local/opt/libpq/lib")

find_package(Drogon REQUIRED)

add_executable(cyber_risk_api
    main.cpp
    backend.cpp
)

target_include_directories(cyber_risk_api PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_directories(cyber_risk_api PRIVATE ${PostgreSQL_LIBRARY_DIRS})
target_link_libraries(cyber_risk_api PRIVATE Drogon::Drogon pq)
