
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// river
/// Copyright (c) 2014, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#include "river/client_response.h"
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;
using namespace std;

client_response::client_response() :
    _headerPieces(),
    _statusCode(),
    _body()
{
}

client_response::client_response( const client_response& rhs ) :
    _headerPieces( rhs._headerPieces ),
    _statusCode( rhs._statusCode ),
    _body( rhs._body )
{
}

client_response::client_response( client_response&& rhs ) noexcept :
    _headerPieces( std::move( rhs._headerPieces ) ),
    _statusCode( std::move( rhs._statusCode ) ),
    _body( std::move( rhs._body ) )
{
}

client_response::~client_response() noexcept
{
}

client_response& client_response::operator = ( const client_response& obj )
{
    _headerPieces = obj._headerPieces;
    _statusCode = obj._statusCode;
    _body = obj._body;

    return *this;
}

client_response& client_response::operator = ( client_response&& obj ) noexcept
{
    _headerPieces = std::move( obj._headerPieces );
    _statusCode = std::move( obj._statusCode );
    _body = std::move( obj._body );

    return *this;
}

bool client_response::get_header( const ck_string& key, ck_string& value ) const
{
    auto found = _headerPieces.find( key );

    if( found != _headerPieces.end() )
    {
        value = (*found).second;
        return true;
    }

    return false;
}

ck_memory client_response::get_body() const
{
    return _body;
}

ck_string client_response::get_body_as_string() const
{
    return ck_string( (char*)_body.map().get_ptr(), _body.size_data() );
}

void client_response::read_response( shared_ptr<ck_stream_io> sok )
{
    do
    {
        bool messageEnd = false;
        std::vector<ck_string> lines;
        size_t bodySize = 0;

        do
        {
            ck_string line;
            int recvCount = 0;
            int foundCount = 0;
            bool foundCarriageReturn = false;

            do
            {
                char character = 0;
                recvCount = (int)sok->recv( &character, 1 );

                if( !sok->valid() || (recvCount != 1) )
                    CK_STHROW(river_exception, ("Failed to read or invalid socket."));

                if ( character == '\n' )
                {
                    lines.push_back(line);
                    break;
                }

                if( character != '\r' )
                {
                    if ( foundCarriageReturn )
                        CK_STHROW(river_exception, ("Failed to find a line feed and have found a Carriage Return before current character(%c)",character));
                    line += character;
                    ++foundCount;
                }
                else foundCarriageReturn = true;

            } while(recvCount>0);

            if ( recvCount == 0 && foundCount != 0 )
                CK_STHROW(river_exception, ("Failed to find end line but not finding data"));

            messageEnd = foundCount == 0;

        }while(!messageEnd);

        bodySize = _parse_lines( lines );

        if ( bodySize > 0 )
            _receive_body(sok, bodySize);

    }while(_statusCode == STATUS_CONTINUE);
}

status client_response::get_status() const
{
    return _statusCode;
}

ck_string client_response::get_session() const
{
    ck_string value;
    if( get_header("Session",value) )
        return value;
    CK_STHROW(river_exception, ("There is no Session Key!"));
}

vector<ck_string> client_response::get_methods() const
{
    ck_string value;
    if( get_header("Public",value) )
    {
        std::vector<ck_string> methods = value.split(',');
        for( size_t i=0; i < methods.size(); ++i )
            methods[i] = methods[i].strip();
        return methods;
    }
    CK_STHROW(river_exception, ("No \"Public\" method found!"));
}

void client_response::_receive_body( shared_ptr<ck_stream_io> sok, size_t bodySize )
{
    _body.clear();

    size_t bytesRead = sok->recv( _body.extend_data(bodySize).get_ptr(), bodySize );
    if( !sok->valid() || (bytesRead != bodySize) )
        CK_STHROW(river_exception, ("IO error or invalid socket."));
}

size_t client_response::_parse_lines( vector<ck_string>& lines )
{
    size_t result = 0;

    for ( size_t i = 0; i < lines.size(); ++i )
    {
        if ( lines[i].contains("RTSP") )
        {
            std::vector<ck_string> parts = lines[i].split(' ');

            if ( parts.size() < 2 )
                CK_STHROW(river_exception, ("Not enough parts in status line."));

            _statusCode = convert_status_code_from_int(parts[1].to_int());

            continue;
        }

        std::vector<ck_string> parts = lines[i].split(": ");

        if ( lines[i].empty() )
            continue;

        if ( parts.size() != 2 )
            CK_STHROW(river_exception, ("Wrong number of parts"));

        if ( lines[i].contains("Content-Length") )
            result = parts[1].to_int();

        _headerPieces.insert(make_pair(parts[0],parts[1]));
    }

    return result;
}
