
#include "river/server_connection.h"
#include "river/river_server.h"
#include "river/river_exception.h"

#include <exception>

using namespace river;
using namespace cppkit;
using namespace std;

server_connection::server_connection( river_server* server, shared_ptr<ck_socket> clientSocket) :
    _thread(),
    _clientSocket( clientSocket ),
    _server( server ),
    _sessionID(),
    _myrunning( false ),
    _isStarted( false )
{
}

server_connection::~server_connection() throw()
{
    if ( _isStarted )
        shutdown();
}

void server_connection::startup()
{
   _myrunning = true;
   _thread = std::thread( &server_connection::_entry_point, this );
    _isStarted = true;
}

void server_connection::shutdown()
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
         if( !_clientSocket->valid() )
         {
            CK_LOG_NOTICE("[server_connection|%s] client socket not valid shutting down",_sessionID.c_str());
            _myrunning = false;
            continue;
         }

         int waitMillis = 2000;
         bool timedOut = _clientSocket->wait_recv( waitMillis );

         if( !timedOut )
         {
            shared_ptr<server_request> request = make_shared<server_request>();
            request->read_request( _clientSocket );

            if( !_clientSocket->valid() )
            {
               CK_LOG_NOTICE("[server_connection|%s] client socket not valid shutting down",_sessionID.c_str());
               _myrunning = false;
               continue;
            }

            ck_string peerIP = _clientSocket->get_peer_ip();
            request->set_peer_ip( peerIP );
            request->set_local_ip( _clientSocket->get_local_ip() );

            ck_string sessionHeader;
            bool hasSessionHeader = request->get_header( "Session", sessionHeader );

            if( hasSessionHeader && (sessionHeader != _sessionID) )
                throw_rtsp_exception( 400, "Invalid session header." );

            method m = request->get_method();

            if( m == M_SETUP )
            {
               if( _sessionID.length() )
                   throw_rtsp_exception( 400, "Got SETUP, but we already have a session ID." );

               _sessionID = river_server::get_next_session_id();

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

            if( ((response->get_status() < S_OK) || (response->get_status() > S_MULTIPLE_CHOICES)) )
                _sessionID.clear();

            response->set_header( "CSeq", sequenceHeader );

            if( _sessionID.length() && ((response->get_status() >= S_OK) && (response->get_status() < S_MULTIPLE_CHOICES)) )
               response->set_header( "Session", _sessionID );

            response->write_response( _clientSocket );
            if( !_clientSocket->valid() )
            {
                CK_LOG_NOTICE("[server_connection|%s] client socket not valid shutting down",_sessionID.c_str());
                _myrunning = false;
                continue;
            }
         }
         else CK_LOG_INFO("[server_connection|%s] Timed out",_sessionID.c_str());
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
   _clientSocket->send( &token, 1 );

   _clientSocket->send( &channel, 1 );

   int16_t length = (int16_t)buffer->size_data();
   int16_t lengthWord = htons( length );

   _clientSocket->send( &lengthWord, 2 );

   _clientSocket->send( buffer->map().get_ptr(), buffer->size_data() );
}
