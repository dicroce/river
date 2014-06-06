
#ifndef __river_server_connection_h
#define __river_server_connection_h

#include "cppkit/ck_socket.h"
#include "cppkit/ck_memory.h"
#include <memory>
#include <thread>

namespace river
{

class river_server;

class server_connection
{
public:
    CK_API server_connection( river_server* server,
                              std::shared_ptr<cppkit::ck_socket> clientSocket = new cppkit::ck_socket );

    CK_API virtual ~server_connection() throw();

    CK_API void startup();
    CK_API void shutdown();

    CK_API inline std::shared_ptr<cppkit::ck_socket> get_socket() { return _clientSocket; }

    CK_API bool running() const;

    CK_API void write_interleaved_packet( uint8_t channel, std::shared_ptr<cppkit::ck_memory> buffer );

    CK_API cppkit::ck_string get_session_id() const { return _sessionID; }

private:
    server_connection( const server_connection& obj );
    server_connection& operator = ( const server_connection& obj );

    void _entry_point();

    std::thread _thread;
    std::shared_ptr<cppkit::ck_socket> _clientSocket;
    river_server* _server;
    cppkit::ck_string _sessionID;
    //Called "myrunning" even though it sounds douche, because it indicates the base class has a "running" member
    bool _myrunning;
    bool _isStarted;
};

}

#endif
