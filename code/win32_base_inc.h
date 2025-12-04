#if !defined(WIN32_BASE_H)
#define WIN32_BASE_H

#define WIN32_LEAN_AND_MEAN

#include "win32_memory.h"
#include "win32_logging.h"
#include "win32_file.h"
#include "win32_http.h"
#include "win32_os.h"

#if ENABLE_ASSERT
# define assert_h(handle) do { assert(((handle) != (INVALID_HANDLE_VALUE))); } while (0)
# define assert_hr(hr)    do { HRESULT _hr = (hr); assert(SUCCEEDED(_hr)); } while (0)
#else
# define assert_h(handle)
# define assert_hr(hr)
#endif

global HRESULT hr;


#endif

