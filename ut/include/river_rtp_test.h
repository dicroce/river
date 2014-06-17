
#include "framework.h"

#include "cppkit/ck_string.h"

class river_rtp_test : public test_fixture
{
public:

    TEST_SUITE(river_rtp_test);
      TEST(river_rtp_test::test_rtp_parse_sequence);
      TEST(river_rtp_test::test_rtp_parse_marker);
      TEST(river_rtp_test::test_rtp_parse_timestamp);
      TEST(river_rtp_test::test_rtp_parse_payload);
    TEST_SUITE_END();

    virtual ~river_rtp_test() throw() {}

    virtual void setup();
    virtual void teardown();

    void test_rtp_parse_sequence();
    void test_rtp_parse_marker();
    void test_rtp_parse_timestamp();
    void test_rtp_parse_payload();
};
