
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
