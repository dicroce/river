
#include "framework.h"
#include "river_rtsp_server_test.h"
#include "river/rtsp_server.h"
#include "river/server_response.h"
#include "river/client_response.h"
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
    CK_API virtual shared_ptr<server_response> handle_request( shared_ptr<server_request> request )
    {
        shared_ptr<server_response> response = make_shared<server_response>();
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
