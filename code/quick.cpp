#include "base_inc.h"
#include "win32_base_inc.h"

static void os_http_init(); // setup arena and stuff
static void os_http_request();
static void os_http_close(); // free arena

typedef struct HTTP_Response{
    bool ok;
    String8 body;
} HTTP_Response;

int main(){
    ScratchArena scratch = begin_scratch(0);

    HINTERNET hSession = 0;
    HINTERNET hConnect = 0;
    HINTERNET hRequest = 0;
    HINTERNET hResults = 0;
    bool bResults = 0;

    // Obtain a session handle.
    hSession = WinHttpOpen(0, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    String8 hostname = str8_lit("www.rafikrizik.com");
    String16 hostname_16 = os_utf16_from_utf8(scratch.arena, hostname);
    if(hSession){
        hConnect = WinHttpConnect(hSession, (wchar*)hostname_16.str, INTERNET_DEFAULT_HTTPS_PORT, 0 );
    }

    // Create an HTTP Request handle.
    String8 endpoint = str8_lit("/expanding-ideas-any-colony-sim/");
    String16 endpoint_16 = os_utf16_from_utf8(scratch.arena, endpoint);
    if(hConnect){
        hRequest = WinHttpOpenRequest(hConnect, L"GET", (wchar*)endpoint_16.str, 0, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    }

    // Send a request.
    if(hRequest){
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    }

    // End the request.
    if(bResults){
        bResults = WinHttpReceiveResponse(hRequest, 0);
    }

    if(bResults){

        String8List response_data = {0};
        u32 downloaded = 0;
        u32 size = 0;
        do {
            if(!WinHttpQueryDataAvailable(hRequest, (DWORD*)&size)){
                print_last_error(GetLastError());
            }

            if(size){
                String8 buffer = {0};
                buffer.str = push_array_zero(scratch.arena, u8, size + 1);
                buffer.size = size;

                if(!WinHttpReadData(hRequest, (void*)buffer.str, size, (DWORD*)&downloaded)){
                    print_last_error(GetLastError());
                }
                u32 a = 1;
            }
        } while(size > 0);

    }

    if(!bResults){
        print_last_error(GetLastError());
    }

    // Close handles.
    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    if(hSession) WinHttpCloseHandle(hSession);

    return(0);
}
