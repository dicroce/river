
#include "framework.h"
#include "river_server_response_test.h"
#include "river/server_response.h"
#include "river/client_response.h"
#include "cppkit/os/ck_time_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <thread>

using namespace std;
using namespace cppkit;
using namespace river;

REGISTER_TEST_FIXTURE(river_server_response_test);

void river_server_response_test::setup()
{
}

void river_server_response_test::teardown()
{
}

void river_server_response_test::test_constructor()
{
    {
        server_response res;
    }

    {
        server_response resA;
        server_response resB = resA;
    }
}

void river_server_response_test::test_assignment_op()
{
    server_response resA;
    resA.set_status( STATUS_CONTINUE );
    resA.set_header( "word", "up" );
    resA.set_body( "this is a body" );
    server_response resB;
    resB = resA;
    UT_ASSERT( resB.get_status() == STATUS_CONTINUE );
    ck_string value;
    UT_ASSERT( resB.get_header( "word", value ) );
    UT_ASSERT( value == "up" );
    ck_string responseBody = resB.get_body_as_string();
    UT_ASSERT( responseBody == "this is a body" );
}

void river_server_response_test::test_set_status()
{
    server_response resA;
    resA.set_status( STATUS_CONTINUE );
    UT_ASSERT( resA.get_status() == STATUS_CONTINUE );
}

void river_server_response_test::test_set_header()
{
    server_response resA;
    resA.set_header( "name", "value" );
    ck_string value;
    UT_ASSERT( resA.get_header( "name", value ) );
    UT_ASSERT( value == "value" );
}

void river_server_response_test::test_set_body()
{
    server_response res;
    res.set_body( "your_mom" );
    UT_ASSERT( res.get_body_as_string() == "your_mom" );
}

#include <algorithm>

bool is_odd( int i ) { return ((i%2)==1); }

void river_server_response_test::test_write_response()
{
    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket sok;
        sok.connect( "127.0.0.1", port );

        server_response res;
        res.set_header( "boo", "ya" );
        res.set_body( "optimal d2f ratio" );

        res.write_response( sok );
    });
    t.detach();

    ck_socket serverSok;
    serverSok.bind( port, ip4_addr_any );
    serverSok.listen();
    auto connected = serverSok.accept();
    client_response cres;
    cres.read_response( connected );

    ck_string value;
    UT_ASSERT( cres.get_header( "boo", value ) );
    UT_ASSERT( value == "ya" );
    UT_ASSERT( cres.get_body_as_string() == "optimal d2f ratio" );
}
