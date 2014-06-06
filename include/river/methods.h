
#ifndef river_methods_h
#define river_methods_h

#include "cppkit/ck_string.h"

namespace river
{

enum method
{
    M_OPTIONS,
    M_DESCRIBE,
    M_SETUP,
    M_TEARDOWN,
    M_PLAY,
    M_PAUSE,
    M_GET_PARAM,
    M_SET_PARAM,
    M_REDIRECT,

    M_NUM_METHODS
};

cppkit::ck_string get_method_name( method m );

method get_method( const cppkit::ck_string& methodName );

}

#endif
