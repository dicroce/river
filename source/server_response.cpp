
#include "river/server_response.h"
#include "river/river_exception.h"
#include <time.h>

using namespace river;
using namespace cppkit;
using namespace std;

server_response::server_response() :
    _status( S_OK ),
    _headers(),
    _body( new ck_memory )
{
}

server_response::server_response( const server_response& rhs ) :
    _status( rhs._status ),
    _headers( rhs._headers ),
    _body( rhs._body )
{
}

server_response::server_response( server_response&& rhs ) noexcept :
    _status( std::move( rhs._status ) ),
    _headers( std::move( rhs._headers ) ),
    _body( std::move( rhs._body ) )
{
}

server_response::~server_response() throw()
{
}


server_response& server_response::operator = ( const server_response& rhs )
{
    _status = rhs._status;
    _headers = rhs._headers;
    _body = rhs._body;

    return *this;
}

server_response& server_response::operator = ( server_response&& rhs ) noexcept
{
    _status = std::move( rhs._status );
    _headers = std::move( rhs._headers );
    _body = std::move( rhs._body );

    return *this;
}

void server_response::set_status( status s )
{
    _status = s;
}

status server_response::get_status() const
{
    return _status;
}

void server_response::set_header( const ck_string& key, const ck_string& value )
{
    _headers.insert( make_pair( key, value ) );
}

bool server_response::get_header( const ck_string& key, ck_string& value )
{
    auto found = _headers.find( key );
    if( found != _headers.end() )
    {
        value = found->second;
        return true;
    }

    return false;
}

void server_response::set_body( std::shared_ptr<cppkit::ck_memory> body )
{
    _body = body;
}

std::shared_ptr<cppkit::ck_memory> server_response::get_body() const
{
    return _body;
}

void server_response::set_body( const cppkit::ck_string& body )
{
    _body->clear();

    size_t bodyLength = body.length();

    memcpy( _body->extend_data( bodyLength ).get_ptr(), body.c_str(), body.length() );
}

ck_string server_response::get_body_as_string() const
{
    size_t bodySize = _body->size_data();

    return ck_string( (char*)_body->map().get_ptr(), bodySize );
}

void server_response::write_response( shared_ptr<ck_stream_io> sok )
{
    ck_string messageHeader = ck_string::format( "RTSP/1.0 %d %s\r\n", _status, get_status_message( _status ).c_str() );

    // Please Note: This is NOT just getting the time, it is strategically chomping a
    // very particular character!
    time_t now = time( 0 );
    char* timeString = ctime( &now );
    int timeLen = strlen( timeString );
    timeString[timeLen-1] = 0;

    ck_string dateTime = timeString;

    ck_string dateLine = ck_string::format( "Date: %s\r\n", dateTime.c_str() );

    for( auto i = _headers.begin(), end = _headers.end(); i != end; i++ )
        messageHeader += ck_string::format( "%s: %s\r\n", i->first.c_str(), i->second.c_str() );

    unsigned int bodySize = _body->size_data();

    if( bodySize > 0 )
        messageHeader += ck_string::format( "Content-Length: %d\r\n", bodySize );

    messageHeader += ck_string::format( "\r\n" );

    ssize_t bytesSent = sok->send( messageHeader.c_str(), messageHeader.length() );
    if( !sok->valid() || (bytesSent != (int32_t)messageHeader.length()) )
        CK_STHROW( rtsp_500_exception, ("I/O error.") );

    if( bodySize )
    {
        bytesSent = sok->send( _body->map().get_ptr(), bodySize );

        if( !sok->valid() || (bytesSent != (int32_t)bodySize) )
            CK_STHROW( rtsp_500_exception, ("I/O error.") );
    }
}

server_response server_response::create_response_to( method m )
{
    server_response ssr;

    if( m == M_OPTIONS )
    {
        ssr.set_header( "Public", "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, GET_PARAMETER, SET_PARAMETER, REDIRECT, RECORD" );
    }

    return ssr;
}
