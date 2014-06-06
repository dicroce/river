
#ifndef _river_server_response_h
#define _river_server_response_h

#include "cppkit/ck_socket.h"
#include "cppkit/ck_stream_io.h"
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
    CK_API server_response( server_response&& obj ) noexcept;

    CK_API virtual ~server_response() noexcept;

    CK_API server_response& operator = ( const server_response& rhs );
    CK_API server_response& operator = ( server_response&& rhs ) noexcept;

    CK_API void set_status( status s );
    CK_API status get_status() const;

    CK_API void set_header( const cppkit::ck_string& key, const cppkit::ck_string& value );
    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& value );

    CK_API void set_body( std::shared_ptr<cppkit::ck_memory> body );
    CK_API std::shared_ptr<cppkit::ck_memory> get_body() const;

    CK_API void set_body( const cppkit::ck_string& body );
    CK_API cppkit::ck_string get_body_as_string() const;

    CK_API void write_response( std::shared_ptr<cppkit::ck_stream_io> sok );

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
