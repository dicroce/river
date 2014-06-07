
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// river
/// Copyright (c) 2014, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef river_status_h
#define river_status_h

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_exports.h"

namespace river
{

enum status
{
    S_CONTINUE = 100,
    S_OK = 200,
    S_CREATED = 201,
    S_LOW_ON_STORAGE_SPACE = 250,
    S_MULTIPLE_CHOICES = 300,
    S_MOVED_PERMANENTLY = 301,
    S_MOVED_TEMPORARILY = 302,
    S_SEE_OTHER = 303,
    S_NOT_MODIFIED = 304,
    S_USE_PROXY = 305,
    S_BAD_REQUEST = 400,
    S_UNAUTHORIZED = 401,
    S_PAYMENT_REQUIRED = 402,
    S_FORBIDDEN = 403,
    S_NOT_FOUND = 404,
    S_METHOD_NOT_ALLOWED = 405,
    S_NOT_ACCEPTABLE = 406,
    S_PROXY_AUTH_REQUIRED = 407,
    S_REQUEST_TIME_OUT = 408,
    S_GONE = 410,
    S_LENGTH_REQUIRED = 411,
    S_PRECONDITION_FAILED = 412,
    S_REQUEST_ENTITY_TOO_LARGE = 413,
    S_REQUEST_URI_TOO_LARGE = 414,
    S_UNSUPPORTED_MEDIA_TYPE = 415,
    S_PARAMETER_NOT_UNDERSTOOD = 451,
    S_CONFERENCE_NOT_FOUND = 452,
    S_NOT_ENOUGH_BANDWIDTH = 453,
    S_SESSION_NOT_FOUND = 454,
    S_METHOD_NOT_VALID_IN_THIS_STATE = 455,
    S_HEADER_FIELD_NOT_VALID_FOR_RESOURCE = 456,
    S_INVALID_RANGE = 457,
    S_PARAMETER_IS_READONLY = 458,
    S_AGGREGATE_OPERATION_NOT_ALLOWED = 459,
    S_ONLY_AGGREGATE_OPERATION_ALLOWED = 460,
    S_UNSUPPORTED_TRANSPORT = 461,
    S_DESTINATION_UNREACHABLE = 462,
    S_INTERNAL_SERVER_ERROR = 500,
    S_NOT_IMPLEMENTED = 501,
    S_BAD_GATEWAY = 502,
    S_SERVICE_UNAVAILABLE = 503,
    S_GATEWAY_TIMEOUT = 504,
    S_RTSP_VERSION_NOT_SUPPORTED = 505,
    S_OPERATION_NOT_SUPPORTED = 551
};

CK_API cppkit::ck_string get_status_message( status s );
CK_API status convert_status_code_from_int( int v );

}

#endif
