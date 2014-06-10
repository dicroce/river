
#include "framework.h"
#include "river_rtsp_server_test.h"
#include "river/rtsp_server.h"
#include "river/client_request.h"
#include "river/server_request.h"
#include "river/server_response.h"
#include "river/client_response.h"
#include "river/client_connection.h"
#include "cppkit/os/ck_time_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <future>

using namespace std;
using namespace cppkit;
using namespace river;

REGISTER_TEST_FIXTURE(river_rtsp_server_test);

class simple_session : public session_base
{
public:
    CK_API simple_session( rtsp_server& server ) : session_base( server ) {}
    CK_API simple_session( rtsp_server& server, uint64_t sessionTimeOutSeconds ) :
        session_base( server, sessionTimeOutSeconds ) {}
    CK_API virtual ~simple_session() throw() {}
    CK_API virtual shared_ptr<session_base> clone() const { return make_shared<simple_session>( _server ); }
    CK_API virtual bool handles_this_presentation( const ck_string& presentation ) { return presentation.contains( "/foo/bar" ); }
    CK_API virtual ck_string get_supported_options() { return "OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN"; }
    CK_API virtual shared_ptr<server_response> handle_request( shared_ptr<server_request> request )
    {
        shared_ptr<server_response> response = make_shared<server_response>();

        switch( request->get_method() )
        {
        case M_DESCRIBE:
            response->set_body("THIS IS MY SDP");
            break;
        case M_SETUP:
            break;
        case M_PLAY:
            break;
        case M_TEARDOWN:
            break;
        case M_GET_PARAM:
            break;
        default:
            break;
        };

        return response;
    }
private:
};

void river_rtsp_server_test::setup()
{
}

void river_rtsp_server_test::teardown()
{
}

void river_rtsp_server_test::test_constructor()
{
    int port = UT_NEXT_PORT();

    rtsp_server server( ip4_addr_any, port );
}

void river_rtsp_server_test::test_start_stop()
{
    int port = UT_NEXT_PORT();

    rtsp_server server( ip4_addr_any, port );
    server.attach_session_prototype( make_shared<simple_session>( server ) );
    server.start();
    server.stop();
}

void river_rtsp_server_test::test_options()
{
    int port = UT_NEXT_PORT();

    rtsp_server server( ip4_addr_any, port );
    server.attach_session_prototype( make_shared<simple_session>( server ) );
    server.start();
    shared_ptr<client_request> req = make_shared<client_request>( M_OPTIONS );
    req->set_uri( "/foo/bar" );
    client_connection conn( "127.0.0.1", port );
    conn.connect();
    conn.write_request( req );
    shared_ptr<client_response> res = conn.read_response();
    ck_string value;
    UT_ASSERT( res->get_header( "Public", value ) );
    UT_ASSERT( value == "OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN" );
    UT_ASSERT( res->get_status() == S_OK );
    server.stop();
}

void river_rtsp_server_test::test_describe()
{
    int port = UT_NEXT_PORT();

    rtsp_server server( ip4_addr_any, port );
    server.attach_session_prototype( make_shared<simple_session>( server ) );
    server.start();
    shared_ptr<client_request> req = make_shared<client_request>( M_DESCRIBE );
    req->set_uri( "/foo/bar" );
    client_connection conn( "127.0.0.1", port );
    conn.connect();
    conn.write_request( req );
    shared_ptr<client_response> res = conn.read_response();
    UT_ASSERT( res->get_body_as_string() == "THIS IS MY SDP" );
    UT_ASSERT( res->get_status() == S_OK );
    server.stop();
}
