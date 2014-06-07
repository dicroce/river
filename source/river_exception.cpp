
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
