
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
    STATUS_CONTINUE = 100,
    STATUS_OK = 200,
    STATUS_CREATED = 201,
    STATUS_LOW_ON_STORAGE_SPACE = 250,
    STATUS_MULTIPLE_CHOICES = 300,
    STATUS_MOVED_PERMANENTLY = 301,
    STATUS_MOVED_TEMPORARILY = 302,
    STATUS_SEE_OTHER = 303,
    STATUS_NOT_MODIFIED = 304,
    STATUS_USE_PROXY = 305,
    STATUS_BAD_REQUEST = 400,
    STATUS_UNAUTHORIZED = 401,
    STATUS_PAYMENT_REQUIRED = 402,
    STATUS_FORBIDDEN = 403,
    STATUS_NOT_FOUND = 404,
    STATUS_METHOD_NOT_ALLOWED = 405,
    STATUS_NOT_ACCEPTABLE = 406,
    STATUS_PROXY_AUTH_REQUIRED = 407,
    STATUS_REQUEST_TIME_OUT = 408,
    STATUS_GONE = 410,
    STATUS_LENGTH_REQUIRED = 411,
    STATUS_PRECONDITION_FAILED = 412,
    STATUS_REQUEST_ENTITY_TOO_LARGE = 413,
    STATUS_REQUEST_URI_TOO_LARGE = 414,
    STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
    STATUS_PARAMETER_NOT_UNDERSTOOD = 451,
    STATUS_CONFERENCE_NOT_FOUND = 452,
    STATUS_NOT_ENOUGH_BANDWIDTH = 453,
    STATUS_SESSION_NOT_FOUND = 454,
    STATUS_METHOD_NOT_VALID_IN_THIS_STATE = 455,
    STATUS_HEADER_FIELD_NOT_VALID_FOR_RESOURCE = 456,
    STATUS_INVALID_RANGE = 457,
    STATUS_PARAMETER_IS_READONLY = 458,
    STATUS_AGGREGATE_OPERATION_NOT_ALLOWED = 459,
    STATUS_ONLY_AGGREGATE_OPERATION_ALLOWED = 460,
    STATUS_UNSUPPORTED_TRANSPORT = 461,
    STATUS_DESTINATION_UNREACHABLE = 462,
    STATUS_INTERNAL_SERVER_ERROR = 500,
    STATUS_NOT_IMPLEMENTED = 501,
    STATUS_BAD_GATEWAY = 502,
    STATUS_SERVICE_UNAVAILABLE = 503,
    STATUS_GATEWAY_TIMEOUT = 504,
    STATUS_RTSP_VERSION_NOT_SUPPORTED = 505,
    STATUS_OPERATION_NOT_SUPPORTED = 551
};

CK_API cppkit::ck_string get_status_message( status s );
CK_API status convert_status_code_from_int( int v );

}

#endif
