
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

void client_connection::write_request( shared_ptr<client_request> request )
{
    _sequence++;

    request->set_server_ip( _serverIP );
    request->set_server_port( _serverPort );

    request->set_header( "CSeq", ck_string::from_int( _sequence ) );

    if( _sessionID.length() > 0 )
        request->set_header( "Session", _sessionID );

    request->write_request( _sok );
}

shared_ptr<client_response> client_connection::read_response()
{
    shared_ptr<client_response> response = make_shared<client_response>();

    response->read_response( _sok );

    ck_string value;
    if( response->get_header( "Session", value ) )
    {
        if( _sessionID.empty() )
            _sessionID = value.substr( 0, value.find(';') ); // If no ';' found, entire string is taken.
    }

    return response;
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

        length = ck_ntohs( sizeShort );

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
