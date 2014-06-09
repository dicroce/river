
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

#ifndef _river_session_base_h
#define _river_session_base_h

#include <ctime>
#include <memory>
#include "cppkit/ck_string.h"

#include "river/server_request.h"
#include "river/server_response.h"

namespace river
{

class rtsp_server;

class session_base
{
public:
    CK_API session_base( rtsp_server& server ) :
        _server( server )
    {
    }

    CK_API session_base( rtsp_server& server, uint64_t sessionTimeOutSeconds ) :
        _server( server ),
        _sessionTimeOutSeconds( sessionTimeOutSeconds )
    {
    }

    CK_API virtual ~session_base() throw() {}

    CK_API virtual std::shared_ptr<session_base> clone() const = 0;

    CK_API virtual bool handles_this_presentation( const cppkit::ck_string& presentation ) = 0;

    CK_API virtual std::shared_ptr<server_response> handle_request( std::shared_ptr<server_request> request ) = 0;

    CK_API cppkit::ck_string get_session_id() const { return _sessionId; }
    CK_API void set_session_id( cppkit::ck_string id ) { _sessionId = id; }

    CK_API std::chrono::steady_clock::time_point get_last_keep_alive_time() const { return _lastKeepAliveTime; }
    CK_API virtual void update_keep_alive_time() { _lastKeepAliveTime = std::chrono::steady_clock::now(); }

    CK_API uint64_t get_timeout_interval_seconds() const { return _sessionTimeOutSeconds; }

    CK_API virtual cppkit::ck_string get_supported_options() { return "OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN"; }

protected:
    rtsp_server& _server;
    cppkit::ck_string _sessionId;
    std::chrono::steady_clock::time_point _lastKeepAliveTime;
    uint64_t _sessionTimeOutSeconds;

private:
    session_base( const session_base& obj ) = delete;
    session_base& operator = ( const session_base& obj ) = delete;
};

}

#endif
