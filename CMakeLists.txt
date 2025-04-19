cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(bf)

file(GLOB_RECURSE SRC ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc/)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)

add_custom_target(
  run
  COMMAND ${CMAKE_COMMAND} -E echo
          "--------------------------------------------------"
  COMMAND ${CMAKE_COMMAND} -E echo ""
  COMMAND ${CMAKE_SOURCE_DIR}/run.sh
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}"
  COMMAND ${CMAKE_COMMAND} -E echo ""
  COMMAND ${CMAKE_COMMAND} -E echo
          "--------------------------------------------------"
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "building and running ${PROJECT_NAME}")
