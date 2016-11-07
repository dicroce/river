
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

#include "river/server_connection.h"
#include "river/rtsp_server.h"
#include "river/river_exception.h"

#include <exception>

using namespace river;
using namespace cppkit;
using namespace std;

server_connection::server_connection( rtsp_server* server, ck_socket clientSocket ) :
    _thread(),
    _clientSocket( std::move(clientSocket) ),
    _server( server ),
    _sessionID(),
    _myrunning( false ),
    _isStarted( false )
{
}

server_connection::~server_connection() throw()
{
    if ( _isStarted )
        stop();
}

void server_connection::start()
{
    _myrunning = true;
    _thread = std::thread( &server_connection::_entry_point, this );
    _isStarted = true;
}

void server_connection::stop()
{
    _myrunning = false;
    _thread.join();
}

void server_connection::_entry_point()
{
    _myrunning = true;

    while( _myrunning )
    {
        try
        {
            if( !_clientSocket.valid() )
            {
                CK_LOG_NOTICE("[server_connection|%s] client socket not valid shutting down",_sessionID.c_str());
                _myrunning = false;
                continue;
            }

            uint64_t waitMillis = 2000;
            if( _clientSocket.recv_wont_block(waitMillis) )
            {
                // we need buffered_recv() equivalent here. If the recv_wont_block() pops out because the other side
                // closed their end, we need a way to determine if their is actual data in our buffer without having
                // to throw due to a timed out recv().

                shared_ptr<server_request> request = make_shared<server_request>();
                request->read_request( _clientSocket );

                if( !_clientSocket.valid() )
                {
                    CK_LOG_NOTICE("[server_connection|%s] client socket not valid shutting down",_sessionID.c_str());
                    _myrunning = false;
                    continue;
                }

                ck_string peerIP = _clientSocket.get_peer_ip();
                request->set_peer_ip( peerIP );
                request->set_local_ip( _clientSocket.get_local_ip() );

                ck_string sessionHeader;
                bool hasSessionHeader = request->get_header( "Session", sessionHeader );

                if( hasSessionHeader && (sessionHeader != _sessionID) )
                    throw_rtsp_exception( 400, "Invalid session header." );

                method m = request->get_method();

                if( m == M_SETUP )
                {
                    if( _sessionID.length() )
                        throw_rtsp_exception( 400, "Got SETUP, but we already have a session ID." );

                    _sessionID = rtsp_server::get_next_session_id();

                    request->set_header( "Session", _sessionID );
                }
                else if( m == M_TEARDOWN )
                {
                    _myrunning = false;
                }

                ck_string sequenceHeader;
                bool hasSequenceHeader = request->get_header( "CSeq", sequenceHeader );

                if( !hasSequenceHeader )
                    throw_rtsp_exception( 400, "[server_connection|%s]Got request without sequence header.", _sessionID.c_str() );

                shared_ptr<server_response> response = _server->route_request( request );

                if( ((response->get_status() < STATUS_OK) || (response->get_status() > STATUS_MULTIPLE_CHOICES)) )
                    _sessionID.clear();

                response->set_header( "CSeq", sequenceHeader );

                if( _sessionID.length() && ((response->get_status() >= STATUS_OK) && (response->get_status() < STATUS_MULTIPLE_CHOICES)) )
                    response->set_header( "Session", _sessionID );

                response->write_response( _clientSocket );
                if( !_clientSocket.valid() )
                {
                    CK_LOG_NOTICE("[server_connection|%s] client socket not valid shutting down",_sessionID.c_str());
                    _myrunning = false;
                    continue;
                }
            }
        }
        catch( exception& ex )
        {
            _myrunning = false;
            CK_LOG_ERROR( "[server_connection|%s] thread caught exception: %s",_sessionID.c_str(),ex.what() );
        }
    }
}

bool server_connection::running() const
{
    return _myrunning;
}

void server_connection::write_interleaved_packet( uint8_t channel, shared_ptr<cppkit::ck_memory> buffer )
{
    uint8_t token = 36;
    _clientSocket.send( &token, 1 );

    _clientSocket.send( &channel, 1 );

    int16_t length = (int16_t)buffer->size_data();
    int16_t lengthWord = ck_htons( length );

    _clientSocket.send( &lengthWord, 2 );

    _clientSocket.send( buffer->map().get_ptr(), buffer->size_data() );
}
