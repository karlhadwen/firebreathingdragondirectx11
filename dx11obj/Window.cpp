#include "stdafx.h"
#include "Window.h"

// standard windows stuff (basically the window)
void Window::Create(int width, int height, const wstring& title) {
	// register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Dx11EngineClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&wcex))
		throw exception("Failed to register class");

	// create window
	m_Instance = wcex.hInstance;
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_Hwnd = CreateWindow(L"Dx11EngineClass", title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_Instance,
		this);
	if (!m_Hwnd)
		throw exception("Failed to create window");

	ShowWindow(m_Hwnd, SW_SHOW);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Window* realWin = nullptr;

	if (message == WM_CREATE) {
		// collected here..
		realWin = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		// .. and then stored for later lookup
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)realWin);
	} else {
		realWin = (Window*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	}

	if (realWin) {
		return realWin->Procedure(message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::Procedure(UINT msg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(this->m_Hwnd, &ps);
		EndPaint(this->m_Hwnd, &ps);
		break;

	default:
		return DefWindowProc(this->m_Hwnd, msg, wParam, lParam);
	}

	return 0;
}