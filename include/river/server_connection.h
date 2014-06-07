
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

#ifndef __river_server_connection_h
#define __river_server_connection_h

#include "cppkit/ck_socket.h"
#include "cppkit/ck_memory.h"
#include <memory>
#include <thread>

namespace river
{

class rtsp_server;

class server_connection
{
public:
    CK_API server_connection( rtsp_server* server,
                              std::shared_ptr<cppkit::ck_socket> clientSocket = new cppkit::ck_socket );

    CK_API virtual ~server_connection() throw();

    CK_API void startup();
    CK_API void shutdown();

    CK_API inline std::shared_ptr<cppkit::ck_socket> get_socket() { return _clientSocket; }

    CK_API bool running() const;

    CK_API void write_interleaved_packet( uint8_t channel, std::shared_ptr<cppkit::ck_memory> buffer );

    CK_API cppkit::ck_string get_session_id() const { return _sessionID; }

private:
    server_connection( const server_connection& obj ) = delete;
    server_connection& operator = ( const server_connection& obj ) = delete;

    void _entry_point();

    std::thread _thread;
    std::shared_ptr<cppkit::ck_socket> _clientSocket;
    rtsp_server* _server;
    cppkit::ck_string _sessionID;
    //Called "myrunning" even though it sounds douche, because it indicates the base class has a "running" member
    bool _myrunning;
    bool _isStarted;
};

}

#endif
