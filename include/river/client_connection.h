
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

    CK_API void write_request( std::shared_ptr<client_request> request );

    CK_API std::shared_ptr<client_response> read_response();

    CK_API cppkit::ck_string get_session_id() const;

    CK_API void set_session_id( const cppkit::ck_string& sessionID );

    CK_API bool receive_interleaved_packet( uint8_t& channel, std::shared_ptr<cppkit::ck_memory> buffer );

private:
    client_connection( const client_connection& rhs ) = delete;
    client_connection& operator = ( const client_connection& rhs ) = delete;

    cppkit::ck_string _serverIP;
    int _serverPort;
    std::shared_ptr<cppkit::ck_socket> _sok;
    int _sequence;
    cppkit::ck_string _sessionID;
    int _recvTimeoutMillis;
};

}

#endif
