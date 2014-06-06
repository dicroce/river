
#include "river/server_request.h"
#include "river/river_exception.h"

using namespace river;
using namespace cppkit;
using namespace std;

server_request::server_request() :
    _initialLine(),
    _requestLines(),
    _headerParts(),
    _peerIP(),
    _localIP()
{
}

server_request::server_request( const server_request& rhs ) :
    _initialLine( rhs._initialLine ),
    _requestLines( rhs._requestLines ),
    _headerParts( rhs._headerParts ),
    _peerIP( rhs._peerIP ),
    _localIP( rhs._localIP )
{
}

server_request::server_request( server_request&& rhs ) noexcept :
    _initialLine( std::move( rhs._initialLine ) ),
    _requestLines( std::move( rhs._requestLines ) ),
    _headerParts( std::move( rhs._headerParts ) ),
    _peerIP( std::move( rhs._peerIP ) ),
    _localIP( std::move( rhs._localIP ) )
{
}

server_request::~server_request() throw()
{
}

server_request& server_request::operator = ( const server_request& rhs )
{
    _initialLine = rhs._initialLine;
    _requestLines = rhs._requestLines;
    _headerParts = rhs._headerParts;
    _peerIP = rhs._peerIP;

    return *this;
}

server_request& server_request::operator = ( server_request&& rhs ) noexcept
{
    _initialLine = std::move( rhs._initialLine );
    _requestLines = std::move( rhs._requestLines );
    _headerParts = std::move( rhs._headerParts );
    _peerIP = std::move( rhs._peerIP );

    return *this;
}

method server_request::get_method() const
{
    auto found = _headerParts.find( "method" );
    if( found != _headerParts.end() )
        return river::get_method( found->second );

    CK_STHROW( rtsp_400_exception, ( "Unknown method." ));
}

ck_string server_request::get_uri() const
{
    auto found = _headerParts.find( "uri" );
    if( found != _headerParts.end() )
        return found->second;

    CK_STHROW( rtsp_400_exception, ( "URI not found." ));
}

ck_string server_request::get_resource_path() const
{
    auto found = _headerParts.find( "resource_path" );
    if( found != _headerParts.end() )
        return found->second;

    CK_STHROW( rtsp_400_exception, ( "resource path not found." ));
}

ck_string server_request::get_resource() const
{
    ck_string resourcePath = get_resource_path();

    size_t rightSlash = resourcePath.rfind( '/' );

    if( (rightSlash != string::npos) && ((resourcePath.length()-(rightSlash+1)) > 0) )
        return resourcePath.substr( rightSlash+1, (resourcePath.length()-(rightSlash+1)) );

    return resourcePath;
}

void server_request::set_peer_ip( const cppkit::ck_string& peerIP )
{
    CK_LOG_TRACE("[server_request] Setting peer ip(%s)",peerIP.c_str());
    _peerIP = peerIP;
}

ck_string server_request::get_peer_ip() const
{
    return _peerIP;
}

void server_request::set_local_ip( const cppkit::ck_string& localIP )
{
    CK_LOG_TRACE("[server_request] Setting local ip(%s)",localIP.c_str());
    _localIP = localIP;
}

ck_string server_request::get_local_ip() const
{
    return _localIP;
}

bool server_request::get_header( const ck_string& key, ck_string& value ) const
{
    auto found = _headerParts.find( key );
    if( found != _headerParts.end() )
    {
        value = found->second;
        return true;
    }

    return false;
}

void server_request::set_header( const cppkit::ck_string& key, const cppkit::ck_string& value )
{
    _headerParts.insert( std::make_pair( key, value ) );
}

