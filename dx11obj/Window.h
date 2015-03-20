#ifndef WINDOW_H
#define WINDOW_H

#include "stdafx.h"

/**
 * Win32 window class
 */
class Window {
public:
	HWND m_Hwnd;
	HINSTANCE m_Instance;

	void Create( int width, int height, const wstring& title  );
	
	inline void GetRect(RECT& rc) {
		GetClientRect(m_Hwnd, &rc);
	}

	void Text(const wstring& newText) {
		SetWindowTextW(m_Hwnd, newText.c_str());
	}

	virtual LRESULT Procedure(UINT msg, WPARAM wParam, LPARAM lParam); // this function can be overridden in derivative windows

	void MessageLoop();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif