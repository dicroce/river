
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
    _resourcePath()
{
}

client_request::client_request( const client_request& rhs ) :
    _method( rhs._method ),
    _userAgent( rhs._userAgent ),
    _additionalHeaders( rhs._additionalHeaders ),
    _serverIP( rhs._serverIP ),
    _serverPort( rhs._serverPort ),
    _resourcePath( rhs._resourcePath )
{
}

client_request::client_request( client_request&& rhs ) noexcept :
    _method( std::move( rhs._method ) ),
    _userAgent( std::move( rhs._userAgent ) ),
    _additionalHeaders( std::move( rhs._additionalHeaders ) ),
    _serverIP( std::move( rhs._serverIP ) ),
    _serverPort( std::move( rhs._serverPort ) ),
    _resourcePath( std::move( rhs._resourcePath ) )
{
}

client_request::~client_request() noexcept
{
}

client_request& client_request::operator = ( const client_request& rhs )
{
    _method = rhs._method;
    _userAgent = rhs._userAgent;
    _additionalHeaders = rhs._additionalHeaders;
    _serverIP = rhs._serverIP;
    _serverPort = rhs._serverPort;
    _resourcePath = rhs._resourcePath;

    return *this;
}

client_request& client_request::operator = ( client_request&& rhs ) noexcept
{
    _method = std::move( rhs._method );
    _userAgent = std::move( rhs._userAgent );
    _additionalHeaders = std::move( rhs._additionalHeaders );
    _serverIP = std::move( rhs._serverIP );
    _serverPort = std::move( rhs._serverPort );
    _resourcePath = std::move( rhs._resourcePath );

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

void client_request::set_resource_path( const ck_string& resourcePath )
{
    _resourcePath = resourcePath;
}

ck_string client_request::get_resource_path() const
{
    return _resourcePath;
}

void client_request::write_request( shared_ptr<ck_stream_io> sok )
{
    ck_string rtspURL = ck_string::format( "rtsp://%s:%d/%s",
                                           _serverIP.c_str(),
                                           _serverPort,
                                           _resourcePath.c_str() );

    if( _method < M_OPTIONS || _method > M_REDIRECT )
        CK_STHROW( rtsp_400_exception, ( "method not recognized." ));

    ck_string request = get_method_name( _method );

    ck_string initialLine = ck_string::format( "%s %s RTSP/1.0\r\n", request.c_str(), rtspURL.c_str() );

    ck_string message = initialLine;

    message += ck_string::format( "User-Agent: %s\r\n", _userAgent.c_str() );

    for( auto i = _additionalHeaders.begin(), end = _additionalHeaders.end(); i != end; i++ )
        message += ck_string::format( "%s: %s\r\n", (*i).first.c_str(), (*i).second.c_str() );

    message += ck_string::format( "\r\n" );

    ssize_t bytesSent = sok->send( message.c_str(), message.length() );

    if( !sok->valid() || (bytesSent != (int32_t)message.length()) )
        CK_STHROW( rtsp_500_exception, ("I/O error.") );
}
