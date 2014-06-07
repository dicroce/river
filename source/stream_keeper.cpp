
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

        for( auto i : sessions )
        {
            if( i.second->get_timeout_interval_seconds() == 0 )
                continue;

            chrono::steady_clock::time_point then = i.second->get_last_keep_alive_time();
            chrono::steady_clock::time_point now = chrono::steady_clock::now();

            int deltaSeconds = chrono::duration_cast<std::chrono::seconds>( now - then ).count();

            if( deltaSeconds < i.second->get_timeout_interval_seconds() )
                continue;

            _server.stop_session( i.second->get_session_id() );
        }

        ck_sleep( 1 );
    }
}
