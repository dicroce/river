
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

#ifndef river_client_request_h
#define river_client_request_h

#include "cppkit/ck_string.h"
#include "cppkit/ck_stream_io.h"
#include "river/methods.h"
#include <memory>
#include <map>

namespace river
{

class client_request
{
public:
    CK_API client_request( method m = M_OPTIONS );
    CK_API client_request( const client_request& rhs );
    CK_API client_request( client_request&& rhs ) noexcept;

    CK_API virtual ~client_request() noexcept;

    CK_API client_request& operator = ( const client_request& rhs );
    CK_API client_request& operator = ( client_request&& rhs ) noexcept;

    CK_API void set_method( method m );
    CK_API method get_method() const;

    CK_API void set_user_agent( const cppkit::ck_string& userAgent );
    CK_API cppkit::ck_string get_user_agent() const;

    CK_API void set_header( const cppkit::ck_string& name,
                            const cppkit::ck_string& value );
    CK_API bool get_header( const cppkit::ck_string& name,
                            cppkit::ck_string& value ) const;

    CK_API void set_server_ip( const cppkit::ck_string& serverIP );
    CK_API cppkit::ck_string get_server_ip() const;

    CK_API void set_server_port( int port );
    CK_API int get_server_port() const;

    CK_API void set_uri( const cppkit::ck_string& uri );
    CK_API cppkit::ck_string get_uri() const;

    CK_API void write_request( std::shared_ptr<cppkit::ck_stream_io> sok );

private:
    method _method;
    cppkit::ck_string _userAgent;
    std::map<std::string, cppkit::ck_string> _additionalHeaders;
    cppkit::ck_string _serverIP;
    int _serverPort;
    cppkit::ck_string _uri;
};

}

#endif
