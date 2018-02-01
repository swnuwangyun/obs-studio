#pragma once

#include <obs.h>
#include <string>

class MiniOBS
{
public:
	MiniOBS();
	~MiniOBS();

	static MiniOBS* instance();

public:
	void bind(HWND hwnd);

private:
	static void render(void *data, uint32_t cx, uint32_t cy);

private:
	obs_display_t *m_display;
};

