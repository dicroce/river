
#include "river/status.h"
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;

const static int S_UNDEFINED = 600;

ck_string river::get_status_message( status s )
{
    switch( s )
    {
    case S_CONTINUE:
        return ck_string("Continue");
        break;
    case S_OK:
        return ck_string("OK");
        break;
    case S_CREATED:
        return ck_string("Created");
        break;
    case S_LOW_ON_STORAGE_SPACE:
        return ck_string("Low on Storage Space");
        break;
    case S_MULTIPLE_CHOICES:
        return ck_string("Multiple Choices");
        break;
    case S_MOVED_PERMANENTLY:
        return ck_string("Moved Permanently");
        break;
    case S_MOVED_TEMPORARILY:
        return ck_string("Moved Temporarily");
        break;
    case S_SEE_OTHER:
        return ck_string("See Other");
        break;
    case S_NOT_MODIFIED:
        return ck_string("Not Modified");
        break;
    case S_USE_PROXY:
        return ck_string("Use Proxy");
        break;
    case S_BAD_REQUEST:
        return ck_string("Bad Request");
        break;
    case S_UNAUTHORIZED:
        return ck_string("Unauthorized");
        break;
    case S_PAYMENT_REQUIRED:
        return ck_string("Payment Required");
        break;
    case S_FORBIDDEN:
        return ck_string("Forbidden");
        break;
    case S_NOT_FOUND:
        return ck_string("Not Found");
        break;
    case S_METHOD_NOT_ALLOWED:
        return ck_string("Method Not Allowed");
        break;
    case S_NOT_ACCEPTABLE:
        return ck_string("Not Acceptable");
        break;
    case S_PROXY_AUTH_REQUIRED:
        return ck_string("Proxy Authentication Required");
        break;
    case S_REQUEST_TIME_OUT:
        return ck_string("Request Time-out");
        break;
    case S_GONE:
        return ck_string("Gone");
        break;
    case S_LENGTH_REQUIRED:
        return ck_string("Length Required");
        break;
    case S_PRECONDITION_FAILED:
        return ck_string("Precondition Failed");
        break;
    case S_REQUEST_ENTITY_TOO_LARGE:
        return ck_string("Request Entity Too Large");
        break;
    case S_REQUEST_URI_TOO_LARGE:
        return ck_string("Request-URI Too Large");
        break;
    case S_UNSUPPORTED_MEDIA_TYPE:
        return ck_string("Unsupported Media Type");
        break;
    case S_PARAMETER_NOT_UNDERSTOOD:
        return ck_string("Parameter Not Understood");
        break;
    case S_CONFERENCE_NOT_FOUND:
        return ck_string("Conference Not Found");
        break;
    case S_NOT_ENOUGH_BANDWIDTH:
        return ck_string("Not Enough Bandwidth");
        break;
    case S_SESSION_NOT_FOUND:
        return ck_string("Session Not Found");
        break;
    case S_METHOD_NOT_VALID_IN_THIS_STATE:
        return ck_string("Method Not Valid in This State");
        break;
    case S_HEADER_FIELD_NOT_VALID_FOR_RESOURCE:
        return ck_string("Header Field Not Valid for Resource");
        break;
    case S_INVALID_RANGE:
        return ck_string("Invalid Range");
        break;
    case S_PARAMETER_IS_READONLY:
        return ck_string("Parameter Is Read-Only");
        break;
    case S_AGGREGATE_OPERATION_NOT_ALLOWED:
        return ck_string("Aggregate operation not allowed");
        break;
    case S_ONLY_AGGREGATE_OPERATION_ALLOWED:
        return ck_string("Only aggregate operation allowed");
        break;
    case S_UNSUPPORTED_TRANSPORT:
        return ck_string("Unsupported transport");
        break;
    case S_DESTINATION_UNREACHABLE:
        return ck_string("Destination unreachable");
        break;
    case S_INTERNAL_SERVER_ERROR:
        return ck_string("Internal Server Error");
        break;
    case S_NOT_IMPLEMENTED:
        return ck_string("Not Implemented");
        break;
    case S_BAD_GATEWAY:
        return ck_string("Bad Gateway");
        break;
    case S_SERVICE_UNAVAILABLE:
        return ck_string("Service Unavailable");
        break;
    case S_GATEWAY_TIMEOUT:
        return ck_string("Gateway Time-out");
        break;
    case S_RTSP_VERSION_NOT_SUPPORTED:
        return ck_string("RTSP Version not supported");
        break;
    case S_OPERATION_NOT_SUPPORTED:
        return ck_string("Option not supported");
        break;
    default:
        break;
    };

    return ck_string( "Unknown" );
}

status river::convert_status_code_from_int( int value )
{
    if ( value == S_CONTINUE )
        return (status)value;
    if ( value == S_OK )
        return (status)value;
    if ( value == S_CREATED )
        return (status)value;
    if ( value == S_LOW_ON_STORAGE_SPACE )
        return (status)value;
    if ( value >= S_MULTIPLE_CHOICES && value <= S_USE_PROXY )
        return (status)value;
    if ( value >= S_BAD_REQUEST && value <= S_REQUEST_TIME_OUT )
        return (status)value;
    if ( value == S_GONE )
        return (status)value;
    if ( value >= S_LENGTH_REQUIRED && value <= S_UNSUPPORTED_MEDIA_TYPE )
        return (status)value;
    if ( value >= S_PARAMETER_NOT_UNDERSTOOD && value <= S_DESTINATION_UNREACHABLE )
        return (status)value;
    if ( value >= S_INTERNAL_SERVER_ERROR && value <= S_RTSP_VERSION_NOT_SUPPORTED )
        return (status)value;
    if ( value == S_OPERATION_NOT_SUPPORTED )
        return (status)value;
    if ( value >= S_BAD_REQUEST && value < S_INTERNAL_SERVER_ERROR )
        return S_BAD_REQUEST;
    if ( value >= S_INTERNAL_SERVER_ERROR && value < S_UNDEFINED )
        return S_INTERNAL_SERVER_ERROR;
    if ( value >= S_OK && value < S_MULTIPLE_CHOICES )
        return S_OK;

    CK_STHROW(river_exception, ("Value(%d) is not a valid status code!",value));
}
