
#ifndef _river_stream_keeper_h
#define _river_stream_keeper_h

#include <map>
#include <mutex>
#include <thread>

#include "cppkit/ck_socket.h"

namespace river
{

class river_server;

class stream_keeper
{
public:
    CK_API stream_keeper( river_server& server  );
    CK_API virtual ~stream_keeper() throw();

    CK_API void start();
    CK_API void stop();

private:
    stream_keeper( const stream_keeper& obj );
    stream_keeper& operator = ( const stream_keeper& obj );

    void _entry_point();

    river_server& _server;
    std::map<std::string, cppkit::ck_string> _sessions;
    bool _running;
    std::thread _thread;
};

}

#endif
