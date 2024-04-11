#include "app.h"

app::app() {
	this->window_name = L"WindowAPP";
	this->h_instance = NULL;
}

HRESULT app::ñreate_window() {
    if (this->h_instance == NULL)
        this->h_instance = (HINSTANCE)GetModuleHandle(NULL);

    HICON h_icon = NULL;
    WCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    // If the icon is NULL, then use the first one found in the exe
    if (h_icon == NULL)
        h_icon = ExtractIcon(this->h_instance, szExePath, 0);

    // Register the windows class
    WNDCLASS wndClass;
    wndClass.style = CS_DBLCLKS;
    wndClass.lpfnWndProc = app::static_window_proc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = this->h_instance;
    wndClass.hIcon = h_icon;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = this->window_name.c_str();

    if (!RegisterClass(&wndClass))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
            return HRESULT_FROM_WIN32(dwError);
    }

    rect_handle;
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    // No menu in this example.
    this->menu_handle = NULL;

    // This example uses a non-resizable 640 by 480 viewport for simplicity.
    int nDefaultWidth = 640;
    int nDefaultHeight = 480;
    SetRect(&rect_handle, 0, 0, nDefaultWidth, nDefaultHeight);
    AdjustWindowRect(
        &rect_handle,
        WS_OVERLAPPEDWINDOW,
        (menu_handle != NULL) ? true : false
    );

    // Create the window for our viewport.
    this->window_handle = CreateWindow(
        this->window_name.c_str(), L"Window",
        WS_OVERLAPPEDWINDOW, x, y,
        (rect_handle.right - rect_handle.left),
        (rect_handle.bottom - rect_handle.top),
        0, menu_handle, this->h_instance, 0
    );

    if (this->window_handle == NULL)
    {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }

    return S_OK;
}

HRESULT app::run(
    std::shared_ptr<device_resources> deviceResources,
    std::shared_ptr<renderer> renderer
) {
    if (!IsWindowVisible(this->window_handle))
        ShowWindow(this->window_handle, SW_SHOW);

    // The render loop is controlled here.
    bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while (WM_QUIT != msg.message) {
        // Process window events.
        // Use PeekMessage() so we can use idle time to render the scene. 
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg) {
            // Translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Update the scene.
            //renderer->update();

            // Render frames during idle time (when no messages are waiting).
            //renderer->render();

            // Present the frame to the screen.
            //deviceResources->present();
        }
    }

    return S_OK;
}

LRESULT CALLBACK app::static_window_proc(
	HWND hwnd, UINT u_msg,
	WPARAM w_param, LPARAM l_param
) {
    switch (u_msg)
    {
    case WM_CLOSE:
        HMENU hMenu;
        hMenu = GetMenu(hwnd);
        if (hMenu != NULL) {
            DestroyMenu(hMenu);
        }
        DestroyWindow(hwnd);

        // I use GetProp/SetProp to hide some properties
        UnregisterClass(
            (LPWSTR)GetProp(hwnd, L"window_name"),
            (HINSTANCE)GetProp(hwnd, L"h_instance")
        );
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, u_msg, w_param, l_param);
}
