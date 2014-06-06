
#ifndef _river_session_base_h
#define _river_session_base_h

#include <ctime>
#include <memory>
#include "cppkit/ck_string.h"

#include "river/server_request.h"
#include "river/server_response.h"

namespace river
{

class river_server;

class session_base
{
public:
    CK_API session_base( river_server& server );
    CK_API session_base( river_server& server, uint64_t sessionTimeOutSeconds );
    CK_API virtual ~session_base() throw();

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
    river_server& _server;
    cppkit::ck_string _sessionId;
    std::chrono::steady_clock::time_point _lastKeepAliveTime;
    uint64_t _sessionTimeOutSeconds;

private:
    session_base( const session_base& obj );
    session_base& operator = ( const session_base& obj );
};

}

#endif
