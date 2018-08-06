cmake_minimum_required(VERSION 3.10)
project(shiro C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake/")

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

# Threading library (Win32 threads, pthreads)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_LINK_LIBS ${CMAKE_LINK_LIBS} Threads::Threads)

# Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS system)

# cURL
find_package(CURL REQUIRED)

# OpenSSL
find_package(OpenSSL REQUIRED)

if (NOT OPENSSL_CRYPTO_LIBRARY)
    message(FATAL_ERROR "Unable to find libcrypto provided by OpenSSL.")
endif()

# Hinnant Date (required by sqlpp11)
find_package(HinnantDate REQUIRED)

# Tcmalloc
find_package(Tcmalloc)

if (Tcmalloc_FOUND)
    set(CMAKE_LINK_LIBS ${CMAKE_LINK_LIBS} ${Tcmalloc_LIBRARIES})

    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-realloc -fno-builtin-free")
    endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS} -Og -g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS} -O3")

# Enable logging with streams in Loguru
add_definitions(-DLOGURU_WITH_STREAMS=1)

if (WIN32)
    set(CMAKE_LINK_LIBS ${CMAKE_LINK_LIBS})
elseif (APPLE)
    set(CMAKE_LINK_LIBS dl sqlpp-mysql mysqlclient crypto ${CMAKE_LINK_LIBS})
elseif (UNIX) # Linux
    set(CMAKE_LINK_LIBS dl sqlpp-mysql mysqlclient crypto ${CMAKE_LINK_LIBS})
endif()

include_directories(${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${HinnantDate_INCLUDE_DIR})
add_executable(shiro ${SRC} src/spectating/spectator_manager.cc src/spectating/spectator_manager.hh src/handlers/spectating/start_spectating_handler.cc src/handlers/spectating/start_spectating_handler.hh src/handlers/spectating/stop_spectating_handler.cc src/handlers/spectating/stop_spectating_handler.hh src/handlers/spectating/spectator_frames_handler.cc src/handlers/spectating/spectator_frames_handler.hh src/handlers/spectating/cant_spectate_handler.cc src/handlers/spectating/cant_spectate_handler.hh)
target_link_libraries(shiro ${Boost_LIBRARIES} ${CURL_LIBRARIES} ${CMAKE_LINK_LIBS})
