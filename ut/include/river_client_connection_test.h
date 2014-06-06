
#include "framework.h"

#include "cppkit/ck_string.h"

class river_client_connection_test : public test_fixture
{
public:

    TEST_SUITE(river_client_connection_test);
      TEST(river_client_connection_test::test_connect);
    TEST_SUITE_END();

    virtual ~river_client_connection_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_connect();
};
