
#include "framework.h"
#include "river_rtp_test.h"
#include "river/rtp.h"
#include "cppkit/ck_exception.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_socket.h"

#include "rtp_stream.cpp"

using namespace std;
using namespace cppkit;
using namespace river;

REGISTER_TEST_FIXTURE(river_rtp_test);

void river_rtp_test::setup()
{
}

void river_rtp_test::teardown()
{
}

void river_rtp_test::test_rtp_parse_sequence()
{
    uint32_t rtpPort = 0;
    uint8_t* reader = RTP_STREAM;
    uint32_t bytesToRead = RTP_STREAM_SIZE;

    uint16_t cmpSeq = 36397;

    while( bytesToRead > 0 )
    {
        uint32_t packetSize = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        uint32_t port = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        if( rtpPort == 0 )
            rtpPort = port;

        if( port == rtpPort )
        {
            uint16_t seq = river::rtp_parse_sequence( ck_byte_ptr( reader, packetSize ) );
            UT_ASSERT( seq == cmpSeq );
            cmpSeq++;
        }

        reader+=packetSize;
        bytesToRead-=packetSize;
    }
}

void river_rtp_test::test_rtp_parse_marker()
{
    uint32_t rtpPort = 0;
    uint8_t* reader = RTP_STREAM;
    uint32_t bytesToRead = RTP_STREAM_SIZE;
    size_t numMarkers = 0;
    size_t numPackets = 0;
    while( bytesToRead > 0 )
    {
        uint32_t packetSize = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        uint32_t port = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        if( rtpPort == 0 )
            rtpPort = port;

        if( port == rtpPort )
        {
            if( river::rtp_parse_marker( ck_byte_ptr( reader, packetSize ) ) )
                numMarkers++;
            else numPackets++;
        }

        reader+=packetSize;
        bytesToRead-=packetSize;
    }

    UT_ASSERT( numMarkers == 50 );
    UT_ASSERT( numPackets == 949 );
}

void river_rtp_test::test_rtp_parse_timestamp()
{
    uint32_t rtpPort = 0;
    uint8_t* reader = RTP_STREAM;
    uint32_t bytesToRead = RTP_STREAM_SIZE;
    uint32_t lastTS = 0;
    bool lastTSValid = false;
    while( bytesToRead > 0 )
    {
        uint32_t packetSize = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        uint32_t port = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        if( rtpPort == 0 )
            rtpPort = port;

        if( port == rtpPort )
        {
            uint32_t ts = river::rtp_parse_timestamp( ck_byte_ptr( reader, packetSize ) );

            if( lastTSValid && lastTS != ts )
            {
                UT_ASSERT( ts > lastTS );
                UT_ASSERT( (ts - lastTS) >= 5995 && (ts - lastTS) <= 6010 );
            }

            lastTSValid = true;
            lastTS = ts;
        }

        reader+=packetSize;
        bytesToRead-=packetSize;
    }
}

void river_rtp_test::test_rtp_parse_payload()
{
    uint32_t rtpPort = 0;
    uint8_t* reader = RTP_STREAM;
    uint32_t bytesToRead = RTP_STREAM_SIZE;
    while( bytesToRead > 0 )
    {
        uint32_t packetSize = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        uint32_t port = ntohl( *(uint32_t*)reader );
        reader+=4;
        bytesToRead-=4;

        if( rtpPort == 0 )
            rtpPort = port;

        if( port == rtpPort )
        {
            ck_byte_ptr payload = river::rtp_parse_payload( ck_byte_ptr( reader, packetSize) );

            UT_ASSERT( payload.length() < 1500 ); // packets must be smaller than the MTU
        }

        reader+=packetSize;
        bytesToRead-=packetSize;
    }
}