void server_request::read_request( std::shared_ptr<cppkit::ck_stream_io> sok )
{
    /// First, read the initial request line...
    ck_string temp = "";
    char lineBuf[MAX_HEADER_LINE+1];
    memset( lineBuf, 0, MAX_HEADER_LINE+1 );

    char* writer = &lineBuf[0];

    bool lineDone = false;

    size_t bytesReadThisLine = 0;

    while( !lineDone && ((bytesReadThisLine+1) < MAX_HEADER_LINE) )
    {
        ssize_t bytesRead = sok->recv( writer, 1 );
        if( !sok->valid() || (bytesRead != 1) )
            CK_STHROW( rtsp_500_exception, ( "IO error reading request." ));

        bytesReadThisLine += 1;

        if( *writer == '\n' )
            lineDone = true;

        writer++;
    }

    if( !lineDone )
        CK_STHROW( rtsp_400_exception, ( "The RTSP initial request line exceeded our max." ) );

    _initialLine = lineBuf;
    temp += _initialLine;
    /// Now, read the rest of the header lines...

    bool headerDone = false;
    while( !headerDone )
    {
        memset( lineBuf, 0, MAX_HEADER_LINE );

        writer = &lineBuf[0];

        bytesReadThisLine = 0;

        lineDone = false;

        while( !lineDone && ((bytesReadThisLine+1) < MAX_HEADER_LINE) )
        {
            ssize_t bytesRead = sok->recv( writer, 1 );
            if( !sok->valid() || (bytesRead != 1) )
                CK_STHROW( rtsp_500_exception, ( "IO error reading request." ));

            bytesReadThisLine += 1;

            if( *writer == '\n' )
                lineDone = true;

            writer++;
        }

        if( !lineDone )
            CK_STHROW( rtsp_400_exception, ( "The RTSP initial request line exceeded our max." ) );

        if( (lineBuf[0] == '\r' && lineBuf[1] == '\n') || (lineBuf[0] == '\n') )
            headerDone = true;
        else
        {
            const ck_string line = lineBuf;
            _parse_additional_lines(line);
        }
        temp += lineBuf;
    }

    /// Now, populate our header hash...
    _parse_initial_line(_initialLine);
    CK_LOG_INFO("[river::server_request] Read: %s",temp.replace_all("\r","\'\\r\'").replace_all("\n","\'\\n\'").replace_all("\t","\'\\t\'").c_str());
}

void server_request::_parse_additional_lines( const ck_string& str )
{
    if( str.starts_with( " " ) || str.starts_with( "\t" ) )
    {
        auto last = _requestLines.rbegin();
        if( last != _requestLines.rend() )
            *last += str;
        else CK_STHROW( rtsp_400_exception, ( "First line of header missing needed seperator." ));
    }
    else _requestLines.push_back( str );
}

void server_request::_parse_initial_line( ck_string& str )
{
    static const ck_string protocolString("://");
    _headerParts.clear();

    const vector<ck_string> initialLineParts = str.split(' ');

    if( initialLineParts.size() != 3 )
    {
        CK_STHROW( rtsp_400_exception, ( "RTSP initial line not composed of 3 parts." ));
    }

    _headerParts.insert( std::make_pair( "method", initialLineParts[0] ) );
    _headerParts.insert( std::make_pair( "uri", initialLineParts[1] ) );

    size_t index = initialLineParts[1].find(protocolString);
    if ( index == std::string::npos )
        CK_STHROW( rtsp_400_exception, ("RTSP URIs must have a protocol type") );

    index = initialLineParts[1].find("/",index+protocolString.length());

    if ( index != string::npos && index+1 < initialLineParts[1].length() )
        _headerParts.insert( make_pair( "resource_path", initialLineParts[1].substr(index+1) ) );
    else _headerParts.insert( make_pair( "resource_path", ck_string("/") ) );

    _headerParts.insert( make_pair( "rtsp_version", initialLineParts[2] ) );

    for( auto line : _requestLines )
    {
        const vector<ck_string> lineParts = line.split( ':' );
        _headerParts.insert( std::make_pair( lineParts[0], lineParts[1].strip() ) );
    }
}
