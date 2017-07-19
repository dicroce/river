
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

#ifndef _river_server_response_h
#define _river_server_response_h

#include "cppkit/ck_socket.h"
#include "cppkit/interfaces/ck_stream_io.h"
#include "cppkit/ck_memory.h"

#include "river/status.h"
#include "river/methods.h"

#include <map>
#include <memory>

namespace river
{

class server_response
{
public:
    CK_API server_response();
    CK_API server_response( const server_response& obj );
    CK_API server_response( server_response&& obj ) throw();

    CK_API virtual ~server_response() throw();

    CK_API server_response& operator = ( const server_response& rhs );
    CK_API server_response& operator = ( server_response&& rhs ) throw();

    CK_API void set_status( status s );
    CK_API status get_status() const;

    CK_API void set_header( const cppkit::ck_string& key, const cppkit::ck_string& value );
    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& value );

    CK_API void set_body( std::shared_ptr<cppkit::ck_memory> body );
    CK_API std::shared_ptr<cppkit::ck_memory> get_body() const;

    CK_API void set_body( const cppkit::ck_string& body );
    CK_API cppkit::ck_string get_body_as_string() const;

    CK_API void write_response( cppkit::ck_stream_io& sok );

    // Factory Methods...
    //

    CK_API static server_response create_response_to( method m );

private:
    status _status;
    std::map<std::string, cppkit::ck_string> _headers;
    std::shared_ptr<cppkit::ck_memory> _body;
};

}

#endif
