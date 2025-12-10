#pragma comment(lib, "WinHttp")
#include <winhttp.h>
#include "base_types.h"
#include "base_string.h"

typedef struct HTTP_Response{
    bool ok;
    u32 status_code;
    String8 body;
} HTTP_Response;

typedef struct HTTP_Info{
    String8 method;
    String8List params;
    String8 content_type;
    String8 auth;
    String8 user_agent;
    String8 cookie;
} HTTP_Info;

#define HTTP_DEFAULT_COMMIT_SIZE MB(1)
typedef struct HTTP_State{
    Arena* arena;
    HINTERNET hSession;
} HTTP_State;
global HTTP_State* win32_http_state = 0;

static void
http_init(u32 size=0){
    if(win32_http_state == 0){
        Arena* arena = 0;
        if(size){
            arena = make_arena(size);
        }
        else{
            arena = make_arena(HTTP_DEFAULT_COMMIT_SIZE);
        }

        win32_http_state = push_struct(arena, HTTP_State);
        win32_http_state->arena = arena;
        // Obtain a session handle.
        win32_http_state->hSession = WinHttpOpen(0, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    }
}

static void
http_close(){
    if (!win32_http_state) return;
    if(win32_http_state->hSession) WinHttpCloseHandle(win32_http_state->hSession);

    Arena* arena = win32_http_state->arena;
    win32_http_state = 0;
    arena_free(arena);
}

static HTTP_Response
http_request(String8 url, HTTP_Info* info){
    ScratchArena scratch = begin_scratch(0);

    HINTERNET hConnect = 0;
    HINTERNET hRequest = 0;
    bool ok = false;
    u32 status_code;

    u32 flags = 0;
    u16 port = INTERNET_DEFAULT_HTTP_PORT;
    if(str8_starts_with_nocase(url, str8_lit("https://"))){
        port = INTERNET_DEFAULT_HTTPS_PORT;
        flags |= WINHTTP_FLAG_SECURE;
        str8_trim_left(&url, 8);
    }
    else if(str8_starts_with_nocase(url, str8_lit("http://"))){
        port = INTERNET_DEFAULT_HTTP_PORT;
        str8_trim_left(&url, 7);
    }

    u64 slash_idx = str8_index_from_left(url, '/');
    String8 hostname = str8_split_left(url, slash_idx);
    String16 hostname_16 = os_utf16_from_utf8(scratch.arena, hostname);
    if(win32_http_state->hSession != 0){
        // Associate the session with a port.
        hConnect = WinHttpConnect(win32_http_state->hSession, (wchar*)hostname_16.str, port, 0 );
        ok = (hConnect != 0);
    }

    String8Join join = {0};
    String8List full_parts = {0};
    String8 path = {0};
    if(slash_idx == u64_max){ // if no slash found
        path = str8_lit("/");
    }
    else{
        path = str8_split_right(url, slash_idx - 1);
    }
    str8_list_push(scratch.arena, &full_parts, path);

    if(info->params.node_count){
        join.mid = str8_lit("&");
        String8 params = str8_join(scratch.arena, &info->params, &join);
        str8_list_push(scratch.arena, &full_parts, params);
    }

    join.mid = str8_lit("?");
    String8 full_path = str8_join(scratch.arena, &full_parts, &join);

    String16 full_path_16 = os_utf16_from_utf8(scratch.arena, full_path);
    String16 method_16 = os_utf16_from_utf8(scratch.arena, info->method);
    if(ok){
        // Create an HTTP Request handle associated with the connection. Includes HTTP method(get,post,...), path, query, flags.
        hRequest = WinHttpOpenRequest(hConnect, (wchar*)method_16.str, (wchar*)full_path_16.str, 0, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
        ok = (hRequest != 0);
    }

    String8List headers_list = {0};
    if(info->content_type.size != 0) { str8_list_pushf(scratch.arena, &headers_list, "Content-Type: %s\r\n", info->content_type.str); }
    if(info->auth.size != 0)         { str8_list_pushf(scratch.arena, &headers_list, "Authorization: %s\r\n", info->auth.str); }
    if(info->user_agent.size != 0)   { str8_list_pushf(scratch.arena, &headers_list, "User-Agent: %s\r\n", info->user_agent.str); }
    if(info->cookie.size != 0)       { str8_list_pushf(scratch.arena, &headers_list, "Cookie: %s\r\n", info->cookie.str); }
    String8 headers = str8_join(scratch.arena, &headers_list, 0);
    String16 headers_16 = os_utf16_from_utf8(scratch.arena, headers);
    if(ok){
        ok = !!WinHttpAddRequestHeaders(hRequest, (wchar*)headers_16.str, (DWORD)headers_16.size, WINHTTP_ADDREQ_FLAG_ADD);
    }

    if(ok){
        // Initiates the outbound half of the HTTP transaction.
        // (resolve dns, establish tcp connection, start TLS handshake, send request headers, send request body)
        ok = !!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    }

    if(ok){
        // Initiates the inbound half of the HTTP transaction.
        // (wait for server to respond, finish HTTP handshake, allow status code & headers to be querued, moved WinHTTP into "response ready" state)
        ok = !!WinHttpReceiveResponse(hRequest, 0);
    }

    // Get response code.
    DWORD status_code_out = 0;
    DWORD status_code_size = sizeof(status_code_out);
    if(ok){
        WinHttpQueryHeaders(hRequest,
                            WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                            &status_code_out, &status_code_size, WINHTTP_NO_HEADER_INDEX);
    }
    status_code = status_code_out;

    // Get response data.
    String8List response_data = {0};
    if(ok){
        u32 downloaded = 0;
        u32 size = 0;
        do {
            if(!WinHttpQueryDataAvailable(hRequest, (DWORD*)&size)){
                print_last_error(GetLastError());
                ok = false;
            }

            if(size){
                String8 buffer = {0};
                buffer.str = push_array_zero(scratch.arena, u8, size + 1);
                buffer.size = size;

                if(!WinHttpReadData(hRequest, (void*)buffer.str, size, (DWORD*)&downloaded)){
                    print_last_error(GetLastError());
                    ok = false;
                }

                str8_list_push(scratch.arena, &response_data, buffer);
            }
        } while(size > 0);
    }

    HTTP_Response response = {0};
    response.body = str8_join(win32_http_state->arena, &response_data, 0);
    response.status_code = status_code;
    response.ok = ok;

    // Close handles.
    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    end_scratch(scratch);

    return(response);
}

