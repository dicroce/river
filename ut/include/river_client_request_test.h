
#include "framework.h"

#include "cppkit/ck_string.h"

class river_client_request_test : public test_fixture
{
public:

    TEST_SUITE(river_client_request_test);
      TEST(river_client_request_test::test_constructor);
      TEST(river_client_request_test::test_assignment_op);
      TEST(river_client_request_test::test_set_method);
      TEST(river_client_request_test::test_set_header);
      TEST(river_client_request_test::test_set_user_agent);
      TEST(river_client_request_test::test_set_server_port);
      TEST(river_client_request_test::test_write_request);
    TEST_SUITE_END();

    virtual ~river_client_request_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_constructor();
    void test_assignment_op();
    void test_set_method();
    void test_set_header();
    void test_set_user_agent();
    void test_set_server_port();
    void test_write_request();
};
