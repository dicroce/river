
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

    CK_API void set_resource_path( const cppkit::ck_string& resourcePath );
    CK_API cppkit::ck_string get_resource_path() const;

    CK_API void write_request( std::shared_ptr<cppkit::ck_stream_io> sok );

private:
    method _method;
    cppkit::ck_string _userAgent;
    std::map<std::string, cppkit::ck_string> _additionalHeaders;
    cppkit::ck_string _serverIP;
    int _serverPort;
    cppkit::ck_string _resourcePath;
};

}

#endif
