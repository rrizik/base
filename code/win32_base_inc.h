#if !defined(WIN32_BASE_H)
#define WIN32_BASE_H

#include "win32_memory.h"
#include "win32_logging.h"
#include "win32_file.h"
#include "win32_os.h"


#define assert_hr(hr) assert((SUCCEEDED((hr))))
global HRESULT hr;


#endif

