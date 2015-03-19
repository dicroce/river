
#include "river/rtp.h"

#include "cppkit/ck_byte_ptr.h"
#include "cppkit/ck_bitsy.h"
#include "cppkit/ck_socket.h"

using namespace cppkit;
using namespace river;
using namespace std;

uint16_t river::rtp_parse_sequence( ck_byte_ptr packet )
{
    packet += 2;

    uint16_t sequenceNumber = x_ntohs( *(uint16_t*)packet );

    return sequenceNumber;
}

bool river::rtp_parse_marker( ck_byte_ptr packet )
{
    ck_bitsy<const uint8_t*> bitsy( packet.get_ptr(), packet.get_ptr() + packet.length() );
    bitsy.skip_bits( 8 );

    return (bitsy.get_bits( 1 ) > 0) ? true : false;
}

uint32_t river::rtp_parse_timestamp( ck_byte_ptr packet )
{
    packet += 4;

    uint32_t ts = x_ntohl( *(uint32_t*)packet );

    return ts;
}

ck_byte_ptr river::rtp_parse_payload( ck_byte_ptr packet )
{
    ck_bitsy<const uint8_t*> bitsy( packet.get_ptr(), packet.get_ptr() + packet.length() );

    bitsy.skip_bits( 3 );
    uint32_t extension = bitsy.get_bits( 1 );
    uint32_t csrcCount = bitsy.get_bits( 4 );
    bitsy.skip_bits( 8 );

    size_t payloadOffset = 12;
    packet += payloadOffset;

    for( uint32_t i = 0; i < csrcCount; i++ )
    {
        packet += 4;
        payloadOffset += 4;
    }

    if( extension == 1 )
    {
        uint32_t extensionSize = (x_ntohs( *(uint16_t*)packet ) + 1) * 4;
        packet += extensionSize;
        payloadOffset += extensionSize;
    }

    return ck_byte_ptr( packet.get_ptr(), (packet.length() - payloadOffset) );
}
