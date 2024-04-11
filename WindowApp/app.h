#pragma once

#include "device_resources.h"
#include "renderer.h"

#include <string>
#include <memory>

#include "WinUser.h"
#include "Windows.h"


class app {
private:
    HMENU menu_handle;
    RECT rect_handle;
    HWND window_handle;

    static HINSTANCE h_instance;
    static std::wstring window_name;
public:
    app();

    HRESULT ñreate_window();

    HWND get_window_handle() { return window_handle; };

    static LRESULT CALLBACK static_window_proc(
        HWND hwnd, UINT u_msg,
        WPARAM w_param, LPARAM l_param
    );

    HRESULT run(
        std::shared_ptr<device_resources> deviceResources,
        std::shared_ptr<renderer> renderer
    );
};
