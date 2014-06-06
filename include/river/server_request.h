
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
    CK_API server_request( server_request&& rhs ) noexcept;

    CK_API virtual ~server_request() throw();

    CK_API server_request& operator = ( const server_request& rhs );
    CK_API server_request& operator = ( server_request&& rhs ) noexcept;

    CK_API method get_method() const;

    CK_API cppkit::ck_string get_uri() const;

    CK_API cppkit::ck_string get_resource_path() const;

    CK_API cppkit::ck_string get_resource() const;

    CK_API void set_peer_ip( const cppkit::ck_string& peerIP );
    CK_API cppkit::ck_string get_peer_ip() const;

    CK_API void set_local_ip( const cppkit::ck_string& localIP );
    CK_API cppkit::ck_string get_local_ip() const;

    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& value ) const;
    CK_API void set_header( const cppkit::ck_string& key, const cppkit::ck_string& value );

    CK_API void read_request( std::shared_ptr<cppkit::ck_stream_io> sok );

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
