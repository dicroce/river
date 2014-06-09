cmake_minimum_required(VERSION 2.8)
project(ut)

set(SOURCES source/framework.cpp
            source/river_client_connection_test.cpp
            source/river_client_request_test.cpp
            source/river_server_response_test.cpp
            source/river_rtsp_server_test.cpp)

set(COMMON_LIBS cppkit river)
set(WINDOWS_LIBS DbgHelp ws2_32 iphlpapi)

include(../../devel_artifacts/build/base_ut.cmake)
