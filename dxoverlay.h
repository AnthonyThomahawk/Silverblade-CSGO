#pragma once
#include "dxrenderer.h"
class dx_overlay {
private:
	struct wnd_rect_t : public RECT {
		int width() { return right - left; }
		int height() { return bottom - top; }
	};

public:
	dx_overlay() { }
	dx_overlay(std::wstring target_class, std::wstring target_window, bool not_topmost);
	~dx_overlay();
	
	void initoverlay(std::wstring target_class, std::wstring target_window, bool not_topmost);
	dx_renderer create_renderer();
	HWND get_overlay_wnd();

	bool is_initialized();

private:
	void init_dx9();
	void create_overlay(std::wstring target_class, std::wstring target_window);
	int m_fps = 0;
	bool m_initialized = false;

	static bool m_not_topmost;

	static HWND m_overlay_wnd, m_target_wnd;
	wnd_rect_t m_overlay_wnd_size, m_target_wnd_size;

	IDirect3D9* m_d3d = nullptr;
	IDirect3DDevice9* m_device = nullptr;

	static LRESULT CALLBACK m_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};