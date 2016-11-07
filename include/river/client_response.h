
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

#ifndef river_client_response_h
#define river_client_response_h

#include "river/status.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_memory.h"
#include <map>
#include <memory>
#include <vector>

namespace river
{

class client_response
{
public:
    CK_API client_response();
    CK_API client_response( const client_response& rhs );
    CK_API client_response( client_response&& rhs ) throw();
    CK_API virtual ~client_response() throw();

    CK_API client_response& operator = ( const client_response& obj );
    CK_API client_response& operator = ( client_response&& obj ) throw();

    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& value ) const;

    CK_API cppkit::ck_memory get_body() const;
    CK_API cppkit::ck_string get_body_as_string() const;

    CK_API void read_response( cppkit::ck_stream_io& sok );

    CK_API status get_status() const;

    CK_API cppkit::ck_string get_session() const;

    CK_API std::vector<cppkit::ck_string> get_methods() const;

private:
    void _receive_body( cppkit::ck_stream_io& sok, size_t bodySize );
    size_t _parse_lines( std::vector<cppkit::ck_string>& lines );

    std::map<std::string, cppkit::ck_string> _headerPieces;
    status _statusCode;
    cppkit::ck_memory _body;
};

}

#endif
