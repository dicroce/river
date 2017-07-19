
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

#include "river/client_request.h"
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;
using namespace std;

client_request::client_request( method m ) :
    _method( m ),
    _userAgent( "river version 1.0.0" ),
    _additionalHeaders(),
    _serverIP(),
    _serverPort(),
    _uri()
{
}

client_request::client_request( const client_request& rhs ) :
    _method( rhs._method ),
    _userAgent( rhs._userAgent ),
    _additionalHeaders( rhs._additionalHeaders ),
    _serverIP( rhs._serverIP ),
    _serverPort( rhs._serverPort ),
    _uri( rhs._uri )
{
}

client_request::client_request( client_request&& rhs ) throw() :
    _method( std::move( rhs._method ) ),
    _userAgent( std::move( rhs._userAgent ) ),
    _additionalHeaders( std::move( rhs._additionalHeaders ) ),
    _serverIP( std::move( rhs._serverIP ) ),
    _serverPort( std::move( rhs._serverPort ) ),
    _uri( std::move( rhs._uri ) )
{
}

client_request::~client_request() throw()
{
}

client_request& client_request::operator = ( const client_request& rhs )
{
    _method = rhs._method;
    _userAgent = rhs._userAgent;
    _additionalHeaders = rhs._additionalHeaders;
    _serverIP = rhs._serverIP;
    _serverPort = rhs._serverPort;
    _uri = rhs._uri;

    return *this;
}

client_request& client_request::operator = ( client_request&& rhs ) throw()
{
    _method = std::move( rhs._method );
    _userAgent = std::move( rhs._userAgent );
    _additionalHeaders = std::move( rhs._additionalHeaders );
    _serverIP = std::move( rhs._serverIP );
    _serverPort = std::move( rhs._serverPort );
    _uri = std::move( rhs._uri );

    return *this;
}

void client_request::set_method( method m )
{
    _method = m;
}

method client_request::get_method() const
{
    return _method;
}

void client_request::set_user_agent( const ck_string& userAgent )
{
    _userAgent = userAgent;
}

ck_string client_request::get_user_agent() const
{
    return _userAgent;
}

void client_request::set_header( const ck_string& name, const ck_string& value )
{
    _additionalHeaders.insert( make_pair( name, value ) );
}

bool client_request::get_header( const ck_string& name, ck_string& value ) const
{
    auto found = _additionalHeaders.find( name );

    if( found != _additionalHeaders.end() )
    {
        value = (*found).second;
        return true;
    }

    return false;
}

void client_request::set_server_ip( const ck_string& serverIP )
{
    _serverIP = serverIP;
}

ck_string client_request::get_server_ip() const
{
    return _serverIP;
}

void client_request::set_server_port( int port )
{
    _serverPort = port;
}

int client_request::get_server_port() const
{
    return _serverPort;
}

void client_request::set_uri( const ck_string& uri )
{
    if( uri.length() == 0 )
        CK_STHROW( rtsp_400_exception, ( "Invalid URI set on client_request." ) );

    if( uri.starts_with( "/" ) )
        _uri = uri.substr( 1 );
    else _uri = uri;
}

ck_string client_request::get_uri() const
{
    return ck_string::format( "/%s", _uri.c_str() );
}

void client_request::write_request( ck_stream_io& sok )
{
    ck_string rtspURL = ck_string::format( "rtsp://%s:%d/%s",
                                           _serverIP.c_str(),
                                           _serverPort,
                                           _uri.c_str() );

    if( _method < M_OPTIONS || _method > M_REDIRECT )
        CK_STHROW( rtsp_400_exception, ( "method not recognized." ));

    ck_string request = get_method_name( _method );

    ck_string initialLine = ck_string::format( "%s %s RTSP/1.0\r\n", request.c_str(), rtspURL.c_str() );

    ck_string message = initialLine;

    message += ck_string::format( "User-Agent: %s\r\n", _userAgent.c_str() );

    for( auto i : _additionalHeaders )
        message += ck_string::format( "%s: %s\r\n", i.first.c_str(), i.second.c_str() );

    message += ck_string::format( "\r\n" );

    sok.send( message.c_str(), message.length() );
}

