cmake_minimum_required(VERSION 3.10)
project(shiro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

file(GLOB_RECURSE SRC src/*.cc src/*.hh)

if (NOT CMAKE_BUILD_TYPE)
    message(WARNING "No build type has been selected, defaulting to Release.")
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif()

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS} -Og -g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS} -O3")

# Enable logging with streams in Loguru
add_definitions(-DLOGURU_WITH_STREAMS=1)

if (WIN32)
    set(CMAKE_LINK_LIBS)
elseif (APPLE)
    set(CMAKE_LINK_LIBS ${CMAKE_LINK_LIBS} c stdc++ mysqlpp)
elseif (UNIX) # Linux
    set(CMAKE_LINK_LIBS ${CMAKE_LINK_LIBS} c stdc++ stdc++fs pthread dl mysqlpp)
endif()

add_executable(shiro ${SRC})
target_link_libraries(shiro ${CMAKE_LINK_LIBS})
