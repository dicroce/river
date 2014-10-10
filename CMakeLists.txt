cmake_minimum_required(VERSION 2.8)
project(river)

set(SOURCES source/river_exception.cpp
            source/methods.cpp
            source/status.cpp
            source/client_connection.cpp
            source/server_connection.cpp
            source/client_request.cpp
            source/client_response.cpp
            source/server_request.cpp
            source/server_response.cpp
            source/rtsp_server.cpp
            source/rtp.cpp)

set(COMMON_LIBS cppkit)

include(common.cmake NO_POLICY_SCOPE)
include(../devel_artifacts/build/base_lib.cmake NO_POLICY_SCOPE)
