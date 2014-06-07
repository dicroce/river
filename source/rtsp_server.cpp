
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

#include "river/rtsp_server.h"
#include "river/river_exception.h"

#include <exception>
#include <algorithm>

using namespace river;
using namespace cppkit;
using namespace std;

uint32_t rtsp_server::_next_session_id = 42;
recursive_mutex rtsp_server::_session_id_lock;

rtsp_server::rtsp_server( const ck_string& serverIP, int port ) :
    _thread(),
    _port( port ),
    _running( false ),
    _sessionPrototypes(),
    _sessions(),
    _connections(),
    _serverSocket( make_shared<ck_socket>() ),
    _serverIP( serverIP ),
    _sessionsLock(),
    _keepAliveTimer(),
    _connectionsLock(),
    _prototypesLock()
{
    _keepAliveTimer = make_shared<stream_keeper>( *this );

    _serverSocket->bind( _port, _serverIP );
    _serverSocket->listen();
}

rtsp_server::rtsp_server( ck_socket::ck_socket_type type, const ck_string& serverIP, int port ) :
    _thread(),
    _port( port ),
    _running( false ),
    _sessionPrototypes(),
    _sessions(),
    _connections(),
    _serverSocket( make_shared<ck_socket>( type ) ),
    _serverIP( serverIP ),
    _sessionsLock(),
    _keepAliveTimer(),
    _connectionsLock(),
    _prototypesLock()
{
    _keepAliveTimer = make_shared<stream_keeper>( *this );

    _serverSocket->bind( _port, _serverIP );
    _serverSocket->listen();
}

rtsp_server::~rtsp_server() throw()
{
    if( _running )
        shutdown();
}

void rtsp_server::attach_session_prototype( shared_ptr<session_base> session )
{
    unique_lock<recursive_mutex> guard( _prototypesLock );

    _sessionPrototypes.push_back( session );
}

void rtsp_server::startup()
{
    {
        unique_lock<recursive_mutex> guard( _prototypesLock );
        if( _sessionPrototypes.empty() )
            CK_STHROW( river_exception, ( "Please call AttachSessionPrototype() at least once before starting this server." ));
    }

   _thread = std::thread( &rtsp_server::_entry_point, this );

    _keepAliveTimer->start();
}

void rtsp_server::stop_session( ck_string id )
{
    {
        unique_lock<recursive_mutex> guard( _sessionsLock );

        auto found = _sessions.find( id );
        if( found != _sessions.end() )
            _sessions.erase( found );
    }

    {
        unique_lock<recursive_mutex> guard( _connectionsLock );

        _connections.erase( remove_if( _connections.begin(),
                                       _connections.end(),
                                       [&](shared_ptr<server_connection> i)->bool{ return (i->get_session_id() == id); } ),
                            _connections.end() );
    }
}

void rtsp_server::check_inactive_sessions()
{
    unique_lock<recursive_mutex> guard( _sessionsLock );

    for( auto i : _sessions )
    {
        if( i.second->get_timeout_interval_seconds() == 0 )
            continue;

        chrono::steady_clock::time_point then = i.second->get_last_keep_alive_time();
        chrono::steady_clock::time_point now = chrono::steady_clock::now();

        int deltaSeconds = chrono::duration_cast<std::chrono::seconds>( now - then ).count();

        if( deltaSeconds < i.second->get_timeout_interval_seconds() )
            continue;

        stop_session( i.second->get_session_id() );
    }
}

void rtsp_server::shutdown()
{
    _running = false;

    if( _serverSocket->valid() )
        _serverSocket->close();

    _thread.join();
    _keepAliveTimer->stop();

    {
        // This joins ALL our ServerConnection threads.
        unique_lock<recursive_mutex> guard( _connectionsLock );
        _connections.clear();
    }
}

