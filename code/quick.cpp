#include "base_inc.h"
#include "win32_base_inc.h"

static void os_http_init(); // setup arena and stuff
static void os_http_request();
static void os_http_close(); // free arena

typedef struct HTTP_Response{
    bool ok;
    String8 body;
} HTTP_Response;

typedef struct Win32_HTTP_State{
    Arena* arena;
    HINTERNET hSession;
} Win32_HTTP_State;

global Win32_HTTP_State* win32_http_state = 0;

static void http_init(u32 size=0){
    if(win32_http_state == 0){
        Arena* arena = 0;
        if(size){
            arena = make_arena(size);
        }
        else{
            arena = make_arena(MB(1));
        }

        win32_http_state = push_struct(arena, Win32_HTTP_State);
        win32_http_state->arena = arena;
        win32_http_state->hSession = WinHttpOpen(0, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    }
}

int main(){
    ScratchArena scratch = begin_scratch(0);

    http_init();
    HINTERNET hConnect = 0;
    HINTERNET hRequest = 0;
    HINTERNET hResults = 0;
    bool bResults = 0;

    // Obtain a session handle.
    win32_http_state->hSession = WinHttpOpen(0, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    String8 hostname = str8_lit("www.rafikrizik.com");
    String16 hostname_16 = os_utf16_from_utf8(scratch.arena, hostname);
    if(win32_http_state->hSession){
        hConnect = WinHttpConnect(win32_http_state->hSession, (wchar*)hostname_16.str, INTERNET_DEFAULT_HTTPS_PORT, 0 );
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

                str8_list_push(scratch.arena, &response_data, buffer);
            }
        } while(size > 0);

        String8 result = str8_join(win32_http_state->arena, &response_data, 0);
        u32 a = 1;
    }

    if(!bResults){
        print_last_error(GetLastError());
    }
    else{
    }

    // Close handles.
    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    if(win32_http_state->hSession) WinHttpCloseHandle(win32_http_state->hSession);

    return(0);
}
