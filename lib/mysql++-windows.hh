// Not used right now.  It's a parking place for code removed from
// several modules because it is bogus.  If this code is useful at all,
// it's only on some Windows C++ compilers, not all.  In any case, if
// something like this is needed, it should go in this file, and then
// the modules that need it should #include this file.

#ifdef __WIN32__
#include <Windows32/Base.h>
#include <Windows32/Defines.h>
#include <Windows32/Structures.h>
#include <winsock.h>
#define errno WSAGetLastError()
#endif

