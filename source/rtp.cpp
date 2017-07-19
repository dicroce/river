
#include "river/rtp.h"

#include "cppkit/ck_byte_ptr.h"
#include "cppkit/ck_socket.h"

using namespace cppkit;
using namespace river;
using namespace std;

uint16_t river::rtp_parse_sequence( ck_byte_ptr packet )
{
    packet += 2;

    uint16_t sequenceNumber = ck_ntohs( *(uint16_t*)packet );

    return sequenceNumber;
}

bool river::rtp_parse_marker( ck_byte_ptr packet )
{
    packet += 1;
    const uint8_t MARKER_MASK = 0x80;
    uint8_t byte = packet.read<uint8_t>();
    return (byte & MARKER_MASK) > 0;
}

uint32_t river::rtp_parse_timestamp( ck_byte_ptr packet )
{
    packet += 4;

    uint32_t ts = ck_ntohl( *(uint32_t*)packet );

    return ts;
}

ck_byte_ptr river::rtp_parse_payload( ck_byte_ptr packet )
{
    const uint8_t EXT_MASK = 0x10;
    const uint8_t CSRC_COUNT_MASK = 0xF;

    uint8_t firstByte = packet.read<uint8_t>();

    uint32_t extension = firstByte & EXT_MASK;
    uint32_t csrcCount = firstByte & CSRC_COUNT_MASK;

    packet += 12 + (csrcCount * sizeof(uint32_t));

    if( extension )
    {
        uint16_t shortVal = packet.read<uint16_t>();
        uint16_t numExtWords = ck_ntohs(shortVal);
        packet += 2 + (numExtWords * sizeof(uint32_t));
    }

    return ck_byte_ptr( packet.get_ptr(), (packet.length() - packet.offset()) );
}
