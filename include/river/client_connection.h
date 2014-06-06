
#ifndef river_client_connection_h
#define river_client_connection_h

#include "cppkit/ck_string.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_memory.h"

#include "river/client_request.h"
#include "river/client_response.h"

namespace river
{

class client_connection
{
public:
    CK_API client_connection( const cppkit::ck_string& serverIP, int port );
    CK_API virtual ~client_connection() throw();

    CK_API bool connect( int timeoutMillis = 3000 );
    CK_API void disconnect();

    CK_API void set_recv_timeout( int timeoutMillis ) { _recvTimeoutMillis = timeoutMillis; }

    CK_API void send_request( client_request& request );

    CK_API void receive_request( client_response& response );

    CK_API cppkit::ck_string get_session_id() const;

    CK_API void set_session_id( const cppkit::ck_string& sessionID );

    CK_API bool receive_interleaved_packet( uint8_t& channel, std::shared_ptr<cppkit::ck_memory> buffer );

private:
    client_connection( const client_connection& rhs );
    client_connection& operator = ( const client_connection& rhs );

    cppkit::ck_string _serverIP;
    int _serverPort;
    std::shared_ptr<cppkit::ck_socket> _sok;
    int _sequence;
    cppkit::ck_string _sessionID;
    int _recvTimeoutMillis;
};

}

#endif
