
#include "framework.h"

#include "cppkit/ck_string.h"

class river_rtsp_server_test : public test_fixture
{
public:

    TEST_SUITE(river_rtsp_server_test);
      TEST(river_rtsp_server_test::test_constructor);
      TEST(river_rtsp_server_test::test_start_stop);
    TEST_SUITE_END();

    virtual ~river_rtsp_server_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_constructor();
    void test_start_stop();
};
