
#include "river/stream_keeper.h"
#include "river/river_server.h"
#include "cppkit/ck_logger.h"
#include "cppkit/os/ck_time_utils.h"

using namespace river;
using namespace cppkit;
using namespace std;

stream_keeper::stream_keeper( river_server& server ) :
    _server( server ),
    _sessions(),
    _running( false ),
    _thread()
{
}

stream_keeper::~stream_keeper() throw()
{
    if( _running )
        stop();
}

void stream_keeper::start()
{
    _running = true;
    _thread = std::thread( &stream_keeper::_entry_point, this );
}

void stream_keeper::stop()
{
   _running = false;
   _thread.join();
}

void stream_keeper::_entry_point()
{
    while( _running )
    {
        map<string, shared_ptr<session_base> > sessions = _server.get_sessions();

        for( auto i = sessions.begin(), end = sessions.end(); i != end; i++ )
        {
            if( i->second->get_timeout_interval_seconds() == 0 )
                continue;

            chrono::steady_clock::time_point then = i->second->get_last_keep_alive_time();
            chrono::steady_clock::time_point now = chrono::steady_clock::now();

            int deltaSeconds = chrono::duration_cast<std::chrono::seconds>( now - then ).count();

            if( deltaSeconds < i->second->get_timeout_interval_seconds() )
                continue;

            _server.stop_session( i->second->get_session_id() );
        }

        ck_sleep( 1 );
    }
}
