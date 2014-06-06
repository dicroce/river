
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;

river_exception::river_exception() :
    ck_exception()
{
}

river_exception::river_exception( const char* msg, ... ) :
    ck_exception()
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

river_exception::river_exception( const ck_string& msg ) :
    ck_exception( msg )
{
}

rtsp_exception::rtsp_exception( int statusCode ) :
    river_exception(),
    _statusCode( statusCode )
{
}

rtsp_exception::rtsp_exception( int statusCode, const char* msg, ... ) :
    river_exception(),
    _statusCode( statusCode )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

rtsp_exception::rtsp_exception( int statusCode, const ck_string& msg ) :
    river_exception( msg ),
    _statusCode( statusCode )
{
}

void river::throw_rtsp_exception( int statusCode, const char* msg, ... )
{
    va_list args;
    va_start( args, msg );
    const ck_string message = ck_string::format( msg, args );
    va_end( args );

    throw_rtsp_exception( statusCode, message );
}

void river::throw_rtsp_exception( int statusCode, const ck_string& msg )
{
    switch( statusCode )
    {
        case 400:
        {
            rtsp_400_exception e( msg );
            e.set_type_name("rtsp_400_exception");
            throw e;
        }
        case 401:
        {
            rtsp_401_exception e( msg );
            e.set_type_name("rtsp_401_exception");
            throw e;
        }
        case 403:
        {
            rtsp_403_exception e( msg );
            e.set_type_name("rtsp_403_exception");
            throw e;
        }
        case 404:
        {
            rtsp_404_exception e( msg );
            e.set_type_name("rtsp_404_exception");
            throw e;
        }
        case 415:
        {
            rtsp_415_exception e( msg );
            e.set_type_name("rtsp_415_exception");
            throw e;
        }
        case 453:
        {
            rtsp_453_exception e( msg );
            e.set_type_name("rtsp_453_exception");
            throw e;
        }
        case 500:
        {
            rtsp_500_exception e( msg );
            e.set_type_name("rtsp_500_exception");
            throw e;
        }
        default:
        {
            rtsp_exception e( statusCode, msg );
            e.set_type_name("rtsp_exception");
            throw e;
        }
    }
}
