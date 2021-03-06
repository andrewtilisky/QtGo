#include "time.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

time::time(void) {
}

time::~time(void) {
}

long time::GetTimeMs64() {

#ifdef WIN32
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed
	 * since January 1, 1601 (UTC) and copy it
	 * to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime (&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	long ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10000; // From 100 nano seconds (10^-7)

	return ret;
#else
	/* Linux */
	struct timeval tv;

	gettimeofday(&tv, 0);

	long ret = tv.tv_usec;

	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
//        ret /= 1000;
	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += (tv.tv_sec * 1000);

	return ret;
#endif
}
