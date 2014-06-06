
#include "river/client_connection.h"

using namespace river;
using namespace cppkit;
using namespace std;

client_connection::client_connection( const cppkit::ck_string& serverIP, int port ) :
    _serverIP( serverIP ),
    _serverPort( port ),
    _sok( new ck_socket ),
    _sequence( 0 ),
    _sessionID(),
    _recvTimeoutMillis( 5000 )
{
}

client_connection::~client_connection() throw()
{
}

bool client_connection::connect( int timeoutMillis )
{
    return _sok->query_connect( _serverIP, _serverPort, timeoutMillis );
}

void client_connection::disconnect()
{
    _sequence = 0;
    _sessionID = "";
    _sok->close();
}

void client_connection::send_request( client_request& request )
{
    _sequence++;

    request.set_server_ip( _serverIP );
    request.set_server_port( _serverPort );

    request.set_header( "CSeq", ck_string::from_int( _sequence ) );

    if( _sessionID.length() > 0 )
        request.set_header( "Session", _sessionID );

    request.write_request( _sok );
}

void client_connection::receive_request( client_response& response )
{
    CK_THROW(("Not implemented yet!"));
}

cppkit::ck_string client_connection::get_session_id() const
{
    return _sessionID;
}

void client_connection::set_session_id( const cppkit::ck_string& sessionID )
{
    _sessionID = sessionID;
}

bool client_connection::receive_interleaved_packet( uint8_t& channel, std::shared_ptr<cppkit::ck_memory> buffer )
{
    buffer->clear();

    uint8_t dollarSign = 0;
    uint16_t length = 0;
    uint16_t maxIterations = 10000;

    while( maxIterations > 0 )
    {
        maxIterations--;

        size_t bytesRead = _sok->recv( &dollarSign, 1, _recvTimeoutMillis );

        if( bytesRead == 0 )
            return false;

        if( dollarSign != 36 )
        {
            CK_LOG_WARNING( "client_connection [%s] invalid token: %d", _serverIP.c_str(), dollarSign );
            continue;
        }

        bytesRead = _sok->recv( &channel, 1, _recvTimeoutMillis );

        if( bytesRead == 0 )
            return false;

        if( channel > 2 )
        {
            CK_LOG_WARNING( "client_connections [%s] invalid channel: %d", _serverIP.c_str(), channel );
            continue;
        }

        uint16_t sizeShort;
        bytesRead = _sok->recv( &sizeShort, 2, _recvTimeoutMillis );

        if( bytesRead == 0 )
            return false;

        length = ntohs( sizeShort );

        //Interleaved appears to use the same RTP packets as UDP - given that,the max packet size of UDP would be 1500 bytes,
        //we really shouldn't see anything greater than 1600
        if( length > 1600 || length == 0 )
        {
            CK_LOG_WARNING( "client_connection [%s] invalid size: %d", _serverIP.c_str(), length );
            continue;
        }

        bytesRead = _sok->recv( buffer->extend_data( length ).get_ptr(), length, _recvTimeoutMillis );

        if( bytesRead == 0 )
            return false;

        return true;
    }

    CK_LOG_ERROR("client_connection [%s] Unable to sync with stream.", _serverIP.c_str() );

    return false;
}
