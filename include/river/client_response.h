
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
    CK_API client_response( client_response&& rhs ) noexcept;
    CK_API virtual ~client_response() noexcept;

    CK_API client_response& operator = ( const client_response& obj );
    CK_API client_response& operator = ( client_response&& obj ) noexcept;

    CK_API bool get_header( const cppkit::ck_string& key, cppkit::ck_string& value ) const;

    CK_API cppkit::ck_memory get_body() const;
    CK_API cppkit::ck_string get_body_as_string() const;

    CK_API void read_response( std::shared_ptr<cppkit::ck_stream_io> sok );

    CK_API status get_status() const;

    CK_API cppkit::ck_string get_session() const;

    CK_API std::vector<cppkit::ck_string> get_methods() const;

private:
    void _receive_body( std::shared_ptr<cppkit::ck_stream_io> sok, size_t bodySize );
    size_t _parse_lines( std::vector<cppkit::ck_string>& lines );

    std::map<std::string, cppkit::ck_string> _headerPieces;
    status _statusCode;
    cppkit::ck_memory _body;
};

}

#endif
