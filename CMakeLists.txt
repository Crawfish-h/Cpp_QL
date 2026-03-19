cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project(Cpp_Ql CXX)

find_package(Boost REQUIRED)

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/Main.cpp
    ${PROJECT_SOURCE_DIR}/Typedefs.h
    ${PROJECT_SOURCE_DIR}/Util.h
    ${PROJECT_SOURCE_DIR}/Result.h
    ${PROJECT_SOURCE_DIR}/Print.h
    ${PROJECT_SOURCE_DIR}/Match.h
    ${PROJECT_SOURCE_DIR}/Environment.cpp
    ${PROJECT_SOURCE_DIR}/Environment.h
    ${PROJECT_SOURCE_DIR}/Type.h
    ${PROJECT_SOURCE_DIR}/Type.cpp
    ${PROJECT_SOURCE_DIR}/Database.h
    ${PROJECT_SOURCE_DIR}/Database.cpp
    ${PROJECT_SOURCE_DIR}/Table.h
    ${PROJECT_SOURCE_DIR}/Table.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})