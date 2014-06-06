
#include "framework.h"
#include "river_client_connection_test.h"
#include "river/client_connection.h"
#include "river/client_request.h"
#include "river/server_request.h"
#include "cppkit/os/ck_time_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <future>

using namespace std;
using namespace cppkit;
using namespace river;

REGISTER_TEST_FIXTURE(river_client_connection_test);

void river_client_connection_test::setup()
{
}

void river_client_connection_test::teardown()
{
}

void river_client_connection_test::test_connect()
{
    int port = UT_NEXT_PORT();

    // For some reason, my first impulse is to always make the thread be the server... but the nice thing about making the
    // thread be the client is that you don't need a sleep!

    // Also, you should probably always call fut.get() to propigate any exceptions thrown in the async task.

    auto fut = std::async(std::launch::async, [&](){
        client_connection cc( "127.0.0.1", port );
        UT_ASSERT( cc.connect() );
        client_request req( M_OPTIONS );
        req.set_resource_path( "/foo/bar" );
        cc.send_request( req );
    });

    ck_socket server_sok;
    server_sok.bind( port, ip4_addr_any );
    server_sok.listen();
    auto connected = server_sok.accept();
    server_request sreq;
    sreq.read_request( connected );

    UT_ASSERT( sreq.get_resource_path() == "/foo/bar" );

    fut.get();
}
