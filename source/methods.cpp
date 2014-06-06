
#include "river/methods.h"
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;

static const ck_string method_names[] = { "OPTIONS",
                                          "DESCRIBE",
                                          "SETUP",
                                          "TEARDOWN",
                                          "PLAY",
                                          "PAUSE",
                                          "GET_PARAMETER",
                                          "SET_PARAMETER",
                                          "REDIRECT" };

ck_string river::get_method_name( method m )
{
    if( m >= M_NUM_METHODS )
        throw_rtsp_exception( 500, "Unknown method." );

    return method_names[m];
}

method river::get_method( const cppkit::ck_string& methodName )
{
    if( methodName == "OPTIONS" )
        return M_OPTIONS;
    else if( methodName == "DESCRIBE" )
        return M_DESCRIBE;
    else if( methodName == "SETUP" )
        return M_SETUP;
    else if( methodName == "TEARDOWN" )
        return M_TEARDOWN;
    else if( methodName == "PLAY" )
        return M_PLAY;
    else if( methodName == "PAUSE" )
        return M_PAUSE;
    else if( methodName == "GET_PARAMETER" )
        return M_GET_PARAM;
    else if( methodName == "SET_PARAMETER" )
        return M_SET_PARAM;
    else if( methodName == "REDIRECT" )
        return M_REDIRECT;

    throw_rtsp_exception( 400, "Unknown method." );
}
