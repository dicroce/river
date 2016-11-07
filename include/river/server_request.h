
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

#ifndef river_server_request_h
#define river_server_request_h

#include "river/methods.h"

#include "cppkit/ck_socket.h"
#include "cppkit/ck_stream_io.h"
#include <map>
#include <list>

namespace river
{

static const unsigned int MAX_HEADER_LINE = 2048;

class server_request
{
public:
    CK_API server_request();
    CK_API server_request( const server_request& rhs );
    CK_API server_request( server_request&& rhs ) throw();

    CK_API virtual ~server_request() throw();

    CK_API server_request& operator = ( const server_request& rhs );
    CK_API server_request& operator = ( server_request&& rhs ) throw();

    CK_API method get_method() const;

    CK_API cppkit::ck_string get_url() const;
    CK_API cppkit::ck_string get_uri() const;

    CK_API void set_peer_ip( const cppkit::ck_string& peerIP );
    CK_API cppkit::ck_string get_peer_ip() const;

    CK_API void set_local_ip( const cppkit::ck_string& localIP );
    CK_API cppkit::ck_string get_local_ip() const;

    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& value ) const;
    CK_API void set_header( const cppkit::ck_string& key, const cppkit::ck_string& value );

    CK_API void read_request( cppkit::ck_stream_io& sok );

private:
    CK_API void _parse_initial_line( cppkit::ck_string& str );
    CK_API void _parse_additional_lines( const cppkit::ck_string& str );

    cppkit::ck_string _initialLine;
    std::list<cppkit::ck_string> _requestLines;
    std::map<std::string, cppkit::ck_string> _headerParts;
    cppkit::ck_string _peerIP;
    cppkit::ck_string _localIP;
};

}

#endif
