cmake_minimum_required(VERSION 2.8)
project(river)

set(SOURCES source/river_exception.cpp
	    include/river/river_exception.h
            source/methods.cpp
	    include/river/methods.h
            source/status.cpp
	    include/river/status.h
            source/client_connection.cpp
	    include/river/client_connection.h
            source/server_connection.cpp
	    include/river/server_connection.h
            source/client_request.cpp
	    include/river/client_request.h
            source/client_response.cpp
	    include/river/client_response.h
            source/server_request.cpp
	    include/river/server_request.h
            source/server_response.cpp
	    include/river/server_response.h
            source/rtsp_server.cpp
	    include/river/rtsp_server.h
            source/rtp.cpp
	    include/river/rtp.h)

set(COMMON_LIBS cppkit)

include(common.cmake NO_POLICY_SCOPE)
include(../devel_artifacts/build/base_lib.cmake NO_POLICY_SCOPE)
