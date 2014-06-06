
#ifndef _river_river_server_h
#define _river_river_server_h

#include <map>
#include <memory>
#include <mutex>

#include "cppkit/ck_socket.h"

#include "river/session_base.h"
#include "river/server_request.h"
#include "river/server_response.h"
#include "river/server_connection.h"
#include "river/stream_keeper.h"

#include <list>

namespace river
{

class river_server
{
public:
    CK_API river_server( const cppkit::ck_string& serverIP, int port = 554 );
    CK_API river_server(  cppkit::ck_socket::ck_socket_type type, const cppkit::ck_string& serverIP, int port = 554 );
    CK_API virtual ~river_server() throw();

    CK_API void attach_session_prototype( std::shared_ptr<session_base> session );

    CK_API void startup();
    CK_API void shutdown();

    CK_API std::shared_ptr<server_response> route_request( std::shared_ptr<server_request> request );

    CK_API bool is_session_current( const cppkit::ck_string& sessionID );

    CK_API std::map<std::string, std::shared_ptr<session_base> > get_sessions();

    CK_API static cppkit::ck_string get_next_session_id();

    CK_API void stop_session( cppkit::ck_string id );
    CK_API void stop_all_sessions();

private:
    river_server( const river_server& obj );
    river_server& operator = ( const river_server& obj );

    void* _entry_point();

    std::shared_ptr<server_response> _handle_options( std::shared_ptr<server_request> request );
    std::shared_ptr<server_response> _handle_describe( std::shared_ptr<server_request> request );
    void _handle_setup( std::shared_ptr<server_request> request );

    std::shared_ptr<session_base> _locate_session_prototype( const cppkit::ck_string& resourcePath );

    std::thread _thread;
    int _port;
    bool _running;
    std::list<std::shared_ptr<session_base> > _sessionPrototypes;
    std::map<std::string, std::shared_ptr<session_base> > _sessions;
    std::list<std::shared_ptr<server_connection> > _connections;
    std::shared_ptr<cppkit::ck_socket> _serverSocket;
    cppkit::ck_string _serverIP;
    std::recursive_mutex _sessionsLock;
    std::shared_ptr<stream_keeper> _keepAliveTimer;
    std::recursive_mutex _connectionsLock;
    std::recursive_mutex _prototypesLock;

    static uint32_t _next_session_id;
    static std::recursive_mutex _session_id_lock;
};

}

#endif
