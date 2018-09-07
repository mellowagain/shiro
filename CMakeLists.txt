cmake_minimum_required(VERSION 3.10)
project(shiro C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-unused-variable -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
endif()

# Threading library (Win32 threads, pthreads)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS system iostreams)

# cURL
find_package(CURL REQUIRED)

# OpenSSL
find_package(OpenSSL REQUIRED)

if (NOT OPENSSL_CRYPTO_LIBRARY)
    message(FATAL_ERROR "Unable to find libcrypto provided by OpenSSL.")
endif()

# mcrypt
find_package(Mcrypt REQUIRED)

# zlib
find_package(ZLIB REQUIRED)

# liblzma
find_package(LibLZMA REQUIRED)

# Hinnant Date
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

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS} -O0 -g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS} -O3 -DNDEBUG=1")

# Enable logging with streams in Loguru
add_definitions(-DLOGURU_WITH_STREAMS=1 -DLOGURU_REDEFINE_ASSERT=1)

# Resources
file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

foreach (RESOURCE ${RESOURCES})
    get_filename_component(RESOURCE_BASENAME ${RESOURCE} NAME)
    configure_file("${RESOURCE}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESOURCE_BASENAME}" COPYONLY)
endforeach()

if (WIN32)
    set(CMAKE_LINK_LIBS ${CMAKE_LINK_LIBS})
elseif (APPLE)
    set(CMAKE_LINK_LIBS dl sqlpp-mysql mysqlclient crypto stdc++fs ${CMAKE_LINK_LIBS})
elseif (UNIX) # Linux
    set(CMAKE_LINK_LIBS dl sqlpp-mysql mysqlclient crypto stdc++fs ${CMAKE_LINK_LIBS})
endif()

include_directories(${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${HinnantDate_INCLUDE_DIR} ${Mcrypt_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS} ${LIBLZMA_INCLUDE_DIRS})
add_executable(shiro ${SRC})
target_link_libraries(shiro Threads::Threads ${Boost_LIBRARIES} ${CURL_LIBRARIES} ${Mcrypt_LIBS} ${ZLIB_LIBRARIES} ${LIBLZMA_LIBRARIES} ${CMAKE_LINK_LIBS})
