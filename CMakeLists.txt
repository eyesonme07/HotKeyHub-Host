cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0141 NEW)

project("HotKeyHub_Host")

set(CMAKE_VERBOSE_MAKEFILE ON)

set(BOOST_ROOT "C:/Boost/boost_1_90_0")
find_package(Boost REQUIRED)

add_executable(HotKeyHub_Host 
    "src/cpp/main.cpp"
    "src/cpp/server.cpp"
    "src/cpp/session.cpp"
    "src/cpp/systemController.cpp"
    "src/header/server.h"
    "src/header/session.h"
    "src/header/systemController.h"
 "src/header/jsonParse.h" "src/cpp/jsonParse.cpp")

set_property(TARGET HotKeyHub_Host PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

if(WIN32)
    target_sources(HotKeyHub_Host PRIVATE app.rc)
endif()

target_compile_definitions(HotKeyHub_Host PRIVATE 
    BOOST_JSON_NO_LIB
    BOOST_ALL_NO_LIB
)

target_link_libraries(HotKeyHub_Host PRIVATE Boost::headers)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET HotKeyHub_Host PROPERTY CXX_STANDARD 20)
endif()