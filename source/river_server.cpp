
#include "river/river_server.h"
#include "river/river_exception.h"

#include <exception>

using namespace river;
using namespace cppkit;
using namespace std;

uint32_t river_server::_next_session_id = 42;
recursive_mutex river_server::_session_id_lock;

river_server::river_server( const ck_string& serverIP, int port ) :
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

river_server::river_server( ck_socket::ck_socket_type type, const ck_string& serverIP, int port ) :
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

river_server::~river_server() throw()
{
    if( _running )
        shutdown();
}

void river_server::attach_session_prototype( shared_ptr<session_base> session )
{
    unique_lock<recursive_mutex> guard( _prototypesLock );

    _sessionPrototypes.push_back( session );
}

void river_server::startup()
{
    {
        unique_lock<recursive_mutex> guard( _prototypesLock );
        if( _sessionPrototypes.empty() )
            CK_STHROW( river_exception, ( "Please call AttachSessionPrototype() at least once before starting this server." ));
    }

   _thread = std::thread( &river_server::_entry_point, this );

    _keepAliveTimer->start();
}

void river_server::stop_session( ck_string id )
{
    {
        unique_lock<recursive_mutex> guard( _sessionsLock );
        auto found = _sessions.find( id );
        if( found != _sessions.end() )
            _sessions.erase( found );
    }

    {
        unique_lock<recursive_mutex> guard( _connectionsLock );
        for( auto iter = _connections.begin(), end = _connections.end(); iter != end; )
        {
            if( (*iter)->get_session_id() == id )
                iter = _connections.erase( iter );
            else ++iter;
        }
    }
}

void river_server::stop_all_sessions()
{
    {
        unique_lock<recursive_mutex> guard( _sessionsLock );
        _sessions.clear();
    }

    {
        unique_lock<recursive_mutex> guard( _connectionsLock );
        _connections.clear();
    }
}

void river_server::shutdown()
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

void* river_server::_entry_point()
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

                for( auto iter = _connections.begin(), end = _connections.end(); iter != end; )
                {
                    if( !(*iter)->running() )
                    {
                        iter = _connections.erase( iter );
                    }
                    else ++iter;
                }
            }
        }
        catch( std::exception& ex )
        {
            CK_LOG_ERROR( "[river_server] EntryPoint Exception: %s", ex.what() );
        }
    }

    return NULL;
}

shared_ptr<server_response> river_server::route_request( shared_ptr<server_request> request )
{
    CK_LOG_TRACE("[river_server] RouteRequest");
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
    {//SM5200-2631 exposed the need to reduce this locks scope
        unique_lock<recursive_mutex> guard( _sessionsLock );
        _sessions.erase( sessionHeader );
    }

    return response;
}

bool river_server::is_session_current( const ck_string& sessionID )
{
    unique_lock<recursive_mutex> guard( _sessionsLock );
    auto found = _sessions.find( sessionID );
    return found != _sessions.end();
}

ck_string river_server::get_next_session_id()
{
    unique_lock<recursive_mutex> guard( _session_id_lock );

    ck_string ID = ck_string::from_uint32( _next_session_id );

    _next_session_id++;

    return ID;
}

shared_ptr<server_response> river_server::_handle_options( shared_ptr<server_request> request )
{
    CK_LOG_TRACE("[river_server] Handling a options inquiry");

    shared_ptr<session_base> session = _locate_session_prototype(request->get_resource_path().strip());

    shared_ptr<server_response> response = make_shared<server_response>();

    response->set_header( "Public", session->get_supported_options() );

    return response;
}

shared_ptr<server_response> river_server::_handle_describe( shared_ptr<server_request> request )
{
    CK_LOG_TRACE("[river_server] Handling a describe request");
    ck_string presentation = request->get_resource_path().strip();

    shared_ptr<session_base> prototype = _locate_session_prototype( presentation );

    //JIRA: MPF-1516
    //Create a separate prototype type from session_base which
    //will serve as a factory for sessions as well as handling options and describe.
    //This distinction makes it easier for a user of Streamy to distinguish
    //stateless prototypes from stateful sessions.
    shared_ptr<session_base> session = prototype->clone();
    shared_ptr<server_response> response = session->handle_request( request );

    response->set_header("Content-type","application/sdp");
    response->set_header("Content-Base", ck_string::format( "rtsp://%s:%d/%s/",
                                                            _serverIP.c_str(),
                                                            _port,
                                                            presentation.c_str()) );

    return response;
}

map<string, shared_ptr<session_base> > river_server::get_sessions()
{
    unique_lock<recursive_mutex> guard( _sessionsLock );
    return _sessions;
}

void river_server::_handle_setup( shared_ptr<server_request> request )
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
    unique_lock<recursive_mutex> guard( _sessionsLock );
    _sessions.insert( make_pair( sessionHeader, session ) );
}

shared_ptr<session_base> river_server::_locate_session_prototype( const ck_string& resourcePath )
{
    unique_lock<recursive_mutex> guard( _prototypesLock );

    for( auto i = _sessionPrototypes.begin(), end = _sessionPrototypes.end(); i != end; i++ )
    {
        if( (*i)->handles_this_presentation( resourcePath ) )
            return *i;
    }

    CK_STHROW( river_exception, ( "Suitable connection prototype for this presentation not found." ) );
}
