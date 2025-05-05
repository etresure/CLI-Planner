cmake_minimum_required(VERSION 3.11)
project(project_main VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TASKS_SOURCES
    source/tasks/task.cpp
    source/tasks/urgent_collab.cpp
    source/tasks/reminder.cpp
)

set(MODEL_SOURCES
    source/model/project.cpp
    source/model/user.cpp
)

set(SOURCES
    ${TASKS_SOURCES}
    ${MODEL_SOURCES}
)

add_library(mainlib STATIC ${SOURCES})

target_include_directories(mainlib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(mainlib PUBLIC cxx_std_17)
add_executable(main_test main_test.cpp)
target_link_libraries(main_test PRIVATE mainlib)
