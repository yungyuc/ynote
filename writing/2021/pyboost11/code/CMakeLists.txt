cmake_minimum_required(VERSION 3.9)
project(pyboost11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BPYVER "python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")

set(PYBOOST11_CXX_FLAGS
  "-g -Werror -Wall -Wextra -Wno-unused-value -Wno-noexcept-type")
set(CMAKE_CXX_FLAGS "${PYBOOST11_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Boost COMPONENTS ${BPYVER} REQUIRED)

################################################################################
# pybind11 "modpyb"
################################################################################

include_directories(${pybind11_INCLUDE_DIRS})

pybind11_add_module(
    modpyb
    MODULE
    modpyb.cpp
)
target_link_libraries(modpyb PRIVATE pybind11::module ${Boost_LIBRARIES})
set_target_properties(modpyb PROPERTIES CXX_VISIBILITY_PRESET "default")

add_custom_target(modpyb_copy
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:modpyb> ${PROJECT_SOURCE_DIR}
    DEPENDS modpyb)

################################################################################
# boost.python "modbpy"
################################################################################

add_library(modbpy MODULE modbpy.cpp)
target_link_libraries(modbpy ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(modbpy PRIVATE ${PYTHON_INCLUDE_DIRS})
set_target_properties(modbpy PROPERTIES
  CXX_VISIBILITY_PRESET "default"
  PREFIX ""
  SUFFIX ".${Python3_SOABI}.so")

add_custom_target(modbpy_copy
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:modbpy> ${PROJECT_SOURCE_DIR}
    DEPENDS modbpy)

# vim: set fenc=utf8 ff=unix et sw=2 ts=2 sts=2:
