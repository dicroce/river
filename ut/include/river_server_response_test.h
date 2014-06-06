
#include "framework.h"

#include "cppkit/ck_string.h"

class river_server_response_test : public test_fixture
{
public:

    TEST_SUITE(river_server_response_test);
      TEST(river_server_response_test::test_constructor);
      TEST(river_server_response_test::test_assignment_op);
      TEST(river_server_response_test::test_set_status);
      TEST(river_server_response_test::test_set_header);
      TEST(river_server_response_test::test_set_body);
      TEST(river_server_response_test::test_write_response);
    TEST_SUITE_END();

    virtual ~river_server_response_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_constructor();
    void test_assignment_op();
    void test_set_status();
    void test_set_header();
    void test_set_body();
    void test_write_response();
};
