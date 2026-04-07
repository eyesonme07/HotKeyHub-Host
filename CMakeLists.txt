cmake_minimum_required(VERSION 3.10)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("HotKeyHub_Host")

set(BOOST_ROOT "C:/Boost/boost_1_90_0")
find_package(Boost REQUIRED)

# === Создаём цель ===
add_executable(HotKeyHub_Host 
    "src/cpp/main.cpp"
    "src/cpp/server.cpp"
    "src/cpp/session.cpp"
    "src/cpp/systemController.cpp"
    "src/header/server.h"
    "src/header/session.h"
    "src/header/systemController.h"
)

target_compile_definitions(HotKeyHub_Host PRIVATE 
    BOOST_JSON_NO_LIB
    BOOST_ALL_NO_LIB
)

target_link_libraries(HotKeyHub_Host PRIVATE Boost::headers)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET HotKeyHub_Host PROPERTY CXX_STANDARD 20)
endif()