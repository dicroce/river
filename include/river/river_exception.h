
#ifndef river_river_exception_h
#define river_river_exception_h

#include "cppkit/ck_exception.h"
#include "cppkit/ck_string.h"

namespace river
{

class river_exception : public cppkit::ck_exception
{
public:
    CK_API river_exception();
    CK_API river_exception( const char* msg, ... );
    CK_API river_exception( const cppkit::ck_string& msg );
    CK_API virtual ~river_exception() throw() {}
};

class rtsp_exception : public river_exception
{
public:
    CK_API rtsp_exception( int statusCode );
    CK_API rtsp_exception( int statusCode, const char* msg, ... );
    CK_API rtsp_exception( int statusCode, const cppkit::ck_string& msg );
    CK_API virtual ~rtsp_exception() throw() {}
    CK_API int get_status() const { return _statusCode; }
private:
    int _statusCode;
};

#define rtsp_exception_class(code) \
class rtsp_##code##_exception : public rtsp_exception \
{ \
public: \
    CK_API rtsp_##code##_exception() : \
        rtsp_exception( code ) \
    { \
    } \
    CK_API rtsp_##code##_exception( const char* msg, ... ) : \
        rtsp_exception( code ) \
    { \
        va_list args; \
        va_start( args, msg ); \
        set_msg( cppkit::ck_string::format( msg, args ) );      \
        va_end( args ); \
    } \
    CK_API rtsp_##code##_exception( const cppkit::ck_string& msg ) : \
        rtsp_exception( code, msg ) \
    { \
    } \
    CK_API virtual ~rtsp_##code##_exception() throw() {} \
    CK_API int get_status() const { return _statusCode; } \
private: \
    int _statusCode; \
}

rtsp_exception_class(400);
rtsp_exception_class(401);
rtsp_exception_class(403);
rtsp_exception_class(404);
rtsp_exception_class(415);
rtsp_exception_class(453);
rtsp_exception_class(500);

/// \brief Throws an exception corresponding to the given status code
///        or a plain rtsp_exception if there isn't one.
CK_API void throw_rtsp_exception( int statusCode, const char* msg, ... );
CK_API void throw_rtsp_exception( int statusCode, const cppkit::ck_string& msg );

#define RTSP_THROW(PARAMS) \
CK_MACRO_BEGIN \
    try \
    { \
        throw_rtsp_exception PARAMS ; \
    } \
    catch(rtsp_exception& e) \
    { \
        e.set_throw_point(__LINE__, __FILE__); \
        std::vector<std::string> stack; \
        cppkit::ck_stack_trace::get_stack(stack); \
        e.set_stack(stack); \
        CK_LOG_WARNING("%s thrown. Msg: \"%s\", At: %s(%d)\n", \
                       e.get_type_name(),                      \
                       e.get_msg(),                            \
                       __FILE__,                               \
                       __LINE__);                              \
        cppkit::ck_log::log_backtrace(stack); \
        throw; \
    } \
CK_MACRO_END

}

#endif