void* rtsp_server::_entry_point()
{
    _running = true;

    while( _running )
    {
        try
        {
            int32_t timeoutMillis = 5000;
            bool timedOut = _serverSocket->wait_recv( timeoutMillis );

            if( !_running )
                continue;

            if( !timedOut )
            {
                shared_ptr<server_connection> serverConnection = make_shared<server_connection>( this, _serverSocket->accept() );

                {
                    unique_lock<recursive_mutex> guard( _connectionsLock );

                    _connections.push_back( serverConnection );

                    serverConnection->startup();
                }
            }

            {
                unique_lock<recursive_mutex> guard( _connectionsLock );
                _connections.erase( remove_if( _connections.begin(),
                                               _connections.end(),
                                               [](shared_ptr<server_connection> i)->bool{ return !i->running(); } ),
                                    _connections.end() );
            }
        }
        catch( std::exception& ex )
        {
            CK_LOG_ERROR( "[rtsp_server] EntryPoint Exception: %s", ex.what() );
        }
    }

    return NULL;
}

shared_ptr<server_response> rtsp_server::route_request( shared_ptr<server_request> request )
{
    method m = request->get_method();

    if( m == M_OPTIONS )
        return _handle_options( request );

    if( m == M_DESCRIBE )
        return _handle_describe( request );

    ck_string sessionHeader;
    bool hasSessionHeader = request->get_header( "Session", sessionHeader );

    if( !hasSessionHeader )
        CK_STHROW( river_exception, ( "Session header not found." ));

    if( m == M_SETUP )
        _handle_setup( request );

    shared_ptr<session_base> session;

    {
        unique_lock<recursive_mutex> guard( _sessionsLock );
        auto found = _sessions.find( sessionHeader );
        if( found != _sessions.end() )
            session = found->second;
    }

    if( session.get() == NULL )
        CK_STHROW( river_exception, ( "No session was found." ));

    if ( m == M_GET_PARAM )
        session->update_keep_alive_time();

    shared_ptr<server_response> response = session->handle_request( request );

    if( m == M_TEARDOWN )
    {
        unique_lock<recursive_mutex> guard( _sessionsLock );
        _sessions.erase( sessionHeader );
    }

    return response;
}

ck_string rtsp_server::get_next_session_id()
{
    unique_lock<recursive_mutex> guard( _session_id_lock );

    ck_string ID = ck_string::from_uint32( _next_session_id );

    _next_session_id++;

    return ID;
}

shared_ptr<server_response> rtsp_server::_handle_options( shared_ptr<server_request> request )
{
    shared_ptr<session_base> session = _locate_session_prototype(request->get_resource_path().strip());

    shared_ptr<server_response> response = make_shared<server_response>();

    response->set_header( "Public", session->get_supported_options() );

    return response;
}

shared_ptr<server_response> rtsp_server::_handle_describe( shared_ptr<server_request> request )
{
    ck_string presentation = request->get_resource_path().strip();

    shared_ptr<session_base> prototype = _locate_session_prototype( presentation );

    shared_ptr<session_base> session = prototype->clone();
    shared_ptr<server_response> response = session->handle_request( request );

    response->set_header("Content-type","application/sdp");
    response->set_header("Content-Base", ck_string::format( "rtsp://%s:%d/%s/",
                                                            _serverIP.c_str(),
                                                            _port,
                                                            presentation.c_str()) );

    return response;
}

void rtsp_server::_handle_setup( shared_ptr<server_request> request )
{
    ck_string sessionHeader;
    if( !request->get_header( "Session", sessionHeader ) )
        CK_STHROW( river_exception, ( "Session header not found." ));

    ck_string presentation = request->get_resource_path().strip();

    shared_ptr<session_base> prototype = _locate_session_prototype( presentation );

    if( prototype.get() == NULL )
        CK_STHROW( river_exception, ( "Suitable connection prototype for this presentation not found." ));

    shared_ptr<session_base> session = prototype->clone();
    session->update_keep_alive_time();

    session->set_session_id( sessionHeader );

    {
        unique_lock<recursive_mutex> guard( _sessionsLock );
        _sessions.insert( make_pair( sessionHeader, session ) );
    }
}

shared_ptr<session_base> rtsp_server::_locate_session_prototype( const ck_string& resourcePath )
{
    unique_lock<recursive_mutex> guard( _prototypesLock );
    for( auto i : _sessionPrototypes )
    {
        if( i->handles_this_presentation( resourcePath ) )
            return i;
    }

    CK_STHROW( river_exception, ( "Suitable connection prototype for this presentation not found." ) );
}
