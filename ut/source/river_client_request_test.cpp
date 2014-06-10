
#include "framework.h"
#include "river_client_request_test.h"
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

REGISTER_TEST_FIXTURE(river_client_request_test);

void river_client_request_test::setup()
{
}

void river_client_request_test::teardown()
{
}

void river_client_request_test::test_constructor()
{
    {
        client_request req;
    }

    {
        client_request reqA;
        client_request reqB = reqA;
    }

    {
        server_request req;
    }

    {
        server_request reqA;
        server_request reqB = reqA;
    }

}

void river_client_request_test::test_assignment_op()
{
    client_request reqA;
    reqA.set_uri("/foo/bar");
    reqA.set_header( "word", "up" );
    client_request reqB;
    reqB = reqA;
    UT_ASSERT( reqB.get_uri() == "/foo/bar" );
    ck_string value;
    UT_ASSERT( reqB.get_header( "word", value ) );
    UT_ASSERT( value == "up" );
}

void river_client_request_test::test_set_method()
{
    client_request reqA;
    reqA.set_method( M_OPTIONS );
    UT_ASSERT( reqA.get_method() == M_OPTIONS );
}

void river_client_request_test::test_set_header()
{
    client_request reqA;
    reqA.set_header( "name", "value" );
    ck_string value;
    UT_ASSERT( reqA.get_header( "name", value ) );
    UT_ASSERT( value == "value" );
}

void river_client_request_test::test_set_user_agent()
{
    client_request req;
    req.set_user_agent( "your_mom" );
    UT_ASSERT( req.get_user_agent() == "your_mom" );
}

void river_client_request_test::test_set_server_port()
{
    client_request req;
    req.set_server_port( 42 );
    UT_ASSERT( req.get_server_port() == 42 );
}

void river_client_request_test::test_write_request()
{
    int port = UT_NEXT_PORT();

    // For some reason, my first impulse is to always make the thread be the server... but the nice thing about making the
    // thread be the client is that you don't need a sleep!

    // Also, you should probably always call fut.get() to propigate any exceptions thrown in the async task.

    auto fu = std::async(std::launch::async, [&](){
        client_connection cc( "127.0.0.1", port );
        UT_ASSERT( cc.connect() );
        shared_ptr<client_request> req = make_shared<client_request>( M_DESCRIBE );
        req->set_uri( "/foo/bar" );
        req->set_header( "your", "mom" );
        req->set_user_agent( "lord_stark" );
        cc.write_request( req );
    });

    ck_socket server_sok;
    server_sok.bind( port, ip4_addr_any );
    server_sok.listen();
    auto connected = server_sok.accept();
    server_request sreq;
    sreq.read_request( connected );

    UT_ASSERT( sreq.get_method() == M_DESCRIBE );
    UT_ASSERT( sreq.get_uri() == "/foo/bar" );
    ck_string value;
    UT_ASSERT( sreq.get_header( "your", value ) );
    UT_ASSERT( value == "mom" );
    UT_ASSERT( sreq.get_header( "User-Agent", value ) );
    UT_ASSERT( value == "lord_stark" );

    fu.get(); // about it....

}
