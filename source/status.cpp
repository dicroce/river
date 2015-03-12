
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

#include "river/status.h"
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;

const static int S_UNDEFINED = 600;

ck_string river::get_status_message( status s )
{
    switch( s )
    {
    case STATUS_CONTINUE:
        return ck_string("Continue");
        break;
    case STATUS_OK:
        return ck_string("OK");
        break;
    case STATUS_CREATED:
        return ck_string("Created");
        break;
    case STATUS_LOW_ON_STORAGE_SPACE:
        return ck_string("Low on Storage Space");
        break;
    case STATUS_MULTIPLE_CHOICES:
        return ck_string("Multiple Choices");
        break;
    case STATUS_MOVED_PERMANENTLY:
        return ck_string("Moved Permanently");
        break;
    case STATUS_MOVED_TEMPORARILY:
        return ck_string("Moved Temporarily");
        break;
    case STATUS_SEE_OTHER:
        return ck_string("See Other");
        break;
    case STATUS_NOT_MODIFIED:
        return ck_string("Not Modified");
        break;
    case STATUS_USE_PROXY:
        return ck_string("Use Proxy");
        break;
    case STATUS_BAD_REQUEST:
        return ck_string("Bad Request");
        break;
    case STATUS_UNAUTHORIZED:
        return ck_string("Unauthorized");
        break;
    case STATUS_PAYMENT_REQUIRED:
        return ck_string("Payment Required");
        break;
    case STATUS_FORBIDDEN:
        return ck_string("Forbidden");
        break;
    case STATUS_NOT_FOUND:
        return ck_string("Not Found");
        break;
    case STATUS_METHOD_NOT_ALLOWED:
        return ck_string("Method Not Allowed");
        break;
    case STATUS_NOT_ACCEPTABLE:
        return ck_string("Not Acceptable");
        break;
    case STATUS_PROXY_AUTH_REQUIRED:
        return ck_string("Proxy Authentication Required");
        break;
    case STATUS_REQUEST_TIME_OUT:
        return ck_string("Request Time-out");
        break;
    case STATUS_GONE:
        return ck_string("Gone");
        break;
    case STATUS_LENGTH_REQUIRED:
        return ck_string("Length Required");
        break;
    case STATUS_PRECONDITION_FAILED:
        return ck_string("Precondition Failed");
        break;
    case STATUS_REQUEST_ENTITY_TOO_LARGE:
        return ck_string("Request Entity Too Large");
        break;
    case STATUS_REQUEST_URI_TOO_LARGE:
        return ck_string("Request-URI Too Large");
        break;
    case STATUS_UNSUPPORTED_MEDIA_TYPE:
        return ck_string("Unsupported Media Type");
        break;
    case STATUS_PARAMETER_NOT_UNDERSTOOD:
        return ck_string("Parameter Not Understood");
        break;
    case STATUS_CONFERENCE_NOT_FOUND:
        return ck_string("Conference Not Found");
        break;
    case STATUS_NOT_ENOUGH_BANDWIDTH:
        return ck_string("Not Enough Bandwidth");
        break;
    case STATUS_SESSION_NOT_FOUND:
        return ck_string("Session Not Found");
        break;
    case STATUS_METHOD_NOT_VALID_IN_THIS_STATE:
        return ck_string("Method Not Valid in This State");
        break;
    case STATUS_HEADER_FIELD_NOT_VALID_FOR_RESOURCE:
        return ck_string("Header Field Not Valid for Resource");
        break;
    case STATUS_INVALID_RANGE:
        return ck_string("Invalid Range");
        break;
    case STATUS_PARAMETER_IS_READONLY:
        return ck_string("Parameter Is Read-Only");
        break;
    case STATUS_AGGREGATE_OPERATION_NOT_ALLOWED:
        return ck_string("Aggregate operation not allowed");
        break;
    case STATUS_ONLY_AGGREGATE_OPERATION_ALLOWED:
        return ck_string("Only aggregate operation allowed");
        break;
    case STATUS_UNSUPPORTED_TRANSPORT:
        return ck_string("Unsupported transport");
        break;
    case STATUS_DESTINATION_UNREACHABLE:
        return ck_string("Destination unreachable");
        break;
    case STATUS_INTERNAL_SERVER_ERROR:
        return ck_string("Internal Server Error");
        break;
    case STATUS_NOT_IMPLEMENTED:
        return ck_string("Not Implemented");
        break;
    case STATUS_BAD_GATEWAY:
        return ck_string("Bad Gateway");
        break;
    case STATUS_SERVICE_UNAVAILABLE:
        return ck_string("Service Unavailable");
        break;
    case STATUS_GATEWAY_TIMEOUT:
        return ck_string("Gateway Time-out");
        break;
    case STATUS_RTSP_VERSION_NOT_SUPPORTED:
        return ck_string("RTSP Version not supported");
        break;
    case STATUS_OPERATION_NOT_SUPPORTED:
        return ck_string("Option not supported");
        break;
    default:
        break;
    };

    return ck_string( "Unknown" );
}

status river::convert_status_code_from_int( int value )
{
    if ( value == STATUS_CONTINUE )
        return (status)value;
    if ( value == STATUS_OK )
        return (status)value;
    if ( value == STATUS_CREATED )
        return (status)value;
    if ( value == STATUS_LOW_ON_STORAGE_SPACE )
        return (status)value;
    if ( value >= STATUS_MULTIPLE_CHOICES && value <= STATUS_USE_PROXY )
        return (status)value;
    if ( value >= STATUS_BAD_REQUEST && value <= STATUS_REQUEST_TIME_OUT )
        return (status)value;
    if ( value == STATUS_GONE )
        return (status)value;
    if ( value >= STATUS_LENGTH_REQUIRED && value <= STATUS_UNSUPPORTED_MEDIA_TYPE )
        return (status)value;
    if ( value >= STATUS_PARAMETER_NOT_UNDERSTOOD && value <= STATUS_DESTINATION_UNREACHABLE )
        return (status)value;
    if ( value >= STATUS_INTERNAL_SERVER_ERROR && value <= STATUS_RTSP_VERSION_NOT_SUPPORTED )
        return (status)value;
    if ( value == STATUS_OPERATION_NOT_SUPPORTED )
        return (status)value;
    if ( value >= STATUS_BAD_REQUEST && value < STATUS_INTERNAL_SERVER_ERROR )
        return STATUS_BAD_REQUEST;
    if ( value >= STATUS_INTERNAL_SERVER_ERROR && value < S_UNDEFINED )
        return STATUS_INTERNAL_SERVER_ERROR;
    if ( value >= STATUS_OK && value < STATUS_MULTIPLE_CHOICES )
        return STATUS_OK;

    CK_STHROW(river_exception, ("Value(%d) is not a valid status code!",value));
}
