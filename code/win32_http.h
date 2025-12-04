
#pragma comment(lib, "WinHttp")
#include <winhttp.h>
#include "base_types.h"

static void http_open(){
    HINTERNET hConnect = 0;
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL, NULL, NULL, NULL, 0);
}
